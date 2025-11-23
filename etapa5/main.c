#include <stdio.h>
#include "asd.h"
#include "iloccode.h"
#include "label.h"
extern int yyparse(void);
extern int yylex_destroy(void);
asd_tree_t *arvore = NULL;
int main ()
{
  ILOCCode_start();
  Label_start();
  int ret = yyparse();
  if (arvore && arvore->code) {
      ILOCCode_print(arvore->code);
  }
  ILOCCode_end();
  Label_end();
  asd_free(arvore);
  yylex_destroy();
  return ret;
}