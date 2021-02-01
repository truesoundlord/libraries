#include <stdio.h>
#include <stdlib.h>
#include <uep_wide.h>

char *pTest;

int main(int argc,char *argv[])
{
  InitUEPWIDE("fr_BE.UTF-8");
  
  wprintf(L"\nTest d'un caractère UTF8 ou non\n");
  pTest=getnchar(1);  	
  
  wprintf(L"\n");  
  
  exit(0);
}
