#include <stdio.h>
#include <stdlib.h>
#include <uep_wide.h>

char *pTest1,*pTest2;

int main(int argc,char *argv[])
{
  InitUEPWIDE("fr_BE.UTF-8");
  
  wprintf(L"\nTest d'un caractère UTF8 ou non suivi d'un autre caractère...\n");
  pTest1=getnchar(1);  	
  wprintf(L"\t-\t");
  pTest2=getnchar(1);
   
  wprintf(L"\n");  
  wprintf(L"%c - %c\n",*pTest1,*pTest2); 
  
  exit(0);
}
