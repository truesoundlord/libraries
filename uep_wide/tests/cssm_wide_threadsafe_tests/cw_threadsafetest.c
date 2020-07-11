/* 
 * File:   cw_threadsafetest.c
 * Author: Dimitri "Hurukan" <soundlord@gmail.com>
 *
 * Créé le July 3, 2020, 10:28 PM
 */

// ****************************************************************************
// HISTORIQUE
// ****************************************************************************

// **************************************************************************** 
// Fichiers de définitions
// **************************************************************************** 

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <wchar.h>
#include <unistd.h>
#include <uep_wide.h>

// **************************************************************************** 
// Déclaration des constantes symboliques
// **************************************************************************** 

#define MAXTHREADS 30


// **************************************************************************** 
// Déclaration des variables globales, externes, ...
// **************************************************************************** 

unsigned int uiWidth,uiHeight;
PositionCadre Fenetre;

pthread_t LesThreads[MAXTHREADS];
pthread_t LandingZone;

char *pMsg;
void *returned;
bool bRunning=true;
bool bStart=false;

long start;
long stop;

_Atomic int cpt=0;

pthread_mutex_t mtxdisplay=PTHREAD_MUTEX_INITIALIZER;


// ****************************************************************************
// SECTION : prototypes des fonctions en test pour CE code source
// ****************************************************************************

void Thread(void*);
void Thread_LZ(void*);

/*
 * FONCTION PRINCIPALE
 */
int main(int argc,char** argv)
{
	InitCSSMWIDE("fr_BE.UTF8");
	GetConsoleDimensions(&uiWidth,&uiHeight);
	
	if(uiWidth>5) pMsg=calloc(uiWidth-2,sizeof(char));
	else return(EXIT_FAILURE);
	
	Fenetre=DrawBoxWithRGB(1,1,uiHeight-3,uiWidth,getRGB(0xff,0xff,0x00,true));
	SetCursor(Fenetre.FirstPrintableX,Fenetre.Last);
	strcpy(pMsg,"Appuyez une touche...");
	wprintf(L"%s",pMsg);
	getch();
	
	
	pthread_create(&LandingZone,NULL,(void*)&Thread_LZ,NULL);
	
	while(cpt<MAXTHREADS)
	{
		pthread_create(&LesThreads[cpt],NULL,(void*)&Thread,&cpt);
		sleep(1);
		cpt++;
	}
	cpt=0;
	bStart=true;
	while(cpt<MAXTHREADS)
	{
		pthread_mutex_trylock(&mtxdisplay);
		sprintf(pMsg,"Waiting for thread %d to stop...",cpt);
		AddToMessageBoxEx(pMsg,&Fenetre);
		pthread_mutex_unlock(&mtxdisplay);
		
		pthread_join(LesThreads[cpt],&returned);
		
		pthread_mutex_trylock(&mtxdisplay);
		int *tmp=(int*)returned;
		sprintf(pMsg,"[main] %s THREAD %d finished !!\x1b[0m",getRGBString(getRGB(255,0,0,true)),*tmp);
		AddToMessageBoxEx(pMsg,&Fenetre);
		pthread_mutex_unlock(&mtxdisplay);
		
		cpt++;
	}
	bRunning=false;
	SetCursor(Fenetre.FirstPrintableX,Fenetre.Last);
	
	free(pMsg);
	pthread_mutex_destroy(&mtxdisplay);
	
	
	
		
	return(EXIT_SUCCESS);
}

// ****************************************************************************
// SECTION : implémentation des fonctions
// ****************************************************************************

void Thread(void *param)
{
	int *pThreadNumber=calloc(1,sizeof(int));
	*pThreadNumber=*(int*)param;
	
	int internattiles=0;
	
	//sprintf(pMsg,"[THREAD %02d] ACTIVE",*pThreadNumber);
	//AfficherXY(pMsg,uiWidth-30,2+*pThreadNumber);
	
	char *internal=calloc(uiWidth,sizeof(char));
	struct timespec realtime={0L,300000000L};
	
	struct s_RGB threadcolor;
	
	
	/*if(*pThreadNumber%2==0) threadcolor=getRGB(64+(2*(*pThreadNumber)),196+(4*(*pThreadNumber)),64+(32*(*pThreadNumber)),true);
	if(*pThreadNumber%3==0) threadcolor=getRGB(64+(3*(*pThreadNumber)),196+(4*(*pThreadNumber)),64+(16*(*pThreadNumber)),true);
	if(*pThreadNumber%5==0) threadcolor=getRGB(64+(5*(*pThreadNumber)),196+(4*(*pThreadNumber)),64+(32*(*pThreadNumber)),true);
	if(*pThreadNumber%7==0) threadcolor=getRGB(64+(7*(*pThreadNumber)),196+(4*(*pThreadNumber)),64+(32*(*pThreadNumber)),true);
	else threadcolor=getRGB(64+(32*(*pThreadNumber)),128+(8*(*pThreadNumber)),128+(16*(*pThreadNumber)),true);*/
	
	srand(time(NULL));
	
	threadcolor=getRGB(rand()%255+100,rand()%255+96,rand()%255+92,true);
	
	while(!bStart) sleep(1);
	while(1)
	{
		pthread_mutex_trylock(&mtxdisplay);
		sprintf(internal,"%s[THREAD %02d] %-9s %03d\x1b[0m",getRGBString(threadcolor),*pThreadNumber,"ACTIVE",internattiles++);
		AfficherXY(internal,uiWidth-30,2+*pThreadNumber);
		SetCursor(uiWidth-30,2+*pThreadNumber);
			
		int hazardx=rand()%(uiWidth-68)+32;
		int hazardy=rand()%(uiHeight-3)+2;
		
		sprintf(internal,"%s%lc\x1b[0m",getRGBString(threadcolor),0x2587);
		AfficherXY(internal,hazardx,hazardy);
		
		sprintf(internal,"%s[THREAD %02d] (%03d,%03d)\x1b[0m",getRGBString(threadcolor),*pThreadNumber,hazardx,hazardy);
		AfficherXY(internal,uiWidth-30,uiHeight-(MAXTHREADS-*pThreadNumber)-1);
		SetCursor(uiWidth-30,22+*pThreadNumber);
		pthread_mutex_unlock(&mtxdisplay);
		
		if(hazardx==32) break;
		if(hazardy==32) break;
		fflush(stdout);		
		nanosleep(&realtime,NULL);
	}
	sleep(2);
	pthread_mutex_trylock(&mtxdisplay);
	sprintf(internal,"%s[THREAD %02d] %-9s\x1b[0m",getRGBString(threadcolor),*pThreadNumber,"DEAD :{");
	AfficherXY(internal,uiWidth-30,2+*pThreadNumber);
	sprintf(internal,"%s[THREAD %02d] (---,---) \x1b[0m",getRGBString(threadcolor),*pThreadNumber);
	AfficherXY(internal,uiWidth-30,uiHeight-(MAXTHREADS-*pThreadNumber)-1);
	pthread_mutex_unlock(&mtxdisplay);
		
	free(internal);
	pthread_mutex_destroy(&mtxdisplay);
	pthread_exit((void*)pThreadNumber);
}

void Thread_LZ(void *param)
{
	struct timespec realtime={0L,20000000L};
	while(bRunning)
	{
		pthread_mutex_lock(&mtxdisplay);
		SetCursor(Fenetre.FirstPrintableX,Fenetre.Last);
		wprintf(L"%*s",Fenetre.Width,"En cours d'exécution...");
		fflush(stdout);
		pthread_mutex_unlock(&mtxdisplay);
		nanosleep(&realtime,NULL);
		//usleep(20);
	}
	
	pthread_exit(NULL);
}
