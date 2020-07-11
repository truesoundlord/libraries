/* 
 * File:   ll_perftests.c
 * Author: Dimitri "Hurukan" <soundlord@gmail.com>
 *
 * Créé le July 2, 2020, 11:40 PM
 */

// ****************************************************************************
// HISTORIQUE
// ****************************************************************************

// **************************************************************************** 
// Fichiers de définitions
// **************************************************************************** 

#include <stdio.h>
#include <stdlib.h>
#include <linkedlist.h>
#include <stdint.h>
#include <time.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <cssm_wide.h>
#include <wchar.h>

// **************************************************************************** 
// Déclaration des constantes symboliques
// **************************************************************************** 

// **************************************************************************** 
// Déclaration des variables globales, externes, ...
// **************************************************************************** 

LinkedList *pTestPerf;

int *Hashes;
unsigned int *ideez;

unsigned int largeur;
unsigned int hauteur;

unsigned int cpt;
unsigned int target;

clock_t start;
clock_t stop;

pthread_t idThreadFill[10],idThreadSearch[10];
pthread_mutex_t mtxprod=PTHREAD_MUTEX_INITIALIZER;

PositionCadre mainwin;
char *strMessage;

// ****************************************************************************
// SECTION : prototypes des fonctions en test pour CE code source
// ****************************************************************************

void fillelems(void*);

/*
 * FONCTION PRINCIPALE
 */
int main(int argc,char** argv)
{
	// Programme de test de performances...
	int cptthfill=0;
	
	pTestPerf=lc_init();
	
	InitCSSMWIDE("fr_BE.UTF8");
	EffacerEcran();
	
	GetConsoleDimensions(&largeur,&hauteur);
	
	strMessage=calloc(largeur-2,sizeof(char));

	// Houlà !!
	
	mainwin=DrawBoxWithRGB(1,1,hauteur-3,largeur,getRGB(127,127,127,true));
	
	AddToMessageBoxEx("[TEST 5] 100.000 éléments",&mainwin);
	Hashes=calloc(100000,sizeof(int));
	ideez=calloc(100000,sizeof(int));	
	
	while(cptthfill<20)
	{
		pthread_create(&idThreadFill[cptthfill],NULL,(void*)fillelems,(void*)&cptthfill);
		sleep(1);
		fflush(stderr);
		cptthfill++;
	}	
	
	start=clock();
	sprintf(strMessage,"[main]");
	for(cpt=0;cpt<20;cpt++)
	{
		//sprintf(strMessage,"[main] waiting for thread %d to exit...",cpt);
		//AddToMessageBoxEx(strMessage,&mainwin);
		pthread_join(idThreadFill[cpt],NULL);
	}
	stop=clock();
	sprintf(strMessage,"\t[FILLING TIME] %2.6f second",(double)(stop-start)/CLOCKS_PER_SEC);
	AddToMessageBoxEx(strMessage,&mainwin);
	
	sprintf(strMessage,"[main] Filling done !!",cpt);
	AddToMessageBoxEx(strMessage,&mainwin);
	
	sprintf(strMessage,"[main] Elements in list %06d",pTestPerf->NbElem);
	AddToMessageBoxEx(strMessage,&mainwin);
	
	srand(time(NULL));
	
	AddToMessageBoxEx("Search randomly...",&mainwin);
	for(cpt=0;cpt<5;cpt++)
	{
		
		target=rand()%100000+1;
	
		sprintf(strMessage,"Searching for element for ID %06d",ideez[target]);
		AddToMessageBoxEx(strMessage,&mainwin);
	
		start=clock();
		
		lc_Datas *tmp=lc_search(pTestPerf,ideez[target]);
		if(tmp!=NULL) sprintf(strMessage,"\t[FOUND] %d",*(int*)tmp->value);
		else sprintf(strMessage,"\t[NOT FOUND] %08x",tmp);
		
		stop=clock();
		
		AddToMessageBoxEx(strMessage,&mainwin);
			
		sprintf(strMessage,"\t[PERF %d] %2.6f second",cpt,(double)(stop-start)/CLOCKS_PER_SEC);
		AddToMessageBoxEx(strMessage,&mainwin);
	}
	
	sleep(1);
	AddToMessageBoxEx("Search sequences... from 0 to 100.000 by 12000 step",&mainwin);
	for(cpt=0;cpt<100000;cpt+=12000)
	{
		
		target=cpt;
		
		sprintf(strMessage,"Searching for ID %06d (index %06d)",ideez[target],target);
		AddToMessageBoxEx(strMessage,&mainwin);
	
		start=clock();
		
		lc_Datas *tmp=lc_search(pTestPerf,ideez[target]);
		if(tmp!=NULL) sprintf(strMessage,"\t[FOUND] %d",*(int*)tmp->value);
		else sprintf(strMessage,"\t[NOT FOUND] %08x",tmp);
		
		stop=clock();
		
		AddToMessageBoxEx(strMessage,&mainwin);
	
		
		
		sprintf(strMessage,"\t[PERF] %2.6f second",(double)(stop-start)/CLOCKS_PER_SEC);
		AddToMessageBoxEx(strMessage,&mainwin);
	}
	
	AddToMessageBoxEx("strike a key for next test...",&mainwin);
	
	SetCursor(mainwin.FirstPrintableX,mainwin.Last);
	getch();
	
	ResetBox(&mainwin);
	//AddToMessageBoxEx("AFTER RESET DEBUG !!!!",&mainwin);
	//getch();
	
	AddToMessageBoxEx("Search sequences... from 48000 to 52.000 by 300 step",&mainwin);
	for(cpt=48000;cpt<52000;cpt+=300)
	{
		
		target=cpt;
		
		sprintf(strMessage,"Searching for ID %06d (index: %06d)",ideez[target],target);
		AddToMessageBoxEx(strMessage,&mainwin);
	
		start=clock();
		
		lc_Datas *tmp=lc_search(pTestPerf,ideez[target]);
		if(tmp!=NULL) sprintf(strMessage,"\t[FOUND] %d",*(int*)tmp->value);
		else sprintf(strMessage,"\t[NOT FOUND] %08x",tmp);
		
		stop=clock();
		
		AddToMessageBoxEx(strMessage,&mainwin);
	
		
		
		sprintf(strMessage,"\t[PERF] %2.6f second",(double)(stop-start)/CLOCKS_PER_SEC);
		AddToMessageBoxEx(strMessage,&mainwin);
	}
	
	AddToMessageBoxEx("strike a key to end...",&mainwin);
	SetCursor(mainwin.FirstPrintableX,mainwin.Last);
	getch();
	
	
	lc_empty(pTestPerf);
	free(Hashes);
	free(ideez);
	pthread_mutex_destroy(&mtxprod);
	
	return(EXIT_SUCCESS);
}

// ****************************************************************************
// SECTION : implémentation des fonctions
// ****************************************************************************

void fillelems(void *param)
{
	int localcpt=0;
	int *pThreadNumber=calloc(1,sizeof(int));
	*pThreadNumber=*(int*)param;
	
	pthread_mutex_lock(&mtxprod); 
	localcpt=5000*(*pThreadNumber);
	pthread_mutex_unlock(&mtxprod);
	
	
	srand(time(NULL));
	
	
	while(localcpt<5000*(*pThreadNumber)+5000)
	{
		pthread_mutex_lock(&mtxprod);
		sprintf(strMessage,"[%02d] %06d",*pThreadNumber,localcpt);
		AfficherXY(strMessage,largeur-12,mainwin.FirstPrintableY+(*pThreadNumber));
		sprintf(strMessage,"[%02d] %06d",*pThreadNumber,pTestPerf->NbElem);
		AfficherXY(strMessage,largeur-12,30+(*pThreadNumber));
		
		Hashes[localcpt]=rand()%INTMAX_MAX-1;
		ideez[localcpt]=lc_add(&Hashes[localcpt],pTestPerf,cssmint,sizeof(int));
		localcpt++;  
		pthread_mutex_unlock(&mtxprod);
		
		
	}
	
	pthread_exit(NULL);
}


