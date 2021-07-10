#include <stdio.h>
#include <stdlib.h>

#include <time.h>

#include <linkedlist.h>

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

void main(void)
{
	test1=lc_init();
	test2=lc_init();
	test3=lc_init();
	test4=lc_init();
	
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
		
		lc_add((void*)&iRandom,test1,cssmint,sizeof(int));
		lc_add((void*)&dRandom,test2,cssmdouble,sizeof(double));
		lc_add((void*)tmpRand,test3,cssmuserdef,sizeof(t_Random));
		lc_add((void*)Random,test4,cssmuserdef,sizeof(char*));
		
		
				
		nanosleep(&respire,NULL);
		
		cpt++;
	}
	printf("%04d %04d %04d %04d\n",test1->NbElem,test2->NbElem,test3->NbElem,test4->NbElem);
			
	
	
}
