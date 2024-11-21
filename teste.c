#include <stdio.h>
#include "cria_func.h"

typedef int (*func_ptr) (int x);

int teste(int x){
  return x;
}

int main (void) {
  DescParam params[1];
  func_ptr f_teste;
  int i;
  unsigned char codigo[500];

  params[0].tipo_val = INT_PAR; 
  params[0].orig_val = PARAM;

  cria_func (teste, params, 1, codigo);
  f_teste = (func_ptr) codigo;   

  printf("Resultado: %d", f_teste(2));

  return 0;
}