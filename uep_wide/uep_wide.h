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

#ifndef COLORSDEFINED
#define COLORSDEFINED

// FIRST DRAW

#define COL_RED {255,0,0,false,0}	
#define COL_GRE {170,255,0,false,0}
#define COL_YEL {255,255,191,false,0}
#define COL_BLU {80,170,255,false,0}
#define COL_MAG {117,80,123,false,0}
#define COL_CYA {85,255,255,false,0}
#define COL_ORA {255,128,0,false,0}
#define COL_WHI {255,255,255,false,0}
#define COL_PNK {255,170,255,false,0}

#define COL_BR_RED {239,41,41,true,0}
#define COL_BR_GRE {198,255,216,true,0}
#define COL_BR_YEL {252,233,79,true,0}
#define COL_BR_BLU {50,175,255,true,0}
#define COL_BR_MAG {173,127,168,true,0}
#define COL_BR_CYA {52,226,255,true,0}
#define COL_BR_WHI {255,255,255,true,0}
#define COL_BR_PNK {255,170,255,true,0}
#define COL_BR_ORA {255,128,0,true,0}


#define COL_INDIANRED		{205,92,92,false,0}
#define COL_LIGHTCORAL	{240,128,128,false,0}
#define COL_SALMON			{250,128,114,false,0}
#define COL_DRKSALMON		{233,150,122,false,0}
#define COL_LGTSALMON		{255,160,122,false,0}
#define COL_CRIMSON			{220,20,60,false,0}
#define COL_FIREBRICK		{178,34,34,false,0}
#define COL_DRKRED			{139,0,0,false,0}

#define COL_PINK				{255,192,203,false,0}
#define COL_LGTPINK			{255,182,193,false,0}
#define COL_HOTPINK			{255,105,180,false,0}
#define COL_DEEPPINK		{255,20,147,false,0}
#define COL_MEDVIOLET		{199,21,133,false,0}
#define COL_PALEVIOLET	{219,112,147,false,0}

#define COL_TOMATO				{255,99,71,false,0}
#define COL_ORANGERED			{255,59,0,false,0}
#define COL_DRKORANGE			{255,140,0,false,0}
#define COL_ORANGE				{255,165,0,false,0}
#define COL_Gold					{255,215,0,false,0}
#define COL_Yellow				{255,255,0,false,0}
#define COL_LightYellow		{255,255,224,false,0}
#define COL_LemonChiffon	{255,250,205,false,0}
#define COL_LgtGdnrodYel	{250,250,210,false,0}
#define COL_PapayaWhip 		{255,239,213,false,0}
#define COL_Moccasin			{255,228,181,false,0}
#define COL_PeachPuff			{255,218,185,false,0}
#define COL_PaleGoldenrod {238,232,170,false,0}
#define COL_Khaki					{240,230,140,false,0}
#define COL_DarkKhaki			{189,183,107,false,0}

#define COL_Lavender				{230,230,250,false,0}
#define COL_Thistle					{216,191,216,false,0}
#define COL_Plum						{221,160,221,false,0}
#define COL_Violet					{238,130,238,false,0}
#define COL_Orchid					{218,112,214,false,0}
#define COL_Fuchsia					{255,0,255,false,0}
#define COL_Magenta					{255,0,255,false,0}
#define COL_MediumOrchid		{186,85,211,false,0}
#define COL_MediumPurple		{147,112,219,false,0}
#define COL_RebeccaPurple		{102,51,153,false,0}
#define COL_BlueViolet			{138,43,226,false,0}
#define COL_DarkViolet			{148,0,211,false,0}
#define COL_DarkOrchid			{153,50,204,false,0}
#define COL_DarkMagenta			{139,0,139,false,0}
#define COL_Purple					{128,0,128,false,0}
#define COL_Indigo					{75,0,130,false,0}
#define COL_SlateBlue				{106,90,205,false,0}
#define COL_DarkSlateBlue 	{72,61,139,false,0}
#define COL_MediumSlateBlue {123,104,238,false,0}

#define COL_GreenYellow				{173,255,47,false,0}
#define COL_Chartreuse				{127,255,0,false,0}
#define COL_LawnGreen					{124,252,0,false,0}
#define COL_Lime							{0,255,0,false,0}
#define COL_LimeGreen					{50,205,50,false,0}
#define COL_PaleGreen					{152,251,152,false,0}
#define COL_LightGreen				{144,238,144,false,0}
#define COL_MediumSpringGreen {0,250,154,false,0}
#define COL_SpringGreen				{0,255,127,false,0}
#define COL_MediumSeaGreen 	 	{60,179,113,false,0}
#define COL_SeaGreen					{46,139,87,false,0}
#define COL_ForestGreen				{34,139,34,false,0}
#define COL_Green							{0,128,0,false,0}
#define COL_DarkGreen					{0,100,0,false,0}
#define COL_YellowGreen				{154,205,50,false,0}
#define COL_OliveDrab					{107,142,35,false,0}
#define COL_Olive							{128,128,0,false,0}
#define COL_DarkOliveGreen 	 	{85,107,47,false,0}
#define COL_MediumAquamarine	{102,205,170,false,0}
#define COL_DarkSeaGreen			{143,188,139,false,0}
#define COL_LightSeaGreen			{32,178,170,false,0}
#define COL_DarkCyan					{0,139,139,false,0}
#define COL_Teal							{0,128,128,false,0}

#define COL_Aqua						{0,255,255,false,0}
#define COL_Cyan						{0,255,255,false,0}
#define COL_LightCyan				{224,255,255,false,0}
#define COL_PaleTurquoise	 	{175,238,238,false,0}
#define COL_Aquamarine			{127,255,212,false,0}
#define COL_Turquoise				{64,224,208,false,0}
#define COL_MediumTurquoise	{72,209,204,false,0}
#define COL_DarkTurquoise  	{0,206,209,false,0}
#define COL_CadetBlue				{95,158,160,false,0}
#define COL_SteelBlue				{70,130,180,false,0}
#define COL_LightSteelBlue 	{176,196,222,false,0}
#define COL_PowderBlue			{176,224,230,false,0}
#define COL_LightBlue				{173,216,230,false,0}
#define COL_SkyBlue					{135,206,235,false,0}
#define COL_LightSkyBlue		{135,206,250,false,0}
#define COL_DeepSkyBlue			{0,191,255,false,0}
#define COL_DodgerBlue			{30,144,255,false,0}
#define COL_CornflowerBlue 	{100,149,237,false,0}
#define COL_MediumSlateBlue	{123,104,238,false,0}
#define COL_RoyalBlue				{65,105,225,false,0}
#define COL_Blue						{0,0,255,false,0}
#define COL_MediumBlue			{0,0,205,false,0}
#define COL_DarkBlue				{0,0,139,false,0}
#define COL_Navy						{0,0,128,false,0}
#define COL_MidnightBlue		{25,25,112,false,0}

#define COL_Cornsilk				{255,248,220,false,0}
#define COL_BlanchedAlmond 	{255,235,205,false,0}
#define COL_Bisque					{255,228,196,false,0}
#define COL_NavajoWhite			{255,222,173,false,0}
#define COL_Wheat						{245,222,179,false,0}
#define COL_BurlyWood				{222,184,135,false,0}
#define COL_Tan							{210,180,140,false,0}
#define COL_RosyBrown				{188,143,143,false,0}
#define COL_SandyBrown			{244,164,96,false,0}
#define COL_Goldenrod				{218,165,32,false,0}
#define COL_DarkGoldenrod  	{184,134,11,false,0}
#define COL_Peru						{205,133,63,false,0}
#define COL_Chocolate				{210,105,30,false,0}
#define COL_SaddleBrown			{139,69,19,false,0}
#define COL_Sienna					{160,82,45,false,0}
#define COL_Brown						{165,42,42,false,0}
#define COL_Maroon					{128,0,0,false,0}

#define COL_Gainsboro				{220,220,220,false,0}
#define COL_LightGray				{211,211,211,false,0}
#define COL_Silver					{192,192,192,false,0}
#define COL_DarkGray				{169,169,169,false,0}
#define COL_Gray						{128,128,128,false,0}
#define COL_DimGray					{105,105,105,false,0}
#define COL_LightSlateGray 	{119,136,153,false,0}
#define COL_SlateGray				{112,128,144,false,0}
#define COL_DarkSlateGray  	{47,79,79,false,0}
#define COL_Black						{0,0,0,false,0}

#endif

#define ALIGN_LEFT				0
#define ALIGN_CENTER			1
#define ALIGN_RIGHT				2

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
// Définition des macros (2022)
//*****************************************************************************

#define ClearScreen EffacerEcran
#define DisplayXY		AfficherXY

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
void  GetConsoleDimensions(unsigned short*,unsigned short*);																		// Permet de récupérer les dimensions d'une console (devrait fonctionner sur Windows)

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
