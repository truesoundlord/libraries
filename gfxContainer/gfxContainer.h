/* 
 * File:   gfxContainer.h
 * Author: Dimitri "Hurukan" <soundlord@gmail.com>
 *
 * Created on 5 juin 2018, 0:54
 */

#ifndef GFXCONTAINER_H
#define GFXCONTAINER_H

// ****************************************************************************
// SECTION : fichiers d'inclusions
// ****************************************************************************

#include <stdbool.h>

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#ifndef __LINKEDLIST__
#include <linkedlist.h>
#endif

#ifndef ERRORCODES_H
#include "errorcodes.h"
#endif

// **************************************************************************** 
// Définitions des constantes symboliques
// **************************************************************************** 

// ****************************************************************************
// SECTION : variables externes/globales/...
// ****************************************************************************

extern LinkedList *pContainerGFX;																	
extern LinkedList *pGFXError;																		// afin de pouvoir récupérer des erreurs générées lors de l'utilisation de GFXContainer ou GFXMenuItems

extern LinkedList *pGFXMenuItems;																// en vue d'utiliser une fonction générale pour l'affichage et la gestion des menus

extern SDL_Renderer *internal_R;																// renderer interne
extern SDL_Window		*internal_W;																// windows interne	

static unsigned int gen_MenuID=4000;                            // pour pouvoir différencier un élément de la liste par rapport à un autre

// **************************************************************************** 
// Définition(s) des structures, types et énumérations
// **************************************************************************** 

typedef LinkedList GFXContainer;																							// alias

typedef struct s_GFXElem
{
	SDL_Texture *pTexture;
	SDL_Rect	  Dimensions;

	bool				bEnabled;									// indique si l'objet peut être affiché/sélectionné ou non...		 (par défaut true)					
	
	unsigned int	idRelated;								// identifiant de la liste chaînée
}t_GFXElem;

typedef struct s_GFXError
{
	int internalcode;
	int realcode;
	
	const char *errormessage;
	const char *source;
}t_GFXError;

typedef struct s_GFXConfig
{
	Uint32 renderconf;												/* configuration du compositeur */
	//Uint32 winhorpos;													/* position horizontale de la fenêtre */
	//Uint32 winverpos;													/* position verticale de la fenêtre */
	Uint32 winshow;														/* flag concernant la manière d'afficher la fenêtre */
}t_GFXConfig;

typedef struct s_GFXMenuItem
{
	unsigned int	idGFXElem;									// identifiant de l'élément graphique associé à cet objet
	unsigned int	MenuID;										// identifiant dissociant deux menus
	unsigned int	ParentID;									// identifiant du menu "parent" 	
	bool	bEnable;														// indique si l'élément du menu est activé ou non
}t_GFXMenuItem;

typedef enum {NONE=0,LEFT=2,CENTER=4,RIGHT=8,VCENTER=16,VHEADER=32,VFOOTER=64}e_align;
typedef enum {DOWNLEFT=2,DOWNRIGHT=4,UPLEFT=8,UPRIGHT=16}e_castshadow;

static t_GFXConfig	internal_WinConf;																															/* Fenêtre de l'application */

// **************************************************************************** 
// Définition(s) des fonctions
// **************************************************************************** 

LinkedList* InitGFXContainer(void);					// Initialiser le container

int		addGFX(t_GFXElem*);

int		gfx_HybridStyle(SDL_Texture*,SDL_Rect);

int		gfx_setMenuItem(int,bool);								// int: identifiant de l'objet bool: état (false: non sélectionné, true: sélectionné)  
int		gfx_addShadedText(	SDL_Color,																												
												SDL_Color,																												
												TTF_Font*,																												
												char*,																														
												SDL_Rect*,																												
												int,																															
												int,																															
												e_align);					/*	couleur de l'élément 
																							couleur de son ombre 
																							type de "police" (/usr/share/fonts/truetype)
																							texte à "renderiser"
																							position en x,y et largeur hauteur de la zone
																							largeur maximum (de la fenêtre)
																							hauteur maximum (de la fenêtre) [je pense que nous pouvons l'obtenir via la librairie SDL2] 
																							alignement (voir e_align)
																					*/

int gfx_addImage(	SDL_Rect*,
									char*,
									SDL_Color,
									e_castshadow);							/*	dimensions (x,y,w,h) de l'image
																									chemin de l'image
																									couleur de l'ombre
																							*/		


bool	gfx_displayContent(void);										// le renderer est interne ("caché" dans la librairie)
bool  gfx_isMenuItem(int);												// int: identifiant d'un objet dont on veut savoir si il fait partie d'un menu

long	freeGFXContainer(void);

void	QuitGFX(void);

int   packGFXError(int,int,const char*,const char*);								/* int: code interne, int: code réel, char* message, char* source */	
int		gfx_getSelectedObject(SDL_MouseButtonEvent);

void  gfx_fx_fade(int,int);												
//void  gfx_fx_rotate(int,double,SDL_Rect);

void	gfx_toggleObject(int);

char* getGFXErrorMessage();																		

SDL_Renderer*	gfx_InitSDL2Framework(Uint32,int,int,char*,Uint32,Uint32);								
SDL_Window*		gfx_CreateMainWindow(char*,int,int,Uint32);

SDL_Window*	gfx_getWindowID(void);




#endif /* GFXCONTAINER_H */

