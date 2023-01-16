/*
 * Dimitri "hurukan" 2010-2019 <soundlord@gmail.com> Ubuntu Edutainment Project
 */

// This work is licensed under a Creative Commons Attribution-NonCommercial 4.0 International License.

/*
 * http://creativecommons.org/licenses/by-nc/4.0
 */

#include <stdio.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <wchar.h>

#if (_WIN64 || _WIN32)
#include <conio.h>
#else
#define LINUX
#include <signal.h>
#include <unistd.h>
#include <termios.h>

#include <sys/ioctl.h>
#include <sys/stat.h>
#endif

#include "uep_wide.h"

// C'est là que l'orienté objet aurait été efficace... 

#ifndef __ANIMATEDTEXT__
#include <pthread.h>
pthread_mutex_t fctcore_mtx_protection=PTHREAD_MUTEX_INITIALIZER;                                                                                               /* mutex interne */
static bool	fctcore_brunning=true;																																																															/* flag interne */
#define __ANIMATEDTEXT__
#endif

//*****************************************************************************
// HISTORIQUE DES MISES A JOUR
// Mars 2019: DrawBoxWithRGB()
//						DrawBox()
//						DrawTitledBox()
//						DrawTitledBoxWithRGB()
//
// Avril et Mai 2019:	AddToMessageBoxEx()
//										fct_core()
//
// Juillet 2020: 	modifications du fichier .h pour les contours des cadres
// 								tentative de vérification au niveau des fonctions censées être "threadsafe"
//
//*****************************************************************************

//#ifndef __DEBUG__
//#define __DEBUG__
//static FILE	*Debug;
//#endif

//*****************************************************************************
// pMsg: chaîne de caractères
// posX: position en X (colonne)
// posY: position en Y (ligne)
//*****************************************************************************
void AfficherXY(char *pMsg,int posX,int posY)
{
	int 		taillereelle=0;
	
#ifdef __DEBUG__
	wchar_t	strDebug[taillereelle+1];
	fflush(stderr);
	if(Debug==NULL)
	{
		Debug=freopen("./logs/AfficherXY.log","a+",stderr);
	}

	// 20 novembre 2017
	
	fwprintf(stderr,L"[DEBUG] %08x (%d)\n",pMsg,taillereelle+1);	
#endif
	
	if(pMsg==NULL || strlen(pMsg)==0) return;
	
	taillereelle=wstrlen(pMsg);
	
	wchar_t tmpMsg[taillereelle+1];											// gdb se plante ici je sais pas pourquoi (17 nov 2017)
	//wchar_t *tmpMsg=(wchar_t*)malloc(sizeof(wchar_t)*(taillereelle+1));
		
	wmemset(tmpMsg,0,taillereelle+1);
	mbstowcs(tmpMsg,pMsg,taillereelle+2);  // juillet 2020
	tmpMsg[taillereelle+1]=L'\0';																									// mars 2015
	
#ifdef __DEBUG__
	fwprintf(stderr,L"[DEBUG][%-*ls]\n",taillereelle+1,tmpMsg);
#endif
	
	wprintf(L"\x1b[s");							/* Sauve la position courante du curseur */
	wprintf(L"\x1b[%d;%dH%ls",posY,posX,tmpMsg);
	wprintf(L"\x1b[u");							/* restaure la position sauvegardée */
	fflush(stdout);
}
//*****************************************************************************
// BackToShell
//
// Entrées:
//	posx: position en X
//	posy: position en Y
//*****************************************************************************
void BackToShell(short posx,short posy)
{
	wprintf(L"\x1b[%d;%dH\x1b[K\x1b[1A\n",posy+3,posx);
	exit(0);
}

//*****************************************************************************
// posX: position en X (colonne)
// posY: position en Y (ligne)
// height: nombre de lignes composant la boite
// length: nombre de colonnes composant la boite
// color: code couleur au format ANSI
//*****************************************************************************
struct FakeWindowPos DrawBox(int posX,int posY,int height,int length,int color)
{
	int origineX=posX,origineY=posY;											// nous sauvegardons les positions d'origine vu que nous allons modifier posX et posY...
	
	wprintf(L"\x1b[%d;%dH",posY,posX);
	wprintf(L"\x1b[%dm%lc",color,UTF8_COINSUPGAUCHE);
	for(posX+=1;posX<=origineX+length;posX++)
	{
		wprintf(L"\x1b[%d;%dH",posY,posX);
		wprintf(L"\x1b[%dm%lc",color,UTF8_LIGNE);
	}
	wprintf(L"\x1b[%dm%lc",color,UTF8_COINSUPDROIT);
	posY++;
	posX=origineX;
	for(;posY<=origineY+height;posY++)
	{
		wprintf(L"\x1b[%d;%dH",posY,posX);
		wprintf(L"\x1b[%dm%lc",color,UTF8_LIGNEVERT);
		for(posX+=1;posX<=origineX+length;posX++)
		{
			wprintf(L"\x1b[%d;%dH",posY,posX);
			wprintf(L" ");
		}
		wprintf(L"\x1b[%dm%lc",color,UTF8_LIGNEVERT);
		posX=origineX;
	}
	wprintf(L"\x1b[%d;%dH",posY,posX);
	wprintf(L"\x1b[%dm%lc",color,UTF8_COININFGAUCHE);
	for(posX+=1;posX<=origineX+length;posX++)
	{
		wprintf(L"\x1b[%d;%dH",posY,posX);
		wprintf(L"\x1b[%dm%lc",color,UTF8_LIGNE);
	}
	wprintf(L"\x1b[%dm%lc",color,UTF8_COININFDROIT);
	wprintf(L"\x1b[0m");
	fflush(stdout);
	return (struct FakeWindowPos){origineX,origineY,length,height,origineX+1,origineY+1,origineY+height+2};
}

//*****************************************************************************
// posX: position en X (colonne)
// posY: position en Y (ligne)
// height: nombre de lignes composant la boite
// length: nombre de colonnes composant la boite
// color: code couleur au format s_RGB
//*****************************************************************************
struct FakeWindowPos DrawBoxWithRGB(int posX,int posY,int height,int length,struct s_RGB color)
{
	int origineX=posX,origineY=posY;											// nous sauvegardons les positions d'origine vu que nous allons modifier posX et posY...
	
	//length-=1;
	//height-=3; (mars 2019: super bug en pleine classe, honte totale :{)
	//C'est la seule fonction que je n'avais pas testé dans la nouvelle moulûre de la librairie :{
	if(height<0) height=1;
	
	char *pColorBoxString;
	pColorBoxString=getRGBString(color);
		
	wprintf(L"\x1b[%d;%dH",posY,posX);
	wprintf(L"%s%lc",pColorBoxString,UTF8_COINSUPGAUCHE);
	for(posX+=1;posX<=origineX+length;posX++)
	{
		wprintf(L"\x1b[%d;%dH",posY,posX);
		wprintf(L"%s%lc",pColorBoxString,UTF8_LIGNE);
	}
	wprintf(L"%s%lc",pColorBoxString,UTF8_COINSUPDROIT);
	posY++;
	posX=origineX;
	
	for(;posY<=origineY+height;posY++)
	{
		wprintf(L"\x1b[%d;%dH",posY,posX);
		wprintf(L"%s%lc",pColorBoxString,UTF8_LIGNEVERT);
		for(posX+=1;posX<=origineX+length;posX++)
		{
			wprintf(L"\x1b[%d;%dH",posY,posX);
			wprintf(L" ");
		}
		wprintf(L"%s%lc",pColorBoxString,UTF8_LIGNEVERT);
		posX=origineX;
	}
	wprintf(L"\x1b[%d;%dH",posY,posX);
	wprintf(L"%s%lc",pColorBoxString,UTF8_COININFGAUCHE);
	for(posX+=1;posX<=origineX+length;posX++)
	{
		wprintf(L"\x1b[%d;%dH",posY,posX);
		wprintf(L"%s%lc",pColorBoxString,UTF8_LIGNE);
	}
	wprintf(L"%s%lc",pColorBoxString,UTF8_COININFDROIT);
	wprintf(L"\x1b[0m");
	fflush(stdout);
	free(pColorBoxString); // 2021 (memory leak)
	return (struct FakeWindowPos){origineX,origineY,length,height,origineX+1,origineY+1,origineY+height+2};
}

//*****************************************************************************
// posX: position en X (colonne)
// posY: position en Y (ligne)
// height: nombre de lignes composant la boite
// length: nombre de colonnes composant la boite
// color: code couleur au format ANSI
// pTitle: titre du cadre  
//*****************************************************************************
struct FakeWindowPos DrawTitledBox(int posX,int posY,int height,int length,int color,char *pTitle)
{
	int origineX=posX,origineY=posY;											// nous sauvegardons les positions d'origine vu que nous allons modifier posX et posY...
	
	if(length<strlen(pTitle)) length=strlen(pTitle)+2;
	
	wchar_t tmpTitle[strlen(pTitle)+1];
	mbstowcs(tmpTitle,pTitle,strlen(pTitle)+1);
		
	wprintf(L"\x1b[s");							/* Sauve la position courante du curseur */
	wprintf(L"\x1b[%d;%dH",posY,posX);
	wprintf(L"\x1b[%dm%lc",color,UTF8_COINSUPGAUCHE);
	wprintf(L"\x1b[%dm%lc",color,UTF8_LIGNE);
	wprintf(L"\x1b[%dm%ls",color,tmpTitle);
	
	posX+=wcslen(tmpTitle)+2;														// nous devons déplacer le curseur de deux caractères puis la taille du titre...
	for(;posX<=origineX+length;posX++)
	{
		wprintf(L"\x1b[%d;%dH",posY,posX);
		wprintf(L"\x1b[%dm%lc",color,UTF8_LIGNE);
	}
	wprintf(L"\x1b[%dm%lc",color,UTF8_COINSUPDROIT);
	posY++;
	posX=origineX;
	for(;posY<=origineY+height;posY++)
	{
		wprintf(L"\x1b[%d;%dH",posY,posX);
		wprintf(L"\x1b[%dm%lc",color,UTF8_LIGNEVERT);
		for(posX+=1;posX<=origineX+length;posX++)
		{
			wprintf(L"\x1b[%d;%dH",posY,posX);
			wprintf(L" ");
		}
		wprintf(L"\x1b[%dm%lc",color,UTF8_LIGNEVERT);
		posX=origineX;
	}
	wprintf(L"\x1b[%d;%dH",posY,posX);
	wprintf(L"\x1b[%dm%lc",color,UTF8_COININFGAUCHE);
	for(posX+=1;posX<=origineX+length;posX++)
	{
		wprintf(L"\x1b[%d;%dH",posY,posX);
		wprintf(L"\x1b[%dm%lc",color,UTF8_LIGNE);
	}
	wprintf(L"\x1b[%dm%lc",color,UTF8_COININFDROIT);
	wprintf(L"\x1b[0m");
	wprintf(L"\x1b[u");							/* restaure la position sauvegardée */
	fflush(stdout);
	return (struct FakeWindowPos){origineX,origineY,length,height,origineX+1,origineY+1,origineY+height+2};
}

//*****************************************************************************
// posX: position en X (colonne)
// posY: position en Y (ligne)
// height: nombre de lignes composant la boite
// length: nombre de colonnes composant la boite
// color: code couleur au format s_RGB
// pTitle: titre du cadre (ne doit pas contenir des caractères "wide") 
//*****************************************************************************
struct FakeWindowPos DrawTitledBoxWithRGB(int posX,int posY,int height,int length,struct s_RGB color,char *pTitle,struct s_RGB titlecolor,short design)
{
	int origineX=posX,origineY=posY;											// nous sauvegardons les positions d'origine vu que nous allons modifier posX et posY...
	struct s_RGB DefaultColor={240,240,240,true};
	wchar_t tmpTitle[strlen(pTitle)+1];
	char *pColorBoxString;
	char *pColorTitleString;
	
	//length-=2;
	
	if(length<strlen(pTitle)) length=strlen(pTitle)+2;
	
	mbstowcs(tmpTitle,pTitle,strlen(pTitle)+1);
	switch(design)
	{
		case COLORED_BOX:
				pColorBoxString=getRGBString(color);
				pColorTitleString=getRGBString(DefaultColor);
				break;
		case COLORED_TITLE:
				pColorBoxString=getRGBString(DefaultColor);
				pColorTitleString=getRGBString(titlecolor);
				break;
		case COLORED_BOTH:
				pColorBoxString=getRGBString(color);
				pColorTitleString=getRGBString(color);
				break;
		case COLORED_SEPARATE:
				pColorBoxString=getRGBString(color);
				pColorTitleString=getRGBString(titlecolor);
				break;
		default:
				pColorBoxString=getRGBString(DefaultColor);
				pColorTitleString=getRGBString(DefaultColor);
	}
	
	wprintf(L"\x1b[s");							/* Sauve la position courante du curseur */
	wprintf(L"\x1b[%d;%dH",posY,posX);
	wprintf(L"%s%lc",pColorBoxString,UTF8_COINSUPGAUCHE);
	wprintf(L"%s%lc",pColorBoxString,UTF8_LIGNE);
	wprintf(L"%s%ls\x1b[0m",pColorTitleString,tmpTitle);
	
	posX+=wcslen(tmpTitle)+2;														// nous devons déplacer le curseur de deux caractères puis la taille du titre...
	for(;posX<=origineX+length;posX++)
	{
		wprintf(L"\x1b[%d;%dH",posY,posX);
		wprintf(L"%s%lc",pColorBoxString,UTF8_LIGNE);
	}
	wprintf(L"%s%lc",pColorBoxString,UTF8_COINSUPDROIT);
	posY++;
	posX=origineX;
	for(;posY<=origineY+height;posY++)
	{
		wprintf(L"\x1b[%d;%dH",posY,posX);
		wprintf(L"%s%lc",pColorBoxString,UTF8_LIGNEVERT);
		for(posX+=1;posX<=origineX+length;posX++)
		{
			wprintf(L"\x1b[%d;%dH",posY,posX);
			wprintf(L" ");
		}
		wprintf(L"%s%lc",pColorBoxString,UTF8_LIGNEVERT);
		posX=origineX;
	}
	wprintf(L"\x1b[%d;%dH",posY,posX);
	wprintf(L"%s%lc",pColorBoxString,UTF8_COININFGAUCHE);
	for(posX+=1;posX<=origineX+length;posX++)
	{
		wprintf(L"\x1b[%d;%dH",posY,posX);
		wprintf(L"%s%lc",pColorBoxString,UTF8_LIGNE);
	}
	wprintf(L"%s%lc",pColorBoxString,UTF8_COININFDROIT);
	wprintf(L"\x1b[0m");
	wprintf(L"\x1b[u");							/* restaure la position sauvegardée */
	fflush(stdout);
	free(pColorBoxString);				// 2021 
	free(pColorTitleString);			// 2021 (memory leak)
	return (struct FakeWindowPos){origineX,origineY,length,height,origineX+1,origineY+1,origineY+height+2};
}

/*****************************************************************************
* Fonction getRGBString() -- permet de récupérer la séquence ANSI pour déterminer
* un code couleur au format RGB (foreground).
******************************************************************************/
char* getRGBString(struct s_RGB target)
{
	char *pTmp=(char*)calloc(25,sizeof(char)); // \x1b[38;2;xxx;xxx;xxxm 
	sprintf(pTmp,"\x1b[38;2;%d;%d;%dm",target.Red,target.Green,target.Blue);
	return pTmp;
}
/*****************************************************************************
* Fonction getReverseRGBString() -- permet de récupérer la séquence ANSI pour déterminer 
* un code couleur au format RGB mais cette fois en mode INVERSE (background)
*****************************************************************************/
char* getReverseRGBString(struct s_RGB target)
{
	char *pTmp=(char*)calloc(25,sizeof(char)); // \x1b[48;2;xxx;xxx;xxxm 
	sprintf(pTmp,"\x1b[48;2;%d;%d;%dm",target.Red,target.Green,target.Blue);
	return pTmp;
}
/*****************************************************************************
* Fonction getRGB() -- permet de récupérer une structure de type s_RGB à partir
* de données de type (R,G,B,flag gras).
*
* Red, Green, Blue: valeur entre 0 et 255
* bold: 
*      false:  normal
*      true:   gras
******************************************************************************/
struct s_RGB getRGB(short Red,short Green,short Blue,bool bold)
{
	struct s_RGB tmp={Red,Green,Blue,bold?1:0,0};
	return tmp;
}

/**************************************************************************************
* pMsg: Le message qui doit être affiché
* posX: coordonnées en X
* posY: coordonnées en Y
* widthmax: longueur exprimée en nombre de colonnes sur laquelle se baser pour aligner
* align: type de l'alignement (0: à gauche, 1: centre, 2: à droite)
*
* output:
*        -1: la taille du message est plus grande que la taille maximum. 
**************************************************************************************/

int DisplayAligned(char *pMsg,int posX,int posY,int widthmax,int align)
{
	// novembre 2017: bug
	// Il faudrait pouvoir insérer des codes ANSI (pour changer les couleurs, etc...) et 
	// faire en sorte qu'ils ne soient pas pris en compte dans le comptage du nombre
	// de caractères.
	
	int	taillereelle=0;
	// widthmax++;
	
	char *pPrefix=NULL;						// ...en général nous devrions trouver dans la chaîne une séquence ANSI définissant la couleur et le mode (gras)
	char *pPrefix_end=NULL;						
	char *pSuffix=NULL;						// ...suivi d'une séquence ANSI remettant les paramètres à zéro
	char *pSuffix_end=NULL;
	
	// ...il nous faut détecter les deux sans altérer le message en lui même et déterminer le nombre de caractères réels 
	// ...afin de pouvoir centrer le message
	
	pPrefix=strstr(pMsg,"\x1b[");							// y a-t-il une séquence ANSI qui débute dans la chaîne ?
	if(pPrefix!=NULL)												
	{
		pPrefix_end=strchr(pPrefix,'m');				// ...dans la majorité des cas la séquence ANSI se termine par un 'm'...
		if(pPrefix_end!=NULL) 
		{
			// ...il faut calculer la place que prend la séquence dans la chaîne de caractère
			pPrefix_end++;												// ...il faut passer au caractère suivant le 'm' détecté... sinon il sera comptabilisé
			taillereelle=pPrefix_end-pPrefix;
			
			pSuffix=strstr(pPrefix_end,"\x1b[");	// y a-t-il une séquence ANSI qui termine la chaîne ?
			if(pSuffix!=NULL)												
			{
				pSuffix_end=strchr(pSuffix,'m');		// ...dans la majorité des cas la séquence ANSI se termine par un 'm'...
				if(pSuffix_end!=NULL) 
				{
					// ...il faut calculer la place que prend la séquence dans la chaîne de caractère
					pSuffix_end++;
					taillereelle+=pSuffix_end-pSuffix;
				}
			}
		}
	}
	
	// Ici il est hors de question de toucher à la taille de la chaîne...
	
	wchar_t tmpMsg[strlen(pMsg)+1];
	mbstowcs(tmpMsg,pMsg,strlen(pMsg)+1);
		
	int longueurMessage=wcslen(tmpMsg)-taillereelle;								// la longueur du message moins les caractères issus des séquences ANSI
	if(longueurMessage-1>widthmax)
	{
     return -1; // pour le moment... (le but étant alors de faire un appel à DisplayAnimatedText ^^)
	}
	
	if(longueurMessage%2) longueurMessage--;	// Novembre 2017: "amélioration" en fonction de la taille (en fonction de la parité)
	else longueurMessage++;

//#define __DEBUG__	
#ifdef __DEBUG__
	int cpy=widthmax;
#endif
	switch(align)
	{
		case ALIGN_LEFT: AfficherXY(pMsg,posX+1,posY);
				break;
		case ALIGN_CENTER:
#ifdef __DEBUG__
				taillereelle=0;
				SetCursor(posX+taillereelle,posY);
				for(;taillereelle<=cpy;taillereelle++) 
				{
					if(taillereelle>9)
					{
						cpy-=10;
						taillereelle=0;
						wprintf(L"0");
					}
					else wprintf(L"%d",taillereelle);
				}
#endif
				AfficherXY(pMsg,posX+((widthmax/2-longueurMessage/2))+1,posY);
				break;
		case ALIGN_RIGHT: 
				AfficherXY(pMsg,posX+(widthmax-longueurMessage)+1,posY);
	}
  return 0;
}
//*****************************************************************************
// SAVEZ PAS LIRE ????
//*****************************************************************************
void EffacerEcran()
{
	wprintf(L"\x1b[2J");
	wprintf(L"\x1b[1;1H");						// Positionne le curseur sur le coin supérieur gauche...
}

/*
 * Fonction qui permet de récupérer un caractère en provenance du clavier sans afficher celui-ci et sans devoir appuyer sur entrée
/* Dans le mode terminal, en général, une "entrée clavier" est terminée quand on a appuyé sur "entrée" (return)
/* or, ici, nous voudrions récupérer une "entrée clavier" (un caractère) sans devoir appuyer sur entrée... */

#ifdef LINUX
int getch() 
{
  struct termios oldt, newt;
	struct sigaction Detournement={{0}};
		
	Detournement.sa_handler=SIG_IGN;
	// On ne peut pas interrompre le getch...
	sigaction(SIGINT,&Detournement,NULL);
		
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
		newt.c_lflag &= ~(ICANON | ECHO ); // works !!
    //newt.c_lflag &= ~(ICANON | ECHO | NOFLSH); // 22 aug 2014
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

		Detournement.sa_handler=SIG_DFL;
		Detournement.sa_flags=0;
		sigaction(SIGINT,&Detournement,NULL);

    return ch;
}
#endif
//*****************************************************************************
// Fonction AddToMessageBox()
//
// Voir détails dans AddToMessageBoxEx() sans la structure de retour.
//
// avril 2019
// OBSOLETE, certains programmes qui l'utilisaient continueront à tourner mais
// il est conseillé d'utiliser AddToMessageBoxEx().
//*****************************************************************************

void AddToMessageBox(char *pMessage,int posx,int maxX,int maxY,int basevert,unsigned short *pCurrentNumber)
{
	char *blankline=(char*)malloc(maxX);
	wchar_t tmpString[maxX];
	int cpt;
	
	if(*pCurrentNumber>maxY || *pCurrentNumber<0) return;
	
	memset((void*)blankline,32,maxX);
	wmemset(tmpString,0,maxX);
	
	tmpString[maxX]=L'\0';																												// mars 2015
	
	if(maxX<4) return; // on affiche rien pour être sûr... 

	(*pCurrentNumber)++;
	int taillereelle=wstrlen(pMessage);
	
	if(taillereelle>maxX)
	{
		mbstowcs(tmpString,pMessage,maxX-4);
		wcscat(tmpString,L"...\0");
	}
	else
	{
		mbstowcs(tmpString,pMessage,taillereelle);
	}
	if((*pCurrentNumber)>=maxY+1)
	{
		//sleep(1);																										// histoire de pouvoir lire les infos avant de les effacer...
		for(cpt=1;cpt<(*pCurrentNumber);cpt++)
			AfficherXY(blankline,posx,basevert+cpt);
		(*pCurrentNumber)=1;
	}
	wprintf(L"\x1b[%d;%dH%ls",basevert+(*pCurrentNumber),posx,tmpString);
	fflush(stdout);
	free(blankline);																								// pour ne pas charger la mémoire...
}

//*****************************************************************************
// AddToMessageBoxEx (Extention de AddToMessageBox)
//
// pMessage: 		Message sous forme de pointeur sur "char"
// datas: 			contient les informations relatives à la "boîte" dans laquelle 
//					le message doit être ajouté.
// pCurrentNumber:	pointeur sur un entier non signé contenant le nombre de messages
//					contenus dans la boîte après l'ajoût de celui-ci.
//
// Remarques: 	si la boite fait moins de 5 colonnes le comportement n'est plus garanti.
//			 				les messages de forme "AddToMessageBox("Message",datas,&nbreElem);"
//							peuvent poser de gros soucis... (TODO: faire mieux). 
//				
//*****************************************************************************

void AddToMessageBoxEx(char *pMessage,struct FakeWindowPos *datas)
{
	char *blankline=(char*)malloc(datas->Width+1);
	wchar_t tmpString[datas->Width];
	int cpt;
			
	if(datas->nbMessages>datas->Height) return;
	
	if(datas->Width<4) return; // on affiche rien pour être sûr... 
	
	int posY=datas->FirstPrintableY-1; // pour éviter une ligne blanche dans les fenêtres...	
	
	memset((void*)blankline,32,datas->Width);
  blankline[datas->Width]='\0';
	wmemset(tmpString,0,datas->Width);
	tmpString[datas->Width]=L'\0';																									// mars 2015
  
	datas->nbMessages++;
	int taillereelle=wstrlen(pMessage);
	
	// L'ANSI fait bugger l'affichage...
	
	if(taillereelle>datas->Width)
	{
		if((strchr(pMessage,27))!=0) 
		{
			mbstowcs(tmpString,pMessage,datas->Width);
		}
		else
		{
			mbstowcs(tmpString,pMessage,datas->Width-4);
		}
		wcscat(tmpString,L"...\0");
	}
	else
	{
		mbstowcs(tmpString,pMessage,taillereelle);
	}
	
	if(datas->nbMessages==datas->Height+1)
	{
		for(cpt=1;cpt<datas->nbMessages;cpt++)
			AfficherXY(blankline,datas->FirstPrintableX,posY+cpt);
		datas->nbMessages=1;
	}
	wprintf(L"\x1b[%d;%dH%ls",posY+datas->nbMessages,datas->FirstPrintableX,tmpString);
	fflush(stdout);
	free(blankline);
}

//*****************************************************************************
// ResetBox() juillet 202
//*****************************************************************************

void ResetBox(struct FakeWindowPos *datas)
{
	if(datas==NULL) return;
	
	int cpt;
	datas->nbMessages=0; // pour pas effacer le cadre du dessus
	char *blankline=(char*)malloc(datas->Width+1);
	
	int posY=datas->FirstPrintableY; // pour éviter une ligne blanche dans les fenêtres...	
	
	memset((void*)blankline,32,datas->Width);
	for(cpt=0;cpt<datas->Height;cpt++)
		AfficherXY(blankline,datas->FirstPrintableX,posY+cpt);
	
	//datas->nbMessages=1;
	fflush(stdout);
	free(blankline);
}



//*****************************************************************************
// Fonction getchAt() -- permet de positionner le curseur avant de faire un getch()
//*****************************************************************************

int getchAt(int posX,int posY)
{
	wprintf(L"\x1b[%d;%dH",posY,posX);
	wprintf(L"\x1b[s");							/* Sauve la position courante du curseur */
	return getch();
}

// *****************************************************************************************************************************************************************
// Fonction qui permet de splitter la date et d'en extraire l'heure
// 
// Entrée: chaine de caractère contenant une date au format dow moy jj hh:mm:ss aaaa (dow: jour de la semaine sur 3 car., moy: mois de l'année sur 3 car.,)
// Sortie: chaine de caractère contenant l'heure au format hh:mm:ss
// *****************************************************************************************************************************************************************

char* splitDateToTime(char *pCompleteDateString)
{
	// Le format est le suivant "Sat Apr 27 21:00:23 2013"
	// Ce qui nous intéresse c'est les 8 caractères contenant l'heure
	
	char *pCurseur;
	char *Heure=malloc(sizeof(char)*9);
	
	memset(Heure,0,9);
	pCurseur=pCompleteDateString;								// pCurseur pointe sur le premier caractère de pCompleteDateString...
	pCurseur+=(strlen(pCompleteDateString)-14); // 8 caractères+un espace+4 caractères de l'année et le backslash zéro... (on se déplace <-> offset)
	
	strncpy(Heure,pCurseur,8);	// on copie dans Heure à partir de pCurseur les 8 premiers caractères (c'est à dire l'heure au format hh:mm:ss)
	return Heure;
}

// *****************************************************************************************************************************************************************
// Fonction qui permet de calculer le temps écoulé entre le début et la fin de la partie
//
// Entrées:
//
//	pBegin: heure de début au format hh:mm:ss
//	pEnd: heure de fin au format hh:mm:ss
//
// Sortie: les heures, minutes, secondes écoulées dans une structure de type "chrono"
// *****************************************************************************************************************************************************************

chrono getTimeElapsed(char *pBegin,char *pEnd)
{
	chrono tmpTempsEcoule;
	char *SplittedBegin=malloc(strlen(pBegin));		// on alloue dynamiquement la taille pour les deux chaines de caractères... 
	char *SplittedEnd=malloc(strlen(pEnd));				// ...mais dans la réalité cette taille sera toujours la même... 
	char *pCurseur;																// "curseur" qui va nous permettre d'isoler les heures des minutes et des secondes...
	char DatasBegin[3][3]={"\0"};									// Tableaux nécessaires pour stocker heures, minutes et secondes...
	char DatasEnd[3][3]={"\0"};
	int cpt=0;																		
	
	memset(SplittedBegin,0,strlen(pBegin));				// On met les deux chaines à 0
	memset(SplittedEnd,0,strlen(pEnd));
	
	strcpy(SplittedBegin,splitDateToTime(pBegin));	// On copie ce que la fonction splitDateToTime a renvoyé comme valeur de retour
	strcpy(SplittedEnd,splitDateToTime(pEnd));
	
	// Exemple 23:12:56 
	
	pCurseur=SplittedBegin;													// On initialise le curseur au début de la chaine de caractères... --> 23:12:56
	while(pCurseur=strchr(pCurseur,':'))						// On cherche le caractère ":" --> :12:56 puis :56
	{
		strncpy(DatasBegin[cpt],SplittedBegin,2);			// ...si strchr retourne autre chose que NULL alors on copie deux caractères --> 23 puis 12
		pCurseur++;																		// ...on passe le caractère ":" --> 12:56 puis 56
		SplittedBegin=pCurseur;												// ...on change la valeur de SplittedBegin afin de ne pas copier tout le temps les deux premiers caractères de la chaine --> 12:56 puis 56
		cpt++;																	
	}
	strncpy(DatasBegin[cpt],SplittedBegin,2);				// ...on copie les deux derniers caractères (les secondes) --> 56
	cpt=0;
	
	pCurseur=SplittedEnd;													
	while(pCurseur=strchr(pCurseur,':'))
	{
		strncpy(DatasEnd[cpt],SplittedEnd,2);
		pCurseur++;
		SplittedEnd=pCurseur;
		cpt++;
	}
	strncpy(DatasEnd[cpt],SplittedEnd,2);
	
	tmpTempsEcoule.Heures=atoi(DatasEnd[0])-atoi(DatasBegin[0]); 
	tmpTempsEcoule.Minutes=atoi(DatasEnd[1])-atoi(DatasBegin[1]); 
	tmpTempsEcoule.Secondes=atoi(DatasEnd[2])-atoi(DatasBegin[2]);
	
	if(tmpTempsEcoule.Heures<0) 
	{
		tmpTempsEcoule.Heures=24-atoi(DatasBegin[0]); // cas de la partie commencée à 23:39:12 et terminée à 00:24:54 
	}
	if(tmpTempsEcoule.Minutes<0)
	{
		tmpTempsEcoule.Heures--;
		tmpTempsEcoule.Minutes=(60-atoi(DatasBegin[1]))+atoi(DatasEnd[1]); // cas de la partie commencée à 13:19:12 et terminée à 14:01:25 
	}
	if(tmpTempsEcoule.Secondes<0)
	{
		tmpTempsEcoule.Minutes--;
		tmpTempsEcoule.Secondes=(60-atoi(DatasBegin[2]))+atoi(DatasEnd[2]); // cas de la partie commencée à 09:16:12 et terminée à 09:39:02 
	}
	return tmpTempsEcoule;
}

//*****************************************************************************
//  Fonction getnchar() -- !!! HAUTE VOLTIGE !!!
//
// Permet de créer une chaîne de caractères sans devoir réserver l'espace
// mémoire au préalable.
//
// Entrée:
//
//  maximum: longueur maximum de la chaîne de caractères
//
//  Sortie:
//
//  char* : une chaîne de caractères (au format UTF8 !!) 
//*****************************************************************************
char *getnchar(int maximum)
{
	int 	offset=0;										// déplacement
	int		nbreCar=0;									// permet de s'assurer que le nombre de caractères est bien atteint
	int 	adaptedsize=maximum;				// la taille peut changer à cause des caractères UTF8
	wchar_t convertedCar;							// permet d'afficher les caractères UTF8 convenablement 
	char    toconvert[2];							// warning leap15 juillet 2018								
	char *pReturnPointer;							// pointeur à retourner à la fonction appelante
	char *pTmp;												// pointeur temporaire
		
	if(maximum>0)
	{
		pReturnPointer=(char*)calloc(maximum+1,sizeof(char));		
		pTmp=(char*)calloc(maximum+1,sizeof(char));				
		//memset((void*)pTmp,0,maximum);
		//memset((void*)pReturnPointer,0,maximum);    // corrections 28 mai 2018
	}
	else
		return NULL;
	
	while(nbreCar<maximum)
	{
		*pTmp=getch();								// c'est complexe je sais...
				
		if(*pTmp==10 || *pTmp==27) break;
		if(*pTmp>0) // Il ne s'agit pas de caractères UTF8
		{
			if(*pTmp!=127) 							// Si pas backspace (effacer)
			{
				wprintf(L"%c",*pTmp);			// sous Windows, getch() ne fait pas l'écho du caractère non plus...
				pTmp++;					
				offset++;	
				nbreCar++;								// incrémentation du compteur pour pouvoir sortir...
			}
			else // gestion du caractère BS (back space)
			{
				if(offset>0 && nbreCar>0)
				{
					wprintf(L"\x1b[1D"); 	// on retourne en arrière
					wprintf(L" ");				// on efface le caractère
					wprintf(L"\x1b[1D");	// on retourne en arrière
					pTmp--;								// idem pour le pointeur il faut revenir une position "en arrière"
					offset--;							// on se déplace d'une "case" à gauche en mémoire...
					nbreCar--;						// il ne faut pas compter cette entrée comme un caractère
				}
				if(*pTmp<0)							// si c'est un caractère UTF8 
				{
					pTmp--;								// il faut effacer le code -61 
					offset--;							// on se déplace d'une "case" à gauche en mémoire...
				}
			}
		}
		// Gestion des caractères UTF8
		else
		{
			if(maximum>1) // ce bloc sera exécuté deux fois (un caractère utf8 est codé sur 2 bytes)...
			{
				if(*pTmp!=-61 && *pTmp!=-62) nbreCar++;																	// on ne compte le caractère que si nous avons traité les deux caractères UTF8
				strncpy(toconvert,pTmp,2);																							// deux bytes à convertir en unicode
				int rc=mbtowc(&convertedCar,toconvert,1);
				if(convertedCar>0 && rc>0) 																							// on affiche uniquement lorsque le caractère est valide...
				{
					wprintf(L"%lc",convertedCar);
				}
				adaptedsize++;   
				
				// tentative de ré-allouer la zone mémoire pour s'adapter aux caractères entrés... (correction 28 mai 2018)
				// BUG: dans certains cas (surtout dans netbeans :{) quand nous plaçons plusieurs caractères UTF8 les uns après les autres 
				// nous "obtenons" un double free or corruption (fasttop)
				
				// C library thinks you did a double free (that is, you freed the same thing twice, which is of course a bug) or that you corrupted its data structures, 
				// such as by writing beyond the end of a buffer you allocated. 
				
				// valgrind indique des lectures, écritures et libérations (free/realloc/delete) dans realloc()...

#ifdef AVANT				
				pReturnPointer=(char*)realloc(pReturnPointer,sizeof(char)*adaptedsize);
#else
				//free(pReturnPointer);
				pReturnPointer=(char*)calloc(adaptedsize,sizeof(char)); 	// BUG résolu, maintenant je n'ai aucune explications concernant le fait que realloc() fonctionne
																																	// "quand elle en a envie" :{
#endif
				pReturnPointer=pTmp-offset;										// nous venons de changer la taille de la chaine il faut lui donner une valeur...
								
				offset++;
				pTmp++;
			}
			else
			{
				// Encodage... d'un seul caractère de type UTF8

				pReturnPointer=(char*)realloc(pReturnPointer,sizeof(char)*3);		// le code -61 (ou -62), le code du caractère et \0...
				pReturnPointer[0]=*pTmp;																				// indique qu'il s'agit d'UTF8 (pour l'affichage)
				pReturnPointer[1]=getch();																			// les caractères UTF8 ne sont pas complets jusqu'à ce que 2 bytes aient été "saisis" le getch() ici récupère la partie manquante...
				pReturnPointer[2]='\0';																					// on indique la fin de la chaine...
				
				// Affichage
				
				wchar_t todisplay;
				int rc=mbtowc(&todisplay,pReturnPointer,2);
				if(rc>0)
					wprintf(L"%lc",todisplay);
				return pReturnPointer;																					// inutile d'aller plus loin...
			}
		}
		
	} // endwhile
	*pTmp='\0'; 
	pReturnPointer=pTmp-offset;	// on retourne le début de phrase et non la position actuelle...
	return pReturnPointer;
}

// Les "wide characters" (caractères accentués) poseront toujours problème
// ils prennent deux bytes... du coup une chaine de 15 caractères avec deux caractères "wide"
// sera considérée comme une chaine de 17 caractères...

char *getncharAt(int posx,int posy,int maximum)
{
		wprintf(L"\x1b[%d;%dH",posy,posx);
		return getnchar(maximum);
}

// wstrlen
// wcslen ne s'applique qu'à une chaine dite "wide" (contenant des caractères unicode)

//******************************************************************
// wstrlen permet de calculer le nombre de caractères d'une chaine
// au contrario de wcslen, cette fonction ne tient pas compte
// du caractère additionnel nécessaire à la prise en charge des
// caractères accentués...
//
// Input: une chaine de caractères.
// Output: 	le nombre de caractères contenus au sens "digit" du 
// 					terme.
// Attention !! Cette fonction ne convient pas pour déterminer
// la place occupée en mémoire (là il vaut mieux utiliser strlen) !! 
// Elle ne sert qu'à savoir le nombre de caractères composant la 
// chaine (pour déterminer la largeur d'une colonne par exemple).
//******************************************************************
int wstrlen(char *pChaine)
{
	int cpt=0;
	
	if(!pChaine) return -1;
	while(*pChaine!='\0')
	{
		if(*pChaine!=-61) 
			cpt++;
		pChaine++;
	}
	return cpt;
}
//******************************************************************
// getStringFromInputBox
//
// Entrées:
//	startposX: position de départ en X
//	startposY: position de départ en Y
//	nbreCols: nombre de colonnes
//  color: code couleur (ANSI)
//	title: titre de la "boite"
//
// Sortie:
//	Une chaine de (nbreCols-2) caractères
//******************************************************************
char* getStringFromInputBox(int startposX,int startposY,int nbreCols,int color,char *pTitle)
{
	int sizeofstring=nbreCols-2;
	if(!pTitle)
	{
		// ...si pas de titre alors on trace une boite sans titre...
		DrawBox(startposX,startposY,1,nbreCols,color);
		return getncharAt(startposX+1,startposY+1,sizeofstring);
	}
	DrawTitledBox(startposX,startposY,1,nbreCols,color,pTitle);
	return getncharAt(startposX+1,startposY+1,sizeofstring);
}

//******************************************************************
// getncharAtEx -- Mai 2014 (Merci Mr Roosen Thibault)
//
// Fait la même chose que getncharAt sauf que le nombre maximum
// de caractères ne peut pas dépasser la largeur du cadre cible
//
// Entrées: le cadre cible...
// Sortie: une chaine de caractères
//******************************************************************

char* getncharAtEx(struct FakeWindowPos DimensionsCadreCible)
{
    if(DimensionsCadreCible.Width>0)
    {
      return getncharAt(DimensionsCadreCible.FirstPrintableX,DimensionsCadreCible.FirstPrintableY,DimensionsCadreCible.Width);
    }
    else
      return NULL;
}

//*****************************************************************************
//  Fonction SetCursor() -- Mettre le curseur à un endroit donné de l'écran
//
//  Entrées:
//
//  posx: valeur en X (colonne)
//  posy: caleur en Y (ligne)
//*****************************************************************************

void SetCursor(int posx,int posy)
{
	wprintf(L"\x1b[%d;%dH\x1b[0m",posy,posx);
	fflush(stdout);
}


//*****************************************************************************
//  Fonction GetConsoleDimensions() -- récupérer les dimensions de la console
//
//  Entrées:
//
//  colonnes: pointeur qui récupère la valeur en nombre de colonnes
//  lignes:   pointeur qui récupère la valeur en nombre de lignes
//*****************************************************************************

void GetConsoleDimensions(unsigned int *colonnes,unsigned int *lignes)
{
	struct winsize Dimensions;
	ioctl(0, TIOCGWINSZ, &Dimensions);
	
	*lignes=Dimensions.ws_row;			// (-3) il faudra faire attention car je retire 3 lignes car je crois que ioctl comptabilise les lignes occupées par le cadre de la console... en plein écran ça marche (175x62)...
	*colonnes=Dimensions.ws_col-2;
}

// février 2014 (abandonné -- pour le moment -- jusqu'à ce que je maitrise mieux l'API dbus)

bool SetConsoleDimensions(int colonnes,int lignes) // Attention il n'envoie pas de messages pour retracer la fenêtre !!
{
	struct winsize Dimensions;
	//char commande[255];
	Dimensions.ws_row=lignes-3;
	Dimensions.ws_col=colonnes;
	
	if(ioctl(0,TIOCSWINSZ,&Dimensions)==-1) return false;
	
	// Tentative d'élargissement ou rétrécissement de la fenêtre
	
	//sprintf(commande,"xterm ",);
	
	return true;
}

//*****************************************************************************
// Fonction Push() -- Placer un élément sur la pile
//
// Entrées:
//
// value:     pointeur sur une donnée à insérer dans la pile... Push((void*)...,...);
// Stack:     pointeur sur la pile en question 
// DataSize:  indique la taille de la donnée qui va être insérée
//*****************************************************************************

void Push(void *value,Stack *pToStack,short DatasSize)
{
	Datas *newElement=(Datas*)malloc(sizeof(Datas));
	if(newElement!=NULL)
	{
		newElement->value=(void*)malloc(DatasSize);
		if(newElement->value!=NULL) memcpy((void*)newElement->value,(void*)value,DatasSize);
		newElement->pNext=NULL;
	}
	if(pToStack->pHead!=NULL)	// il y avait déjà un élément 
	{
		newElement->pNext=pToStack->pHead;
	}
	pToStack->pHead=newElement;
	pToStack->nbElem++;	// on ajoute un élément sur la pile 
}

//*****************************************************************************
// Fonction Pop() -- Retirer un élément de la pile
//
// Entrées:
//
// Stack:     pointeur sur la pile d'où extraire l'élément
// DataSize:  indique la taille de l'élément à enlever
//
// Sortie:
//
// void*  un élément de n'importe quel type... 
//*****************************************************************************

void* Pop(Stack *pToStack,short DatasSize)
{
	void *tmp=(void*)malloc(DatasSize);
	if(pToStack->pHead!=NULL)
	{
		memcpy((void*)tmp,(void*)pToStack->pHead->value,DatasSize);
		// modif 9 juillet 2016
		free(pToStack->pHead->value);
		pToStack->pHead=pToStack->pHead->pNext;
		pToStack->nbElem--;	// on retire un élément de la pile
	}
	return tmp;
}

//******************************************************************
// Log -- Octobre 2014
//
// Permet de journaliser les événements de manière claire
// dans un fichier de type texte.
//
// Entrées: 
//	openedFile: le descripteur de fichier (pointeur) 
//	pMessage:		la chaine à journaliser
// Sortie: néant
//******************************************************************

void Log(FILE *openedFile,char *pMessage)
{
	time_t EventLogTime=time(NULL);
	struct timespec ClockTime;
	char	timestamp[SIZEDATE];			
	char  tmp[strlen(pMessage)+LOGBUF];				// 20 août 2015 corrigé le 20 janvier 2016 (dépassement de buffer)
	//char 	clocktime[SIZEDATE];
	
	if(openedFile==NULL) return;					// 31 mars 2015
	
	//tmp[strlen(pMessage)+LOGBUF]='\0';
	memset(tmp,0,strlen(pMessage)+LOGBUF);
		
	strcpy(timestamp,asctime(localtime(&EventLogTime)));
	NoCarriage(timestamp);
	
	clock_gettime(0,&ClockTime);
	
	// corrigé le 20 janvier 2016
	
	//sprintf(clocktime,"[%08d.%09d]",ClockTime.tv_sec,ClockTime.tv_nsec);
	//fwrite(clocktime,1,strlen(clocktime),openedFile);
	//fflush(openedFile);
	sprintf(tmp,"[%08d.%09d][%s]\t%s\n",ClockTime.tv_sec,ClockTime.tv_nsec,timestamp,pMessage);  //modifié en juillet 2016
	fwrite(tmp,1,strlen(tmp),openedFile);
	fflush(openedFile);
}

//******************************************************************
// InitLog -- Janvier 2018
//
// Permet de créer un répertoire permettant d'y créer un fichier 
// de journalisation (log file)
//
// Entrées: 
// 	pFolder: nom du répertoire
//	pFilename: nom du fichier
//  mode: LOGW (write),LOGA (append)		
// Sortie: descripteur de fichier
//******************************************************************
FILE *InitLog(char *pFolder,char *pFilename,e_logmode mode)
{
	FILE	*pFichier;
	
	if(!pFolder || !pFilename) return NULL;
	
	umask(0);
	mkdir(pFolder,S_IRWXO);
	chdir(pFolder); 							// Fout la merde !!!!! août 2020
	
	if(mode==LOGA)
		pFichier=fopen(pFilename,"a+");
	else
		pFichier=fopen(pFilename,"w");
	
	// Calculer le nombre de niveaux pour remonter !!!! août 2020
	
	char *pChercher=NULL;
	int niveaux=0;
	while((pChercher=strchr(pFolder,'/'))!=NULL)
	{
		niveaux++;												
		pFolder++;  										// "pointer" le caractère situé après le '/'
		if(*pFolder=='\0') break;				// si on arrive à la fin de la chaîne, hop !! on sort ^^
	}
	
	while(--niveaux>0)
	{
		chdir(".."); // revenir au répertoire du dessus...
	}
	
	return pFichier;
}


//*****************************************************************************
// Fonction NoCarriage -- Enlève les caractères de fin de ligne superflus
//
//  Entrée:
//  
//  pChaine: chaîne de caractères (elle se vera retirer le '\n' et '\r')
//*****************************************************************************

void NoCarriage(char *pChaine)
{
	int cpt;
	
	for(cpt=0;cpt<=strlen(pChaine);cpt++) 
	{
		if(pChaine[cpt]==13 || pChaine[cpt]==10) pChaine[cpt]='\0';
	}
}

//*****************************************************************************
// Fonction InitCSSMWIDE -- Initialise la librairie CSSMWIDE
//
// Entrée: 
//		pLocale: chaîne représentant l'encodage de la console voulu (ex: fr_BE.UTF-8)
// Sortie: néant
//*****************************************************************************
void InitUEPWIDE(char *pLocale)
{
	setlocale(LC_ALL,pLocale);
}

//*****************************************************************************
// Fonction setForegroundColor -- spécifie la couleur RGB de l'avant plan
//
// Entrée: 
//		color:  structure contenant les dosages de rouge, vert et bleu ainsi que 
//            le "flag" bold/gras.
// Sortie: néant
//*****************************************************************************

void setForegroundColor(struct s_RGB color)
{
	char *couleurAvantPlan=getRGBString(color);
	if(color.bold) wprintf(L"\x1b[1m");
	else wprintf(L"\x1b[0m");
	wprintf(L"%s",couleurAvantPlan);
	fflush(stdout);
	free(couleurAvantPlan);
}

//*****************************************************************************
// Fonction setBackgroundColor -- spécifie la couleur RGB de l'arrière plan
//
// Entrée: 
//		color:  structure contenant les dosages de rouge, vert et bleu ainsi que 
//            le "flag" bold/gras.
// Sortie: néant
//*****************************************************************************

void setBackgroundColor(struct s_RGB color)
{
	char *couleurFond=getReverseRGBString(color);
	wprintf(L"%s",couleurFond);
	free(couleurFond);
}

//*****************************************************************************
// Fonction EmptyStack -- vide la pile et libère la mémoire
//
// Entrée: 
//		target:  la pile dont nous voudrions libérer la mémoire
//            	
// Sortie: néant
//*****************************************************************************
void EmptyStack(Stack *target)
{
	while(target->pHead!=NULL)
	{
		struct Datas *tmp=target->pHead->pNext;
		free(target->pHead->value);
		target->pHead=tmp;
		target->nbElem--;
	}
}
//*****************************************************************************
// Fonction PaintBox -- colorie le cadre cible 
//
// Entrée: 
//		target:	le cadre à peindre
//		color:	la couleur de fond de type RGB 
//            	
// Sortie: néant
//*****************************************************************************
void PaintBox(struct FakeWindowPos target,struct s_RGB color)
{
	int origineX=target.FirstPrintableX,origineY=target.FirstPrintableY;											// nous sauvegardons les positions d'origine vu que nous allons modifier posX et posY...
	int posX=origineX;
	int posY=origineY;
	int height=target.Height;
	int length=target.Width;
	
	char *pColorBoxString;
	pColorBoxString=getReverseRGBString(color);
		
	wprintf(L"\x1b[%d;%dH",posY,posX);
	wprintf(L"%s%lc",pColorBoxString,L' ');
	for(posX++;posX<origineX+length-1;posX++)
	{
		wprintf(L"\x1b[%d;%dH",posY,posX);
		wprintf(L"%s%lc",pColorBoxString,L' ');
	}
	wprintf(L"%s%lc",pColorBoxString,L' ');
	posY++;
	posX=origineX;
	for(;posY<=origineY+height-2;posY++)
	{
		wprintf(L"\x1b[%d;%dH",posY,posX);
		wprintf(L"%s%lc",pColorBoxString,L' ');
		for(posX++;posX<origineX+length-1;posX++)
		{
			wprintf(L"\x1b[%d;%dH",posY,posX);
			wprintf(L" ");
		}
		wprintf(L"%s%lc",pColorBoxString,L' ');
		posX=origineX;
	}
	wprintf(L"\x1b[%d;%dH",posY,posX);
	wprintf(L"%s%lc",pColorBoxString,L' ');
	for(posX++;posX<origineX+length-1;posX++)
	{
		wprintf(L"\x1b[%d;%dH",posY,posX);
		wprintf(L"%s%lc",pColorBoxString,L' ');
	}
	wprintf(L"%s%lc",pColorBoxString,L' ');
	wprintf(L"\x1b[0m");
	fflush(stdout);
}
//*****************************************************************************
// Fonction nbgetch (Merci à Lena ^_^ 2016)
//
// Entrée:	néant
//            	
// Sortie: le caractère entré au clavier
//*****************************************************************************
#ifdef LINUX
int nbgetch(void)
{
	char							Buffer;
	struct termios		oldtcfg;
	struct termios		newtcfg;
	struct sigaction	Detournement={ { 0 } };
	
	Detournement.sa_handler=SIG_IGN;
	// On ne peut pas interrompre le getch...
	sigaction(SIGINT,&Detournement,NULL);
		
	tcgetattr(STDIN_FILENO, &oldtcfg);
  newtcfg = oldtcfg;
	
	//newtcfg.c_lflag &= ~(ICANON | ECHO); // | ECHONL | ECHOCTL); 			// affiche le caractère alors que je dis de ne pas faire l'écho...
	//tcsetattr(STDIN_FILENO, TCSANOW, &newtcfg);
	
	//newtcfg.c_lflag=0;
	// tcsetattr(STDIN_FILENO, TCSADRAIN, &newtcfg);
	
	// mode non bloquant...
		
	newtcfg.c_cc[VMIN] = 0;
	newtcfg.c_cc[VTIME] = 2;																											// 0 ça pousse le processeur à 100% (faire gaffe à ce que l'on fait)
	
	// BUG 2021
	// Il ne faut pas faire l'écho du '\n'...
	
	newtcfg.c_lflag &= ~(ICANON|ECHOCTL);																					// n'a pas besoin d'appuyer sur "entrée" (caractère non bloquant de la procédure)
	tcsetattr(STDIN_FILENO, TCSANOW, &newtcfg);
	
	Buffer=getchar();	// retourne EOF (-1) si il y a rien dans le buffer
	
	// on restitue "l'ancienne configuration"
	tcsetattr(STDIN_FILENO, TCSANOW, &oldtcfg); 
	
	Detournement.sa_handler=SIG_IGN;
		
	//sigaction(SIGINT,&Detournement,NULL); 2021
	
	// 2021
	
	Detournement.sa_handler=SIG_DFL;
	Detournement.sa_flags=0;
	sigaction(SIGINT,&Detournement,NULL);
	
	return Buffer;
}
#endif

//*****************************************************************************
// Fonction nbgetchEx (sans écho/affichage du caractère saisi)
//
// Entrée:	néant
//            	
// Sortie: le caractère entré au clavier
//*****************************************************************************
#ifdef LINUX
int nbgetchEx(void)
{
	char							Buffer;
	struct termios		oldtcfg;
	struct termios		newtcfg;
	struct sigaction	Detournement={ { 0 } };
	
	Detournement.sa_handler=SIG_IGN;
	// On ne peut pas interrompre le getch...
	sigaction(SIGINT,&Detournement,NULL);
		
	tcgetattr(STDIN_FILENO, &oldtcfg);
  newtcfg = oldtcfg;
	
	//newtcfg.c_lflag &= ~(ICANON | ECHO); // | ECHONL | ECHOCTL); 			// affiche le caractère alors que je dis de ne pas faire l'écho...
	//tcsetattr(STDIN_FILENO, TCSANOW, &newtcfg);
	
	//newtcfg.c_lflag=0;
	// tcsetattr(STDIN_FILENO, TCSADRAIN, &newtcfg);
	
	// mode non bloquant...
		
	newtcfg.c_cc[VMIN] = 0;
	newtcfg.c_cc[VTIME] = 2;																											// 0 ça pousse le processeur à 100% (faire gaffe à ce que l'on fait)
	newtcfg.c_lflag &= ~(ICANON|ECHOCTL|ECHO);																		// n'a pas besoin d'appuyer sur "entrée" (caractère non bloquant de la procédure) sans écho
	tcsetattr(STDIN_FILENO, TCSANOW, &newtcfg);
	
	Buffer=getchar();	// retourne EOF (-1) si il y a rien dans le buffer
	
	// on restitue "l'ancienne configuration"
	tcsetattr(STDIN_FILENO, TCSANOW, &oldtcfg); 
	
	Detournement.sa_handler=SIG_DFL;
	Detournement.sa_flags=0;
	sigaction(SIGINT,&Detournement,NULL);
	
	return Buffer;
}
#endif


//*****************************************************************************
// Fonction getTimedUTF8String
//
// Permet une saisie d'une chaîne de caractères avec un timeout...
//
// Entrées:	
//	maxlen: 	nombre de caractères à saisir
//	maxtime: 	délais du timeout
//            	
// Sortie: la chaîne de caractères 
//*****************************************************************************

//*****************************************************************************
// Fonction en tests ^^
//*****************************************************************************

// HISTORIQUE

// 2016: éllaboration du mécanisme au niveau "grosso merdo"
// 2017: décembre
//				remplacement des printf par wprintf
//				mise en place de l'empêchement d'appuyer sur CTRL-C
// 2021: il fallait compléter cette fonction pour qu'elle ressemble à getnchar()

// Refonte février 2021
// Copier getnchar() et juste remplacer getch() par nbgetch()
// Ainsi que la gestion du "timer"

char* getTimedUTF8String(short maximum,short maxtime)
{
	int 	offset=0;										// déplacement
	int		nbreCar=0;									// permet de s'assurer que le nombre de caractères est bien atteint
	int 	adaptedsize=maximum;				// la taille peut changer à cause des caractères UTF8
	wchar_t convertedCar;							// permet d'afficher les caractères UTF8 convenablement 
	char    toconvert[2];							// warning leap15 juillet 2018								
	char *pReturnPointer;							// pointeur à retourner à la fonction appelante
	char *pTmp;												// pointeur temporaire
		
	if(maximum>0)
	{
		pReturnPointer=(char*)calloc(maximum+1,sizeof(char));		
		pTmp=(char*)calloc(maximum+1,sizeof(char));				
		//memset((void*)pTmp,0,maximum);
		//memset((void*)pReturnPointer,0,maximum);    // corrections 28 mai 2018
	}
	else
		return NULL;
	
	time_t start=time(NULL);
	time_t current;
		
	while(nbreCar<maximum)
	{
		*pTmp=nbgetch();								// c'est complexe je sais...
		current=time(NULL);
		if(current-start==maxtime) break;
		if(*pTmp==-1) continue;					// on ne fait le traitement que si pTmp contient autre chose que -1 (aucune touche appuyée)
				
		if(*pTmp==10 || *pTmp==27) break;
		
		
		if(*pTmp>0) // Il ne s'agit pas de caractères UTF8
		{
			if(*pTmp!=127) 							// Si pas backspace (effacer)
			{
				wprintf(L"%c",*pTmp);			// sous Windows, getch() ne fait pas l'écho du caractère non plus...
				pTmp++;					
				offset++;	
				nbreCar++;								// incrémentation du compteur pour pouvoir sortir...
			}
			else // gestion du caractère BS (back space)
			{
				if(offset>0 && nbreCar>0)
				{
					wprintf(L"\x1b[1D"); 	// on retourne en arrière
					wprintf(L" ");				// on efface le caractère
					wprintf(L"\x1b[1D");	// on retourne en arrière
					pTmp--;								// idem pour le pointeur il faut revenir une position "en arrière"
					offset--;							// on se déplace d'une "case" à gauche en mémoire...
					nbreCar--;						// il ne faut pas compter cette entrée comme un caractère
				}
				if(*pTmp<0)							// si c'est un caractère UTF8 
				{
					pTmp--;								// il faut effacer le code -61 
					offset--;							// on se déplace d'une "case" à gauche en mémoire...
				}
			}
		}
		// Gestion des caractères UTF8
		else
		{
			if(maximum>1) // ce bloc sera exécuté deux fois (un caractère utf8 est codé sur 2 bytes)...
			{
				if(*pTmp!=-61 && *pTmp!=-62) nbreCar++;																	// on ne compte le caractère que si nous avons traité les deux caractères UTF8
				//strncpy(&toconvert,pTmp,2);																						// deux bytes à convertir en unicode
				strncpy(toconvert,pTmp,2);
				//int rc=mbtowc(&convertedCar,&toconvert,1);
				int rc=mbtowc(&convertedCar,toconvert,1);
				if(convertedCar>0 && rc>0) 																							// on affiche uniquement lorsque le caractère est valide...
				{
					wprintf(L"%lc",convertedCar);
				}
				adaptedsize++;   
				
				// tentative de ré-allouer la zone mémoire pour s'adapter aux caractères entrés... (correction 28 mai 2018)
				// BUG: dans certains cas (surtout dans netbeans :{) quand nous plaçons plusieurs caractères UTF8 les uns après les autres 
				// nous "obtenons" un double free or corruption (fasttop)
				
				// C library thinks you did a double free (that is, you freed the same thing twice, which is of course a bug) or that you corrupted its data structures, 
				// such as by writing beyond the end of a buffer you allocated. 
				
				// valgrind indique des lectures, écritures et libérations (free/realloc/delete) dans realloc()...
#ifdef AVANT				
				pReturnPointer=(char*)realloc(pReturnPointer,sizeof(char)*adaptedsize);
#else
				//free(pReturnPointer);
				pReturnPointer=(char*)calloc(adaptedsize,0); 	// BUG résolu, maintenant je n'ai aucune explications concernant le fait que realloc() fonctionne
																											// "quand elle en a envie" :{
#endif

				pReturnPointer=pTmp-offset;																							// nous venons de changer la taille de la chaine il faut lui donner une valeur...
								
				offset++;
				pTmp++;
			}
			else
			{
				// Encodage... d'un seul caractère de type UTF8

				pReturnPointer=(char*)realloc(pReturnPointer,sizeof(char)*3);		// le code -61 (ou -62), le code du caractère et \0...
				pReturnPointer[0]=*pTmp;						// indique qu'il s'agit d'UTF8 (pour l'affichage)
				pReturnPointer[1]=getch();					// les caractères UTF8 ne sont pas complets jusqu'à ce que 2 bytes aient été "saisis" le getch() ici récupère la partie manquante...
				pReturnPointer[2]='\0';							// on indique la fin de la chaine...
				
				// Affichage
				
				wchar_t todisplay;
				int rc=mbtowc(&todisplay,pReturnPointer,2);
				if(rc>0)
					wprintf(L"%lc",todisplay);
				return pReturnPointer;																					// inutile d'aller plus loin...
			}
		}
		
	} // endwhile
	*pTmp='\0'; 
	pReturnPointer=pTmp-offset;	// on retourne le début de phrase et non la position actuelle...
	return pReturnPointer;
}


/*
 * Fonction qui active ou désactive l'écho des caractères entrés au terminal (foire avec l'utilisation de getch() et consors --> à tester :{ )
 */
int toggleEcho(void)
{
	//static bool toggle=true;
	struct termios terminal;
	
	if(tcgetattr(fileno(stdout),&terminal)==-1) return -1;
	
	terminal.c_lflag ^= ECHO;				// inversion du bit ECHO
	
	tcsetattr(fileno(stdout),TCSAFLUSH,&terminal);
	return 0;
}

int toggleSignalsEcho(void)
{
	struct termios		terminal;
	
	if(tcgetattr(STDOUT_FILENO, &terminal)==-1) return -1;
	
	// juillet 2018
	terminal.c_lflag |= ~ISIG;																			// permet de modifier le caractère affiché lorsque nous appuyons sur CTRL-Z
	terminal.c_cc[VSUSP]=0;																					// on dit que CTRL-Z n'affichera rien du tout
	terminal.c_lflag ^= ECHOCTL;																		// éviter de voir ^C ou ^Z quand on appuye sur ces touches...
	
	
	tcsetattr(STDOUT_FILENO,TCSAFLUSH,&terminal);
	return 0;
}

/*
 * Affiche un texte dans un cadre mais si celui-ci contient plus de caractères que la largeur du cadre, contrairement aux autres
 * fonctions de type Display* le texte va défiler de droite à gauche... (merci Monsieur Guagenti).
*/

/*
 * Input: 
 * position: position du cadre
 * pTexte: chaîne de caractères (texte à faire défiler)
 * Foreground: couleur d'avant-plan au format RGBa 
 * Background: couleur d'arrière plan au format RGBa
 * 
 * Output: l'identifiant du thread.
*/

pthread_t DisplayAnimatedText(PositionCadre position,char *pTexte,struct s_RGB Foreground,struct s_RGB Background)
{
	int local_threadID;
	
  // Pour pouvoir faire en sorte de ne pas bloquer le thread principal, il faut créer un thread fils...
  
  pthread_t   pt_core;
  //t_CSSMTData dat_donnees={pTexte,Foreground,Background,position};                                    // erreur de débutant :{
  
  t_CSSMTData *dat_donnees=malloc(sizeof(t_CSSMTData));                                                 // Là c'est mieux !!
  dat_donnees->pTexte=pTexte;
  dat_donnees->data_posCadre=position;
  dat_donnees->background=Background;
  dat_donnees->foreground=Foreground;
  
	/*
	 *	Ce n'est pas le summun en matière de programmation car les threads occupent un descripteur (4 bytes, c'est pas beaucoup)
	 *  et ici je ne suis pas certain que cet espace mémoire soit libéré à la fin du programme...
	 */
  		
  local_threadID=pthread_create(&pt_core,NULL,(void*)&fct_core,(void*)dat_donnees);
	if(local_threadID==0) return pt_core;
	return (pthread_t)-1;					// mars 2019: erreur qui mérite la mort: pthread_create ne RETOURNE PAS UN IDENTIFIANT (il se trouve dans pt_core)
}

/* 
 * Fonction qui EFFECTIVEMENT provoque l'animation du texte passé en paramètre... 
 * 
 */

void fct_core(void *magie)
{
  struct timespec attente;
  
  int indicelettre=0;																// Nous allons extraire les lettres de la phrase une à une
  int posX=0;																				// position de la lettre
  
  attente.tv_sec=0L;
  attente.tv_nsec=125000000L;												// 1/8ème de seconde...     
   
  // 18 juin 2017: l'instruction qui suit peut dans le cadre des threads poser des soucis... 
  //t_CSSMTData *donnees=(t_CSSMTData*)magie;
  
  t_CSSMTData *donnees=malloc(sizeof(t_CSSMTData));
	donnees=(t_CSSMTData*)magie;

	int taillereelle=wstrlen(donnees->pTexte);
  wchar_t tmpMsg[taillereelle+1];
	
  wmemset(tmpMsg,0,taillereelle+1);									// memset "wide" (utf8) 
  mbstowcs(tmpMsg,donnees->pTexte,taillereelle+1);  // converti une chaine de caractères normale en "wide"
  tmpMsg[taillereelle+1]=L'\0';                     // pour éviter les "effets de bord"
	
	nanosleep(&attente,NULL);
  while(fctcore_brunning)
  {
    // vers la gauche
		
    while(indicelettre<taillereelle-1)							// Nous devons lire tout le texte...
    {
			pthread_mutex_lock(&fctcore_mtx_protection);																											// VEROUILLAGE
			SetCursor(donnees->data_posCadre.FirstPrintableX+posX,donnees->data_posCadre.FirstPrintableY);
			setForegroundColor(donnees->foreground);
      setBackgroundColor(donnees->background);
			wprintf(L"%lc",tmpMsg[indicelettre]); 
      fflush(stdout);
			posX++;
			pthread_mutex_unlock(&fctcore_mtx_protection);																										// DEVEROUILLAGE
			
			if(posX>donnees->data_posCadre.Width-1)         // ...si nous arrivons à la fin du cadre...
      {
        posX=0;
        indicelettre-=donnees->data_posCadre.Width-1;																										// donne l'effet de défilement...
      
        SetCursor(donnees->data_posCadre.FirstPrintableX,donnees->data_posCadre.FirstPrintableY);
        nanosleep(&attente,NULL);
      }
      indicelettre++;                               // Nous passons à la lettre suivante...
    }
		
		nanosleep(&attente,NULL);

    // sens inverse vers la droite...

    // Logiquement nous avons indicelettre qui représente le dernier élément de la phrase...

    // Nous allons parcourir le tableau dans l'autre sens...

    posX=donnees->data_posCadre.Width+donnees->data_posCadre.FirstPrintableX-1;            // on doit commencer non plus à la première colonne disponible DANS le cadre mais à la dernière...

    // ...et comme FirstPrintableX indique la position de la marge de gauche, il faut l'ajouter sinon notre calcul n'est pas correct...

    while(indicelettre>=0)
    {
			pthread_mutex_lock(&fctcore_mtx_protection);																																																						// VEROUILLAGE		
      SetCursor(posX,donnees->data_posCadre.FirstPrintableY);
			setForegroundColor(donnees->foreground);
      setBackgroundColor(donnees->background);
      wprintf(L"%lc",tmpMsg[indicelettre]); 
			fflush(stdout);         // ...sinon on risque de ne pas être "synchro" avec ce qui affiché à l'écran...
			
			posX--;
			pthread_mutex_unlock(&fctcore_mtx_protection);																																																					// DEVEROUILLAGE
			
      if(posX<donnees->data_posCadre.FirstPrintableX && indicelettre!=0) // si nous arrivons à la marge de gauche... et qu'en plus nous ne sommes pas arrivés au "début" de la phrase...
      {
        // ...nous simulons un déplacement vers la droite...
        posX=donnees->data_posCadre.Width+donnees->data_posCadre.FirstPrintableX-1;
        indicelettre+=donnees->data_posCadre.Width-1;
        SetCursor(posX,donnees->data_posCadre.FirstPrintableY);
        nanosleep(&attente,NULL);
      }
      

      indicelettre--;
    }
    
    // on retourne dans l'autre sens... (je l'ai pas fait en cyclique mais c'est à mettre en place ^^) 
    
    indicelettre=1;   // petite tricherie ^^
    posX=1;           // Ceci pour ne pas avoir, lorsque nous retournons de nouveau vers la droite, deux fois la première lettre qui s'affiche...
		nanosleep(&attente,NULL);
  } // lorsque bRunning passe à "faux"
	free(donnees); // mai 2019
	pthread_exit(0);
}

void toggleAnimatedText(void)
{
	if(fctcore_brunning) fctcore_brunning=false;
	else fctcore_brunning=true;
}
