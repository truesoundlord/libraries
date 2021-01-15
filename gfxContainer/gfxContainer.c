/* 
 * File:   gfxContainer.c
 * Author: Dimitri "Hurukan" <soundlord@gmail.com>
 *
 * Created on 5 juin 2018, 0:54
 */

// ****************************************************************************
// HISTORIQUE
// ****************************************************************************

// 22 décembre 2018: refonte des fonctions en considérant que le Renderer* utilisé
// dans quelques fonctions est "caché" dans la librairie, il n'a pas besoin
// d'être fourni d'autant qu'il ne peut y en avoir qu'un seul de défini



// ****************************************************************************
// SECTION : fichiers d'inclusions
// ****************************************************************************

#include <stdio.h>
#include <stdlib.h>

#include "gfxContainer.h"

#ifndef ERRORCODES_H
#include "errorcodes.h"
#endif

// ****************************************************************************
// SECTION : déclaration des constantes/types/structures/énumérations/...
// ****************************************************************************
// ****************************************************************************
// SECTION : déclaration des variables globales
// ****************************************************************************

LinkedList *pContainerGFX;
LinkedList *pGFXError;
LinkedList *pGFXMenuItems;

SDL_Renderer	*internal_R;
SDL_Window		*internal_W;

// ****************************************************************************
// SECTION : implémentation des fonctions
// ****************************************************************************
LinkedList* InitGFXContainer(void)
{
	pContainerGFX=lc_init();
	pGFXError=lc_init();
	pGFXMenuItems=lc_init();
	return pContainerGFX;
}

long freeGFXContainer(void)
{
	lc_empty(pGFXError);
	lc_empty(pGFXMenuItems);
	return lc_empty(pContainerGFX);
}

/* InitSDL2Framework
 * 
 * Fonction qui met en place tout ce qui est nécessaire pour afficher LA
 * "fenêtre SDL2", à savoir un SDL_Renderer pour afficher les éléments
 * et SDL_Window pour la gestion de la fenêtre
 */

SDL_Renderer*	gfx_InitSDL2Framework(Uint32 flags,int width,int height,char *title,Uint32 renderflags,Uint32 createflags)
{
	int returncode;
	
	
	// Premièrement initialiser la librairie SDL2
	returncode=SDL_Init(flags);
	if(returncode!=0)
	{
		// Mal parti ^^
		packGFXError(INITERR,-1,SDL_GetError(),__func__);
		return NULL;
	}
	
	// Initialisation de la "couche" TTF
	
	if(TTF_Init()!=0)
	{
		packGFXError(TTFERR,-1,TTF_GetError(),__func__);
		return NULL;
	}
	
	// Obtention de la fenêtre au sens SDL2 du terme
	
	internal_W=gfx_CreateMainWindow(title,width,height,createflags);				// Création de la fenêtre	
	if(!internal_W)
	{
		packGFXError(WINERR,-1,SDL_GetError(),__func__);
		return NULL;
	}
	
	//SDL_SetWindowPosition(internal_W,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED);
	
	// Obtention du "compositeur"
	
	internal_R=SDL_CreateRenderer(internal_W,-1,renderflags);
	if(internal_R==NULL)
	{
		packGFXError(RENDERR,-1,SDL_GetError(),__func__);
		return NULL;
	}
	internal_WinConf.renderconf=renderflags;
	internal_WinConf.winshow=createflags;
	
	return internal_R; 
}

SDL_Window*	gfx_CreateMainWindow(char *Titre,int SCREEN_WIDTH,int SCREEN_HEIGHT,Uint32 createflags)
{
	if(Titre==NULL) return NULL;
	if(!strlen(Titre)) strcpy(Titre,"sans titre");
	
	
	
	return SDL_CreateWindow(Titre,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,SCREEN_WIDTH,SCREEN_HEIGHT,createflags);
}

void QuitGFX(void)
{
	freeGFXContainer();
	TTF_Quit();
	SDL_DestroyWindow(internal_W);
	SDL_Quit();
}

// ****************************************************************************
// SECTION : TODO (ce qui reste à faire)
// ****************************************************************************


