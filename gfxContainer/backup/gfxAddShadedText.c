/* 
 * File:   gfxAddShadedText.c
 * Author: Dimitri "Hurukan" <soundlord@gmail.com>
 *
 * Created on 5 juin 2018, 1:04
 */

// ****************************************************************************
// HISTORIQUE
// ****************************************************************************


// ****************************************************************************
// SECTION : fichiers d'inclusions
// ****************************************************************************

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

// Retourne l'identifiant du texte inséré dans la liste chaînée...

int gfxAddShadedText(	SDL_Renderer *target,
											SDL_Color color,
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
	
	if(target==NULL) return -1;
	if(font==NULL) return -1;
	if(pContainerGFX==NULL) return -1;
	if(text==NULL) return -1;
	
	sOmbre=TTF_RenderUTF8_Blended(font,text,shadecolor);	// Du fait de l'utilisation des polices d'écran il faut générer le texte avec les glyphes...
	tOmbre=SDL_CreateTextureFromSurface(target,sOmbre);		// En SDL2 une surface seule ne suffit pas (contrairement à SDL1)...
	SDL_FreeSurface(sOmbre);		
	
	sTexte=TTF_RenderUTF8_Blended(font,text,color);
	tTexte=SDL_CreateTextureFromSurface(target,sTexte);
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
	PositionOmbre.y=PositionTexte->y+2;
	
	// C'est une technique que j'ai mis au point, les éléments graphiques sont constitué d'une liste contenant des textures dont la fonction
	// Updatescreen() est chargée d'afficher à l'écran... cette liste contient des "objets" de type t_GFXElem...
	
	gfxelem=malloc(sizeof(t_GFXElem));
	gfxelem->pTexture=tOmbre;
	gfxelem->Dimensions=PositionOmbre;
	
	lc_add((void*)gfxelem,pContainerGFX,cssmuserdef,sizeof(t_GFXElem));	// on ajoute un élément dans la liste... il sera affiché plus tard
	
	// L'ombre est dans la liste, on place le texte au-dessus, couche par couche (pensez aux calques sous Photoshop(r) )
	
	gfxelem=malloc(sizeof(t_GFXElem));
	gfxelem->pTexture=tTexte;
	gfxelem->Dimensions=*PositionTexte;
	
	int rc=lc_add((void*)gfxelem,pContainerGFX,cssmuserdef,sizeof(t_GFXElem));	
	
	return rc;
}

// ****************************************************************************
// SECTION : TODO (ce qui reste à faire)
// ****************************************************************************


