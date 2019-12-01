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

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>

#ifndef __LINKEDLIST__
#include <linkedlist.h>
#endif

// **************************************************************************** 
// Définitions des constantes symboliques
// **************************************************************************** 

// ****************************************************************************
// SECTION : variables externes/globales/...
// ****************************************************************************

static LinkedList *pContainerGFX;
static LinkedList *pGFXError;

// **************************************************************************** 
// Définition(s) des structures, types et énumérations
// **************************************************************************** 

typedef struct s_GFXElem
{
	SDL_Texture *pTexture;
	SDL_Rect	  Dimensions;

	int					idRelated;								// identifiant de la liste chaînée
}t_GFXElem;

typedef enum {NONE=0,LEFT=2,CENTER=4,RIGHT=8,VCENTER=16,VHEADER=32,VFOOTER=64}e_align;

// **************************************************************************** 
// Définition(s) des fonctions
// **************************************************************************** 

LinkedList* InitGFXContainer(void);					// Initialiser le container

int		addGFX(t_GFXElem*);
int		gfxAddShadedText(	SDL_Renderer*,																										
												SDL_Color,																												
												SDL_Color,																												
												TTF_Font*,																												
												char*,																														
												SDL_Rect*,																												
												int,																															
												int,																															
												e_align);					/* "compositeur" graphique cible 
																						couleur de l'élément 
																						couleur de son ombre 
																						type de "police" (/usr/share/fonts/truetype)
																						texte à "renderiser"
																						position en x,y et largeur hauteur de la zone
																						largeur maximum (de la fenêtre)
																						hauteur maximum (de la fenêtre) [je pense que nous pouvons l'obtenir via la librairie SDL2] 
																						alignement (voir e_align)
																					*/


void	displayContent(SDL_Renderer*);
long	freeGFXContainer(void);

char* getGFXError(int);										/* Le "int" est un identifiant */

#endif /* GFXCONTAINER_H */

