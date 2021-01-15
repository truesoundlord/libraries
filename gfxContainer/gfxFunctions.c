/* 
 * File:   gfxAdd.c
 * Author: Dimitri "Hurukan" <soundlord@gmail.com>
 *
 * Created on 23 juillet 2018, 17:39
 */

// ****************************************************************************
// HISTORIQUE
// ****************************************************************************


// ****************************************************************************
// SECTION : fichiers d'inclusions
// ****************************************************************************

#include <stdio.h>
#include <stdlib.h>


#ifdef __unix__
#include <unistd.h>
#endif

#include "errorcodes.h"
#include "gfxContainer.h"

// ****************************************************************************
// SECTION : déclaration des constantes/types/structures/énumérations/...
// ****************************************************************************
// ****************************************************************************
// SECTION : déclaration des variables globales
// ****************************************************************************

// ****************************************************************************
// SECTION : implémentation des fonctions
// ****************************************************************************


// addGFX
// Ajouter un élément de type t_GFXElem dans la liste interne pContainerGFX

int addGFX(t_GFXElem *target)
{
	if(target==NULL) return -1;
	target->bEnabled=true;
	//fprintf(stderr,"[%s] initial value: %05u\n",__func__,lc_getTailID(pContainerGFX));
	target->idRelated=(int)lc_getTailID(pContainerGFX);	
	if(pContainerGFX->NbElem==0) target->idRelated=gen_Number;
	else target->idRelated++;
	return lc_add((void*)target,pContainerGFX,cssmuserdef,sizeof(t_GFXElem));
}

// gfx_setMenuItem
// Indique que cet élément dans la liste est un élément de menu

int gfx_setMenuItem(int idTarget,bool status)
{
	lc_Datas *item=lc_search(pGFXMenuItems,idTarget);
	
	if(item==NULL)
	{
		t_GFXMenuItem *new=calloc(1,sizeof(t_GFXMenuItem));
		new->idGFXElem=idTarget;
		new->bEnable=status;
		new->MenuID=gen_MenuID++;
		return lc_add((void*)new,pGFXMenuItems,cssmuserdef,sizeof(t_GFXMenuItem));
	}
	t_GFXMenuItem *old=(t_GFXMenuItem*)item->value;
	old->idGFXElem=idTarget;
	old->bEnable=status;
	old->MenuID=gen_MenuID++;
	lc_setByID((void*)old,pContainerGFX,idTarget);
	return idTarget;
}

// gfx_isMenuItem
// Indique si un élément de la liste est un élément de menu

bool gfx_isMenuItem(int idTarget)
{
	lc_Datas *comp=pGFXMenuItems->pHead;
	while(comp!=NULL)
	{
		t_GFXMenuItem *datacomp=(t_GFXMenuItem*)comp->value;
		if(datacomp->idGFXElem==idTarget) return true;							// l'identifiant est bien présent dans la liste des éléments considérés comme faisant partie d'un menu
		comp=comp->pNext;
	}
	return false;
}

// gfx_displayContent
// Affiche les éléments de la liste à l'écran

bool gfx_displayContent(void)
{
	lc_Datas *pParcours=pContainerGFX->pHead;				// La liste est lue en FIFO (first in first out), le premier entré est le premier sorti, c'est une "file d'attente"... 
	
	SDL_RenderClear(internal_R);
	
	if(pParcours==NULL) return false;
	
	
	// Parcourir la liste des éléments contenant une texture et une zone d'affichage (SDL_Rect)
	// Copier ces textures dans le "renderer" (compositeur) et effectuer la présentation des textures
	// qui s'empilent dans l'ordre d'arrivée qui doit TOUJOURS être orienté du fond vers l'avant plan.
		
	while(pParcours!=NULL)
	{
		t_GFXElem *tmpDatas=(t_GFXElem*)pParcours->value;
		if(tmpDatas->pTexture!=NULL && tmpDatas->bEnabled) // 25 mars 2018
			SDL_RenderCopy(internal_R,tmpDatas->pTexture,NULL,&tmpDatas->Dimensions);
		pParcours=pParcours->pNext;
	}
	
	SDL_RenderPresent(internal_R);
	return true;
}

// Retourne l'identifiant du texte inséré dans la liste chaînée...

int gfx_addShadedText(	SDL_Color color,
												SDL_Color shadecolor,
												TTF_Font *font,
												char *text,
												SDL_Rect *PositionTexte,
												int maxwidth,
												int maxheight,
												e_align aligned)
{
	SDL_Surface *sTexte;
	SDL_Surface *sOmbre;
	
	SDL_Texture *tTexte;
	SDL_Texture	*tOmbre;
	
	SDL_Rect PositionOmbre;
		
	t_GFXElem *gfxelem;
	t_GFXElem *gfxshadow;
	
	if(internal_R==NULL) 
	{
		packGFXError(-1,-1,"target NULL",__func__);
		return -1;
	}
	if(font==NULL) 
	{
		packGFXError(-2,-2,"font NULL",__func__);
		return -2;
	}
	if(pContainerGFX==NULL) 
	{
		packGFXError(-3,-3,"pContainerGFX NULL",__func__);
		return -3;
	}
	if(text==NULL) 
	{
		packGFXError(-4,-4,"text NULL",__func__);
		return -4;
	}
	
	sOmbre=TTF_RenderUTF8_Blended(font,text,shadecolor);			// Du fait de l'utilisation des polices d'écran il faut générer le texte avec les glyphes...
	tOmbre=SDL_CreateTextureFromSurface(internal_R,sOmbre);		// En SDL2 une surface seule ne suffit pas (contrairement à SDL1)...
	SDL_FreeSurface(sOmbre);		
	
	sTexte=TTF_RenderUTF8_Blended(font,text,color);
	tTexte=SDL_CreateTextureFromSurface(internal_R,sTexte);
	SDL_FreeSurface(sTexte);
	
	TTF_SizeUTF8(font,text,&PositionTexte->w,&PositionTexte->h);
	
	if((aligned&CENTER)==CENTER)
	{
		PositionTexte->x=(maxwidth-PositionTexte->w)/2;
	}
	if((aligned&RIGHT)==RIGHT)
	{
		PositionTexte->x=(maxwidth-PositionTexte->w);
	}
	if((aligned&LEFT)==LEFT) PositionTexte->x=0;
	
	if((aligned&VCENTER)==VCENTER)
	{
		PositionTexte->y=(maxheight-PositionTexte->h)/2;
	}
	if((aligned&VFOOTER)==VFOOTER)
	{
		PositionTexte->y=(maxheight-PositionTexte->h);
	}
	if((aligned&VHEADER)==VHEADER) PositionTexte->y=0;
	
	TTF_SizeUTF8(font,text,&PositionOmbre.w,&PositionOmbre.h);
	PositionOmbre.x=PositionTexte->x+1;
	PositionOmbre.y=PositionTexte->y+1;
	
	// C'est une technique que j'ai mis au point, les éléments graphiques sont constitué d'une liste contenant des textures dont la fonction
	// Updatescreen() est chargée d'afficher à l'écran... cette liste contient des "objets" de type t_GFXElem...
	
	gfxshadow=malloc(sizeof(t_GFXElem));
	gfxshadow->pTexture=tOmbre;
	gfxshadow->Dimensions=PositionOmbre;
	
	
	//lc_add((void*)gfxshadow,pContainerGFX,cssmuserdef,sizeof(t_GFXElem));	// on ajoute un élément dans la liste... il sera affiché plus tard
	
	addGFX(gfxshadow);
	
	// L'ombre est dans la liste, on place le texte au-dessus, couche par couche (pensez aux calques sous Photoshop(r) )
	
	gfxelem=malloc(sizeof(t_GFXElem));
	gfxelem->pTexture=tTexte;
	gfxelem->Dimensions=*PositionTexte;
	
	//int rc=lc_add((void*)gfxelem,pContainerGFX,cssmuserdef,sizeof(t_GFXElem));	
	
	return addGFX(gfxelem);
}

// gfx_addImage
// Ajoute une image dans la liste via addGFX()
// L'image est représentée par un fichier de type image (png, jpg, ...) sur le disque dur
// le paramètre "path" représente le chemin d'accès à ce fichier.

// Valeurs possibles de "direction" (pour l'effet d'ombre)
// DOWNLEFT
// UPLEFT
// UPRIGHT
// DOWNRIGHT
// NONE

int gfx_addImage(	SDL_Rect *dimensions,
									char *path,
									SDL_Color color,
									e_castshadow direction)
{
	
	t_GFXElem *gfxelem;
	t_GFXElem *gfxShadow;
	
	SDL_Surface *sImage;
	SDL_Texture *tImage;
	
	if(internal_R==NULL) 
	{
		packGFXError(-1,-1,"target NULL",__func__);
		return -1;
	}
	if(path==NULL) 
	{
		packGFXError(-2,-2,"path NULL",__func__);
		return -2;
	}
	if(pContainerGFX==NULL) 
	{
		packGFXError(-3,-3,"pContainerGFX NULL",__func__);
		return -3;
	}
	if(dimensions==NULL) 
	{
		packGFXError(-4,-4,"dimensions NULL",__func__);
		return -4;
	}	
	
	sImage=(SDL_Surface*)IMG_Load(path);
	if(sImage==NULL)
	{
		packGFXError(-5,-5,SDL_GetError(),__func__);
		return -5;
	}
	
	tImage=SDL_CreateTextureFromSurface(internal_R,sImage);
	if(tImage==NULL)
	{
		packGFXError(-6,-6,SDL_GetError(),__func__);
		return -6;
	}

	if(direction!=NONE)
	{
		SDL_Surface *sShadow=(SDL_Surface*)IMG_Load(path);
		SDL_Texture *tShadow=SDL_CreateTextureFromSurface(internal_R,sShadow);
	
		SDL_Rect posOmbre=(SDL_Rect)*dimensions;
		
		
		SDL_FreeSurface(sShadow);
	
		// Tentative d'application d'un filtre pour obtenir une "ombre" de l'image

		SDL_SetTextureColorMod(tShadow,color.r,color.g,color.b);	
		SDL_SetTextureBlendMode(tShadow,SDL_BLENDMODE_BLEND);

		gfxShadow=calloc(1,sizeof(t_GFXElem));
		gfxShadow->pTexture=tShadow;

		// bit wise !!

		if((direction&DOWNLEFT)==DOWNLEFT)
		{
			posOmbre.x-=4;
			posOmbre.y+=2;
		}

		if((direction&UPLEFT)==UPLEFT)
		{
			posOmbre.x-=4;
			posOmbre.y-=2;
		}

		if((direction&UPRIGHT)==UPRIGHT)
		{
			posOmbre.x+=4;
			posOmbre.y-=2;
		}

		if((direction&DOWNRIGHT)==DOWNRIGHT)
		{
			posOmbre.x+=4;
			posOmbre.y+=2;
		}

		gfxShadow->Dimensions=posOmbre;

		//lc_add((void*)gfxShadow,pContainerGFX,cssmuserdef,sizeof(t_GFXElem));
		addGFX(gfxShadow);
	}
	
	SDL_FreeSurface(sImage);
	
	gfxelem=calloc(1,sizeof(t_GFXElem));
	gfxelem->pTexture=tImage;
	gfxelem->Dimensions=*dimensions;
	
	//fprintf(stderr,"[%s] verify integrity (%04d,%04d,%04d,%04d)\n",__func__,gfxelem->Dimensions.x,gfxelem->Dimensions.y,gfxelem->Dimensions.w,gfxelem->Dimensions.h);
	SDL_SetTextureBlendMode(tImage,SDL_BLENDMODE_BLEND);
	
	return addGFX(gfxelem);
}

// gfx_getSelectedObject
// Récupère l'identifiant de l'objet sélectionné

int gfx_getSelectedObject(SDL_MouseButtonEvent souris)
{
	if(pContainerGFX==NULL) return -1;
	
	// Parcourir chaque élément graphique et déterminer lequel est sélectionné en fonction de la position de cet objet
	// et la position du curseur de la souris au moment du clic
	
	lc_Datas *parcours=pContainerGFX->pTail;
	while(parcours)
	{
		t_GFXElem *datas=(t_GFXElem*)parcours->value;
		
#ifdef DEBUG
		//fprintf(stderr,"[%s] integrity idRelated %05d\n",__func__,datas->idRelated);
		//fprintf(stderr,"[%s] is (%05d) selectable ? %s\n",__func__,parcours->item_Number,datas->bSelectable?"yes":"no");
#endif
		
		if(	(souris.x <= datas->Dimensions.x+datas->Dimensions.w && souris.y <= datas->Dimensions.y+datas->Dimensions.h) && 
				(souris.x >= datas->Dimensions.x && souris.y >= datas->Dimensions.y))
		{
			// Le curseur se trouve sur l'objet
			if(datas->bEnabled)
				return parcours->item_Number;
		}
		parcours=parcours->pPrevious;
	}
	return NOTFOUND;
}

// ****************************************************************************
// Tentative d'écriture d'effets
// ****************************************************************************

void gfx_fx_fade(int id,int speed)
{
	Uint8	currentalpha,oldalpha;
	int		step;

	lc_Datas *tmp=lc_search(pContainerGFX,id);
	if(tmp)
	{
		t_GFXElem *datas=(t_GFXElem*)tmp->value;
		SDL_GetTextureAlphaMod(datas->pTexture,&currentalpha);		
		oldalpha=currentalpha;
		step=currentalpha/16;
		
		while(step>0)
		{
			SDL_SetTextureAlphaMod(datas->pTexture,currentalpha-=16);
			gfx_displayContent();
			SDL_Delay(speed/5);
			step--;
		}
		
		while(step<oldalpha/16)
		{
			SDL_SetTextureAlphaMod(datas->pTexture,currentalpha+=16);
			gfx_displayContent();
			SDL_Delay(speed/5);
			step++;
		}
	}
}

// ****************************************************************************
// ...fin
// ****************************************************************************

// gfx_toggleObject
// Permet de définir si l'objet doit être "masqué" ou non...

void gfx_toggleObject(int id)
{
	lc_Datas *tmp=lc_search(pContainerGFX,id);
	if(tmp!=NULL)  // ou if(tmp) c'est pareil ;)
	{
		((t_GFXElem*)tmp->value)->bEnabled^=1;								// inverse la valeur de manière plus efficace que ce que je fais d'habitude :}
	}
}

// gfx_getWindowID
// Récupération de l'identifiant interne de la fenêtre

SDL_Window*	gfx_getWindowID(void)
{
	return internal_W;
}

// gfx_HybridStyle
// ???? je sais plus ????

int		gfx_HybridStyle(SDL_Texture *target,SDL_Rect targetdim)
{
	t_GFXElem *gfxelem;
	
	gfxelem=calloc(1,sizeof(t_GFXElem));
	gfxelem->pTexture=target;
	gfxelem->Dimensions=targetdim;
	
	return addGFX(gfxelem);
}

// ****************************************************************************
// SECTION : TODO (ce qui reste à faire)
// ****************************************************************************

/* Le flag bEnabled doit pouvoir être "toggled" (inversé) en fonction
 * de notre volonté d'afficher tous les éléments de la liste ou
 * si nous voulons sélectionner certains d'entre eux pour l'affichage
 * sans devoir les enlever de la liste
 * 
 * Idées de fonctions: gfx_toggleObject() en fournissant la liste chaînée et l'identifiant
 * 
 * De plus la fonction displayContent() doit pouvoir n'afficher que les objets dont
 * le flag bEnabled est à true (TODO)
 * 
 */

