#include <stdio.h>
#include <stdlib.h>
#include <uep_wide.h>
#include <wchar.h>

char *pTest1,*pTest2;

int main(int argc,char *argv[])
{
  InitUEPWIDE("fr_BE.UTF-8");
  
  wprintf(L"\nTest de saisie d'un caractère UTF8 (ou non) suivi d'un autre caractère UTF8 (ou non)...\n");
  pTest1=getnchar(1);  	
  wprintf(L"\t-\t");
  pTest2=getnchar(1);
   
  wprintf(L"\n");  
  
  wprintf(L"Mode caractère \"wide\" (%%lc)...\t");
  wprintf(L"%lc - %lc\n",*pTest1,*pTest2);				// ne lit qu'un seul caractère... 
  wprintf(L"Mode caractère (%%c)...\t");
  wprintf(L"\t%c - %c\n",*pTest1,*pTest2);				// ne lit qu'un seul caractère... 
  wprintf(L"Mode string (%%s)...\t");
  wprintf(L"\t%s - %s\n",pTest1,pTest2); 
  
  wprintf(L"%%lc cherche à lire un \x1b[1mwchar_t\x1b[0m (2 bytes) OR *pTest1 est un \x1b[1mchar\x1b[0m...\n");
  wprintf(L"%%c cherche à lire un \x1b[1mchar\x1b[0m et ne saura donc pas afficher un \x1b[1mwchar_t\x1b[0m...\n");
  wprintf(L"%%s fonctionne vu que la chaîne contient le nombre de caractères suffisant pour afficher soit des \x1b[1mwchar_t\x1b[0m ou des \x1b[1mchar\x1b[0m...\n");
  
  // il faut savoir si oui ou non nous avons à faire avec un "wide character"...  
  
  if(*pTest1<0)
  {
    char *pReturnPointer=(char*)calloc(1,sizeof(char)*3);						// le code -61 (ou -62), le code du caractère et \0...
    pReturnPointer[0]=*pTest1;																			// indique qu'il s'agit d'UTF8 (pour l'affichage)
    pReturnPointer[1]=*(++pTest1);																	// les caractères UTF8 ne sont pas complets jusqu'à ce que 2 bytes aient été "saisis" le getch() ici récupère la partie manquante...
    pReturnPointer[2]='\0';  
  
    wchar_t todisplay;
    int rc=mbtowc(&todisplay,pReturnPointer,2);
    if(rc>0)
	    wprintf(L"(lc)pTest1 --> %lc\n",todisplay);
  }
	else
	  wprintf(L"(c)pTest1 --> %c\n",*pTest1);  
	
	if(*pTest2<0)
	{
	  char *pReturnPointer=(char*)calloc(1,sizeof(char)*3);						// le code -61 (ou -62), le code du caractère et \0...
	  pReturnPointer[0]=*pTest2;																			// indique qu'il s'agit d'UTF8 (pour l'affichage)
	  pReturnPointer[1]=*(++pTest2);																	// les caractères UTF8 ne sont pas complets jusqu'à ce que 2 bytes aient été "saisis" le getch() ici récupère la partie manquante...
	  pReturnPointer[2]='\0';  
  
	  wchar_t todisplay;
	  int rc=mbtowc(&todisplay,pReturnPointer,2);
	  if(rc>0)
	    wprintf(L"(lc)pTest2 --> %lc\n",todisplay);
  }
	else
	  wprintf(L"(c)pTest2 --> %c\n",*pTest2);  
  
  exit(0);
}


