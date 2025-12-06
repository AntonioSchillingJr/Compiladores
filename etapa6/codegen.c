#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "codegen.h"
#include "semantics.h"
#include "ilocinstruction.h"

/* Converte "r123" em 123; retorna -1 se não for registrador */
static int parse_reg(const char *s) {
    if (!s || s[0] != 'r') return -1;
    char *endptr;
    long v = strtol(s + 1, &endptr, 10);
    if (*endptr != '\0' || v < 0) return -1;
    return (int)v;
}

/* Converte string de inteiro decimal em int */
static int parse_int(const char *s) {
    if (!s) return 0;
    char *endptr;
    long v = strtol(s, &endptr, 10);
    (void)endptr;
    return (int)v;
}

/* Offset (em bytes) para variáveis locais, relativo a %rbp.
   logical_off vem do ILOC (0,4,8,...) e viram -4,-8,-12,... */
static int local_stack_offset(int logical_off) {
    return -(logical_off + 4);
}

/* Offset (em bytes) para temporários rN, relativo a %rbp.
   A área de temporários fica "abaixo" da área de locais. */
static int temp_stack_offset(int reg_id, int local_area_size) {
    return -(local_area_size + 4 * (reg_id + 1));
}

static void analyze_iloc(ILOCCode *code,
                         int *out_max_reg,
                         int *out_max_rfp_off,
                         int *out_max_rbss_off)
{
    int max_reg  = -1;
    int max_rfp  = -1;
    int max_rbss = -1;

    for (int i = 0; i < code->count; i++) {
        ILOCInstruction *in = &code->instructions[i];

        if (in->src1) {
            int r = parse_reg(in->src1);
            if (r > max_reg) max_reg = r;
        }
        if (in->src2) {
            int r = parse_reg(in->src2);
            if (r > max_reg) max_reg = r;
        }
        if (in->dest) {
            int r = parse_reg(in->dest);
            if (r > max_reg) max_reg = r;
        }

        /* Offsets em loadAI / storeAI */
        if (in->op_code && !strcmp(in->op_code, "loadAI")) {
            if (in->src1 && in->src2) {
                int off = parse_int(in->src2);
                if (!strcmp(in->src1, "rfp")) {
                    if (off > max_rfp) max_rfp = off;
                } else if (!strcmp(in->src1, "rbss")) {
                    if (off > max_rbss) max_rbss = off;
                }
            }
        } else if (in->op_code && !strcmp(in->op_code, "storeAI")) {
            if (in->dest) {
                char base[32], off_str[32];
                if (sscanf(in->dest, " %31[^,], %31s", base, off_str) == 2) {
                    int off = parse_int(off_str);
                    if (!strcmp(base, "rfp")) {
                        if (off > max_rfp) max_rfp = off;
                    } else if (!strcmp(base, "rbss")) {
                        if (off > max_rbss) max_rbss = off;
                    }
                }
            }
        }
    }

    if (out_max_reg)      *out_max_reg      = max_reg;
    if (out_max_rfp_off)  *out_max_rfp_off  = max_rfp;
    if (out_max_rbss_off) *out_max_rbss_off = max_rbss;
}

/* Emite uma instrução ILOC como assembly */
static void emit_instruction(ILOCInstruction *in, int local_area_size)
{
    if (!in || !in->op_code) return;

    /* Labels: op_code termina com ':' (ex: "L0:") */
    size_t len = strlen(in->op_code);
    if (len > 0 && in->op_code[len - 1] == ':') {
        printf("%s\n", in->op_code);
        return;
    }

    /* jumpI label */
    if (!strcmp(in->op_code, "jumpI")) {
        if (in->dest) {
            printf("\tjmp\t%s\n", in->dest);
        }
        return;
    }

    /* cbr cond -> Ltrue, Lfalse */
    if (!strcmp(in->op_code, "cbr")) {
        if (in->src1 && in->dest) {
            int rcond = parse_reg(in->src1);
            int off_cond = temp_stack_offset(rcond, local_area_size);

            char ltrue[32], lfalse[32];
            if (sscanf(in->dest, " %31[^,], %31s", ltrue, lfalse) == 2) {
                printf("\tmovl\t%d(%%rbp), %%eax\n", off_cond);
                printf("\tcmpl\t$0, %%eax\n");
                printf("\tjne\t%s\n", ltrue);
                printf("\tjmp\t%s\n", lfalse);
            }
        }
        return;
    }

    /* loadI c => rD */
    if (!strcmp(in->op_code, "loadI")) {
        if (in->src1 && in->dest) {
            int rd = parse_reg(in->dest);
            int off_d = temp_stack_offset(rd, local_area_size);
            printf("\tmovl\t$%s, %d(%%rbp)\n", in->src1, off_d);
        }
        return;
    }

    /* loadAI base, off => rD */
    if (!strcmp(in->op_code, "loadAI")) {
        if (in->src1 && in->src2 && in->dest) {
            int rd = parse_reg(in->dest);
            int off_d = temp_stack_offset(rd, local_area_size);
            int off = parse_int(in->src2);

            if (!strcmp(in->src1, "rfp")) {
                int loc_off = local_stack_offset(off);
                printf("\tmovl\t%d(%%rbp), %%eax\n", loc_off);
                printf("\tmovl\t%%eax, %d(%%rbp)\n", off_d);
            } else if (!strcmp(in->src1, "rbss")) {
                printf("\tmovl\trbss+%d(%%rip), %%eax\n", off);
                printf("\tmovl\t%%eax, %d(%%rbp)\n", off_d);
            }
        }
        return;
    }

    /* storeAI rS => base, off */
    if (!strcmp(in->op_code, "storeAI")) {
        if (in->src1 && in->dest) {
            int rs = parse_reg(in->src1);
            int off_s = temp_stack_offset(rs, local_area_size);

            char base[32], off_str[32];
            if (sscanf(in->dest, " %31[^,], %31s", base, off_str) == 2) {
                int off = parse_int(off_str);

                printf("\tmovl\t%d(%%rbp), %%eax\n", off_s);
                if (!strcmp(base, "rfp")) {
                    int loc_off = local_stack_offset(off);
                    printf("\tmovl\t%%eax, %d(%%rbp)\n", loc_off);
                } else if (!strcmp(base, "rbss")) {
                    printf("\tmovl\t%%eax, rbss+%d(%%rip)\n", off);
                }
            }
        }
        return;
    }

    /* binárias aritméticas/lógicas: add, sub, mult, div, and, or */
    if (!strcmp(in->op_code, "add")  ||
        !strcmp(in->op_code, "sub")  ||
        !strcmp(in->op_code, "mult") ||
        !strcmp(in->op_code, "div")  ||
        !strcmp(in->op_code, "and")  ||
        !strcmp(in->op_code, "or")) {

        if (in->src1 && in->src2 && in->dest) {
            int r1 = parse_reg(in->src1);
            int r2 = parse_reg(in->src2);
            int rd = parse_reg(in->dest);
            int o1 = temp_stack_offset(r1, local_area_size);
            int o2 = temp_stack_offset(r2, local_area_size);
            int od = temp_stack_offset(rd, local_area_size);

            printf("\tmovl\t%d(%%rbp), %%eax\n", o1);

            if (!strcmp(in->op_code, "add")) {
                printf("\taddl\t%d(%%rbp), %%eax\n", o2);
            } else if (!strcmp(in->op_code, "sub")) {
                printf("\tsubl\t%d(%%rbp), %%eax\n", o2);
            } else if (!strcmp(in->op_code, "mult")) {
                printf("\timull\t%d(%%rbp), %%eax\n", o2);
            } else if (!strcmp(in->op_code, "div")) {
                printf("\tcltd\n");
                printf("\tmovl\t%d(%%rbp), %%ecx\n", o2);
                printf("\tidivl\t%%ecx\n");
            } else if (!strcmp(in->op_code, "and")) {
                printf("\tandl\t%d(%%rbp), %%eax\n", o2);
            } else if (!strcmp(in->op_code, "or")) {
                printf("\torl\t%d(%%rbp), %%eax\n", o2);
            }

            printf("\tmovl\t%%eax, %d(%%rbp)\n", od);
        }
        return;
    }

    /* rsubI: unário '-' (0 - rS) */
    if (!strcmp(in->op_code, "rsubI")) {
        if (in->src1 && in->dest) {
            int rs = parse_reg(in->src1);
            int rd = parse_reg(in->dest);
            int os = temp_stack_offset(rs, local_area_size);
            int od = temp_stack_offset(rd, local_area_size);

            printf("\tmovl\t%d(%%rbp), %%eax\n", os);
            printf("\tnegl\t%%eax\n");
            printf("\tmovl\t%%eax, %d(%%rbp)\n", od);
        }
        return;
    }

    /* comparações: cmp_* */
    if (!strncmp(in->op_code, "cmp_", 4)) {
        if (in->src1 && in->src2 && in->dest) {
            int r1 = parse_reg(in->src1);
            int r2 = parse_reg(in->src2);
            int rd = parse_reg(in->dest);
            int o1 = temp_stack_offset(r1, local_area_size);
            int o2 = temp_stack_offset(r2, local_area_size);
            int od = temp_stack_offset(rd, local_area_size);

            printf("\tmovl\t%d(%%rbp), %%eax\n", o1);
            printf("\tcmpl\t%d(%%rbp), %%eax\n", o2);

            const char *set_instr = "sete";
            if (!strcmp(in->op_code, "cmp_EQ"))      set_instr = "sete";
            else if (!strcmp(in->op_code, "cmp_NE")) set_instr = "setne";
            else if (!strcmp(in->op_code, "cmp_LT")) set_instr = "setl";
            else if (!strcmp(in->op_code, "cmp_LE")) set_instr = "setle";
            else if (!strcmp(in->op_code, "cmp_GT")) set_instr = "setg";
            else if (!strcmp(in->op_code, "cmp_GE")) set_instr = "setge";

            printf("\t%s\t%%al\n", set_instr);
            printf("\tmovzbl\t%%al, %%eax\n");
            printf("\tmovl\t%%eax, %d(%%rbp)\n", od);
        }
        return;
    }
}

/* Função principal */
void generate_assembly(ILOCCode *code)
{
    if (!code) return;

    int max_reg, max_rfp, max_rbss;
    analyze_iloc(code, &max_reg, &max_rfp, &max_rbss);

    int local_area_size  = (max_rfp  >= 0) ? (max_rfp  + 4) : 0;
    int temp_count       = (max_reg  >= 0) ? (max_reg  + 1) : 0;
    int temps_area_size  = temp_count * 4;
    int stack_size       = local_area_size + temps_area_size;
    int rbss_size        = (max_rbss >= 0) ? (max_rbss + 4) : 0;

    /* segmento de dados (rbss único para globais) */
    if (rbss_size > 0) {
        printf("\t.data\n");
        printf("\t.bss\n");
        printf("\t.align 4\n");
        printf("\t.globl rbss\n");
        printf("rbss:\n");
        printf("\t.zero %d\n", rbss_size);
    }
    printf("\t.text\n");

    /* função main */
    printf("\t.globl main\n");
    printf("\t.type main, @function\n");
    printf("main:\n");
    printf("\tpushq\t%%rbp\n");
    printf("\tmovq\t%%rsp, %%rbp\n");
    if (stack_size > 0) {
        printf("\tsubq\t$%d, %%rsp\n", stack_size);
    }

    /* Corpo: traduzindo o ILOC sequencialmente */
    for (int i = 0; i < code->count; i++) {
        emit_instruction(&code->instructions[i], local_area_size);
    }

    /* Valor de retorno da main em %%eax */
    if (SEM_main_has_return && SEM_main_return_place) {
        int rret = parse_reg(SEM_main_return_place);
        if (rret >= 0) {
            int off = temp_stack_offset(rret, local_area_size);
            printf("\tmovl\t%d(%%rbp), %%eax\n", off);
        } else {
            printf("\tmovl\t$0, %%eax\n");
        }
    } else {
        /* Se não achar retorno, devolve 0 */
        printf("\tmovl\t$0, %%eax\n");
    }

    printf("\tleave\n");
    printf("\tret\n");
}
