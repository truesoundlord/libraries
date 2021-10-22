#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>
#include <wchar.h>
#include <unistd.h>
#include <math.h>

#include <linkedlist.h>
#include <uep_wide.h>

LinkedList *test1;
LinkedList *test2;
LinkedList *test3;
LinkedList *test4;

struct timespec respire={0L,250000000L};

int 	*iRandom;
char	*Random;

typedef struct s_Random
{
	int Random;
	char *sRandom;
}t_Random;

double *dRandom;

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
	while(cpt<80)
	{
		srand(time(NULL));
		t_Random *tmpRand=malloc(sizeof(t_Random));
		iRandom=malloc(sizeof(int));
		dRandom=malloc(sizeof(double));
		*iRandom=rand()%4000+1;
		*dRandom=(double)sqrt(rand()%1000);
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
		
		//wprintf(L"> iRandom <-- %d\n",*iRandom);
		//wprintf(L"> Random <-- %lf\n",*dRandom);
		
		//wprintf(L"> tmpRand->sRandom <-- %s\n",tmpRand->sRandom);
		//wprintf(L"> Random <-- %s\n",Random);
		
		lc_insert((void*)iRandom,test1,cssmint,sizeof(int));
		lc_insert((void*)dRandom,test2,cssmdouble,sizeof(double));
		lc_insert((void*)tmpRand,test3,cssmuserdef,sizeof(t_Random));
		lc_insert((void*)Random,test4,cssmuserdef,sizeof(char*));
						
		sprintf(Message,"Progress %04.0f%% %04d",(float)(test1->NbElem/120.0)*100.0,test1->NbElem);
		nanosleep(&respire,NULL);
		AfficherXY(Message,1,1);
		
		cpt++;
	}
	wprintf(L"\n%04d %04d %04d %04d\n",test1->NbElem,test2->NbElem,test3->NbElem,test4->NbElem);
	
	cpt=0;
	
	while(cpt<40)
	{
		lc_Datas *tmp1,*tmp2,*tmp3,*tmp4;
		tmp1=lc_pop(test1);
		tmp2=lc_pop(test2);
		tmp3=lc_pop(test3);
		tmp4=lc_pop(test4);
		
		int *iTmp=(int*)tmp1->value;
		double *dTmp=(double*)tmp2->value;
		t_Random *elemTmp=(t_Random*)tmp3->value;
		char *sTmp=(char*)tmp4->value;
		
		sprintf(Message,"[%04d][%9.5lf][%20s][%20s] (%04d)",*iTmp,(double)(*dTmp),elemTmp->sRandom,sTmp,test1->NbElem);
		AfficherXY(Message,1,1);
		/*sprintf(Message,"[%04d][%04.3lf][%20s][%20s] (%04d)",*iTmp,(double)(*dTmp),elemTmp->sRandom,sTmp,test2->NbElem);
		AfficherXY(Message,1,2);
		sprintf(Message,"[%04d][%04.3lf][%20s][%20s] (%04d)",*iTmp,(double)(*dTmp),elemTmp->sRandom,sTmp,test3->NbElem);
		AfficherXY(Message,1,3);
		sprintf(Message,"[%04d][%04.3lf][%20s][%20s] (%04d)",*iTmp,(double)(*dTmp),elemTmp->sRandom,sTmp,test4->NbElem);
		AfficherXY(Message,1,4);*/
		
		nanosleep(&respire,NULL);
		cpt++;
	}
	SetCursor(1,5);
	wprintf(L"\n%04d %04d %04d %04d\n",test1->NbElem,test2->NbElem,test3->NbElem,test4->NbElem);
	
	lc_empty(test1);
	lc_empty(test2);
	lc_empty(test3);
	lc_empty(test4);
	
	BackToShell(1,6);
}
