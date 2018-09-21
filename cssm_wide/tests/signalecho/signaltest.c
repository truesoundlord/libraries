#include <stdio.h>
#include <stdlib.h>

#include <signal.h>

#include <cssm_wide.h>

void ArretPropre(int);

int main(int argc,char *argv[])
{
		
	struct sigaction detournement;
	struct sigaction protection;
		
	printf("Programme de test concernant la fonction suivante: \n");
	printf("\ttoggleSignalsEcho()\n");
	
	printf("Détournement des signaux...\n");
	
	detournement.sa_handler=ArretPropre;
	sigaction(SIGHUP,&detournement,NULL);					// kill -1
	sigaction(SIGQUIT,&detournement,NULL);				// kill -3
	sigaction(SIGTERM,&detournement,NULL);				// kill -15
	sigaction(SIGSEGV,&detournement,NULL);				// kill -11 
	sigaction(SIGCONT,&detournement,NULL);				// kill -18
	sigaction(SIGABRT,&detournement,NULL);				// kill -6
	
	protection.sa_handler=SIG_IGN;
	sigaction(SIGINT,&protection,NULL);						// ignorer le CTRL-C
	sigaction(SIGPIPE,&protection,NULL);					// kill -13 (là je suis pas certain de ce que je fais :{ )
		
	// 25 jan 2018
	//sigaction(SIGTSTP,&protection,NULL);					// ignorer le CTRL-Z (mais il affiche une merde à l'écran quand même :{) [résolu en juillet 2018 -- amélioration de la fonction toggleSignalsEcho()]
	sigaction(SIGTSTP,&detournement,NULL);
	
	if(argv[1]!=NULL)
	 toggleSignalsEcho();
	
	while(1)
	{
		printf("waiting...\n");
		sleep(1);
	}
	exit(0);
}

void ArretPropre(int signal)
{
	unsigned int uiHeight;
	unsigned int uiWidth;
	
	GetConsoleDimensions(&uiWidth,&uiHeight);
	
	fprintf(stderr,"[%s] Signal %03d\n",__func__,signal);
		
	switch(signal)
	{
		case SIGINT:
		case SIGTERM:
		case SIGHUP:
		case SIGQUIT:
		case SIGABRT:
		case SIGPIPE:	
									break;
		case SIGSEGV: 
									SetCursor(1,uiHeight-3);
									exit(1);
		case SIGTSTP: fprintf(stderr,"[%s] STOP !!",__func__);
									break;
		case SIGCONT: fprintf(stderr,"...continue");
									break;
				
	}
	//toggleSignalsEcho();
}