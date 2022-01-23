#ifndef __UEPWIDE__
#define __UEPWIDE__

#include <stdio.h>
#include <pthread.h>

#ifdef BEFORE
#define UTF8_COINSUPGAUCHE  0x2554
#define UTF8_LIGNE          0x2550
#define UTF8_COINSUPDROIT   0x2557
#define UTF8_LIGNEVERT      0x2551
#define UTF8_COININFGAUCHE  0x255a
#define UTF8_COININFDROIT   0x255d
#define UTF8_COLSUP         0x2566
#define UTF8_COLINF         0x2569
#define UTF8_CELLGAUCHE     0x2560
#define UTF8_CELLDROITE     0x2563
#define UTF8_CROISEMENT			0x256c
#else
#define UTF8_COINSUPGAUCHE  0x250F
#define UTF8_LIGNE          0x2501
#define UTF8_COINSUPDROIT   0x2513
#define UTF8_LIGNEVERT      0x2503
#define UTF8_COININFGAUCHE  0x2517
#define UTF8_COININFDROIT   0x251B
#define UTF8_COLSUP         0x2533	
#define UTF8_COLINF         0x253B
#define UTF8_CELLGAUCHE     0x2523
#define UTF8_CELLDROITE     0x252B	
#define UTF8_CROISEMENT			0x254B
#endif

#define ALIGN_LEFT          0
#define ALIGN_CENTER        1
#define ALIGN_RIGHT         2

#define COLORED_BOX				0
#define COLORED_TITLE 		1
#define COLORED_BOTH      2
#define COLORED_SEPARATE	3
#define COLOR_NULL				((struct s_RGB){0})

#define CSSM_AS_FST	  065000000L
#define CSSM_AS_NRML  127000000L
#define	CSSM_AS_SLW	  254000000L

#define echoflags (ECHO|ECHOE|ECHOK|ECHONL)

#ifdef PORTABLE
#define COULCOORD	30
#else
#define COULCOORD	32
#endif

#define SIZEDATE	26
#define LOGBUF 		48

//*****************************************************************************
// Définition des structures utilisées
//*****************************************************************************
#ifndef bool                                                                                                                                                    // pour ne pas rentrer en conflit avec le "bool" défini dans la librairie MySQL...
typedef enum {false,true}bool;
#define bool bool
#endif                                                                                                                                                          // ATTENTION !! Il faut que "uep_wide.h" soit inclu AVANT tout header relatif à MySQL.


/* juillet 2020
peut faire double emploi dans certaines applications déjà existantes /!\
*/


typedef struct  
{
	int x;
	int y;
}Coordonnee;

typedef struct
{
	int Heures;
	int Minutes;
	int Secondes;
}chrono;

struct s_RGB
{
  unsigned short Red;
  unsigned short Green;
  unsigned short Blue;
  bool  bold;
  unsigned short alpha;  // facilite la gestion de la transparence en SDL2 
};

// Structure qui devrait permettre de pouvoir adresser une "fenêtre" (fausse fenêtre) sur un écran

struct FakeWindowPos
{
	unsigned short posX;            // position horizontale (position du cadre de gauche)
	unsigned short posY;            // position verticale   (position du cadre supérieur)
	unsigned short Width;           // taille				(largeur, nombre de caractères)
	unsigned short Height;          // hauteur				(nombre de lignes exploitables)
	
	unsigned short FirstPrintableX;	// position du premier caractère "imprimable"
	unsigned short FirstPrintableY;	// position de la première ligne "exploitable"
	
	unsigned short Last;						// première ligne exploitable en dehors du cadre
	unsigned short nbMessages;			// tant qu'à utiliser un paramètre autant le faire bien
};

// Structures nécessaires à l'utilisation des piles (Stack), LIFO 

typedef struct Datas
{
	void          *value;
	struct Datas	*pNext;
}Datas;

typedef struct s_Stack
{
	Datas	*pHead;
	int     nbElem;                             // Censé contenir le nombre d'éléments actuel...
}Stack;

typedef struct s_CSSMTData
{
  char                  *pTexte;
  struct s_RGB          foreground;
  struct s_RGB          background;
  struct FakeWindowPos  data_posCadre;
}t_CSSMTData;    																// Permet l'animation de textes dans un cadre prédéfini...

typedef struct FakeWindowPos    PositionCadre;	// Définition d'un alias... (octobre 2015)
typedef struct s_Stack          Pile;

typedef enum {LOGW,LOGA}e_logmode;



//*****************************************************************************
// Prototypes des fonctions
//*****************************************************************************

void  AddToMessageBox(char*,int,int,int,int,unsigned short*);																// Ajoute une entrée dans une boite aux coordonnées (x,y)
#ifdef OLDVERSION
void  AddToMessageBoxEx(char*,struct FakeWindowPos,unsigned short*);												// ...idem mais avec moins de paramètres ^^
#else
void  AddToMessageBoxEx(char*,struct FakeWindowPos*);																				// ...idem mais avec moins de paramètres ^^
#endif
void  AfficherXY(char*,int,int);																														// Affiche un texte aux coordonnées (x,y)
void  BackToShell(short,short);																															// Fonction qui permet d'afficher le prompt au bon endroit
void  EffacerEcran();																																				// Efface l'écran
void  SetCursor(int,int);																																		// Permet de placer le curseur à une coordonnée (x,y) 
void  GetConsoleDimensions(unsigned int*,unsigned int*);																		// Permet de récupérer les dimensions d'une console (devrait fonctionner sur Windows)

// Ajouté le 19 octobre 2014

void Log(FILE*,char*);																																			// Permet d'ajouter des informations dans le fichier de log...
void NoCarriage(char*);																																			// Permet d'enlever les caractères '\n' et '\r' d'une chaine de caractères...	

// Ajouté le 31 octobre 2015

void InitUEPWIDE(char*);																																		// Permet d'initialiser la librairie CSSM_WIDE

// Ajouté le 05 novembre 2015

void setForegroundColor(struct s_RGB);																											// Permet de spécifier la couleur RGB de l'avant plan  
void setBackgroundColor(struct s_RGB);																											// Permet de spécifier la couleur RGB de l'arrière plan

// Ajouté le 4 juin 2016

void PaintBox(struct FakeWindowPos,struct s_RGB);                                           // Permet de "peindre" le background d'un cadre...	

//void  Interruptions(int);																																	// Fonction utilisée dans getch() --> doit être ajoutée dans tous les programmes utilisant uep_wide.h 

void  Push(void*,Stack*,short);																															// Permet de "pusher" (mettre sur la pile) une variable de n'importe quel type (haute voltige)
void* Pop(Stack*,short);																																		// Permet de "popper" (enlever de la pile) une variable de n'importe quel type (haute voltige)
void  EmptyStack(Stack*);                                                                   // Permet de vider la pile  
int   toggleEcho(void);                                                                     // Permet d'activer ou désactiver l'écho des caractères entrés au clavier

// 7 octobre 2017 
int     toggleSignalsEcho(void);
struct  termios saveTerminalSettings();

int	  DisplayAligned(char*,int,int,int,int);																								// Affiche un texte avec une justification aux coordonnées (x,y)
int	  getch(void);																																					// Effectue une saisie d'un caractère sans besoin d'appuyer sur return
int   nbgetch(void);																																				// Effectue une saisie d'un caractère sans besoin d'appuyer sur return (non bloquant)
int		nbgetchEx(void);																																			// idem mais sans faire l'écho du caractère saisi...
int	  getchAt(int,int);																																			// Effectue un getch aux coordonnées (x,y)
int	  wstrlen(char*);																																				// Calcule le nombre de caractères lisibles sans se préoccuper de l'encodage

char* getnchar(int);																																				// Effectue la saisie d'une chaine de caractères
char* getncharAt(int,int,int);																															// Effectue la saisie d'une chaine de caractères aux coordonnées (x,y)
char* getncharAtEx(struct FakeWindowPos);																										// ...même chose mais nous ne pourrons pas dépasser les marges du cadre...			
char* getRGBString(struct s_RGB);																														// Permet de convertir une structure s_RGB en chaine ANSI (foreground)
char* getReverseRGBString(struct s_RGB);																										// Même chose mais pour le background (affichage inversé)
char* getStringFromInputBox(int,int,int,int,char*);																					// Fonction émulant un contrôle texte en mode console
char* splitDateToTime(char*);																																// Permet de récupérer l'heure

chrono	getTimeElapsed(char*,char*);																												// Permet de connaître le temps écoulé

struct FakeWindowPos DrawBox(int,int,int,int,int);																					// Trace une boite
struct FakeWindowPos DrawBoxWithRGB(int,int,int,int,struct s_RGB);													// Même chose mais en RGB
struct FakeWindowPos DrawTitledBox(int,int,int,int,int,char*);															// Trace une boite avec un titre
struct FakeWindowPos DrawTitledBoxWithRGB(int,int,int,int,struct s_RGB,char*,struct s_RGB,short);			// Amélioration de DrawTitledBox pour la prise en charge de l'RGB

struct s_RGB getRGB(short,short,short,bool);																													// Permet de convertir des valeurs RGB en structure s_RGB

FILE*	InitLog(char*,char*,e_logmode);																																	// Initialise le système de journalisation

// Ajoutée le 28 février 2017

char* getTimedUTF8String(short,short);                                                                // permet de faire en sorte de limiter le temps imparti pour la saisie

// Marche pas...

bool SetConsoledimensions(int,int);																																		// Permet de forcer les dimensions d'une console (ne marche pas encore)

// tests d'inclusion de nouvelles fonctions (18 juin 2017)

pthread_t DisplayAnimatedText(PositionCadre, char*,struct s_RGB,struct s_RGB);                              // Permet d'animer du texte à l'intérieur d'un "cadre" prédéfini...
void fct_core(void*);																																									// Fonction de gestion de l'animation proprement dite...
void toggleAnimatedText();

void* getElementEx(void*,int,int,int,int,int,int);
int setElementEx(void*,int,int,int,int,int,int,void*);

// juillet 2020

void ResetBox(struct FakeWindowPos*);
unsigned int getPIDByName(char*);

#endif
