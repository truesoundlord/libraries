#include <stdio.h>
#include <stdlib.h>
#include <uep_wide.h>
#include <wchar.h>

char *pTest;

int main(int argc,char *argv[])
{
  InitUEPWIDE("fr_BE.UTF-8");
  
  wprintf(L"\nTest vingt caractères UTF8\n");
  pTest=getnchar(20);  	
  
  wprintf(L"\n");  
  
  exit(0);
}
