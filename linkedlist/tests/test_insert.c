#include <stdio.h>
#include <stdlib.h>

#include <time.h>
#include <wchar.h>

#include <linkedlist.h>
#include <uep_wide.h>

LinkedList *test1;
LinkedList *test2;
LinkedList *test3;
LinkedList *test4;

struct timespec respire={0L,30000000L};

int 	iRandom;
char	*Random;

typedef struct s_Random
{
	int Random;
	char *sRandom;
}t_Random;

double dRandom;

int cpt;
int idx;

char 	Message[80];

void main(void)
{
	test1=lc_init();
	test2=lc_init();
	test3=lc_init();
	test4=lc_init();
	
	EffacerEcran();
	while(cpt<1200)
	{
		srand(time(NULL));
		t_Random *tmpRand=malloc(sizeof(t_Random));
		iRandom=rand()%40+1;
		dRandom=rand()%1000/rand()%50+1;
		tmpRand->Random=rand()%100+1;
		tmpRand->sRandom=malloc(sizeof(char)*10);
		Random=malloc(sizeof(char)*20);
		
		for(idx=0;idx<10;idx++)
		{
			tmpRand->sRandom[idx]=rand()%26+65;
		}
		for(idx=0;idx<20;idx++)
		{
			Random[idx]=rand()%26+65;
		}
		
		//printf("> tmpRand->sRandom <-- %s\n",tmpRand->sRandom);
		//printf("> Random <-- %s\n",Random);
		
		lc_insert((void*)&iRandom,test1,cssmint,sizeof(int));
		lc_insert((void*)&dRandom,test2,cssmdouble,sizeof(double));
		lc_insert((void*)tmpRand,test3,cssmuserdef,sizeof(t_Random));
		lc_insert((void*)Random,test4,cssmuserdef,sizeof(char*));
						
		sprintf(Message,"Progress %04.0f%% %04d",(float)(test1->NbElem/1200.0)*100.0,test1->NbElem);
		nanosleep(&respire,NULL);
		AfficherXY(Message,1,1);
		
		cpt++;
	}
	wprintf(L"\n%04d %04d %04d %04d\n",test1->NbElem,test2->NbElem,test3->NbElem,test4->NbElem);
			
	lc_empty(test1);
	lc_empty(test2);
	lc_empty(test3);
	lc_empty(test4);
	
	BackToShell(2,1);
}
