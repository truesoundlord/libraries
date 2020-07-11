/* 
 * File:   setElementEx.c
 * Author: Dimitri "Hurukan" <soundlord@gmail.com>
 *
 * Created on 5 janvier 2018, 7:32
 */

// ****************************************************************************
// HISTORIQUE
// ****************************************************************************


// ****************************************************************************
// SECTION : fichiers d'inclusions
// ****************************************************************************

#include <stdio.h>
#include <stdlib.h>

// ****************************************************************************
// SECTION : déclaration des constantes/types/structures/énumérations/...
// ****************************************************************************
// ****************************************************************************
// SECTION : déclaration des variables globales
// ****************************************************************************

// ****************************************************************************
// SECTION : implémentation des fonctions
// ****************************************************************************
int setElementEx(void *pTab,int posx,int posy,int posz,int nbcol,int nblig,int taille,void *datas)
{
	if(pTab==NULL) return -1;
	if(nbcol<0) return -1; 								// perror devrait être modifié
	if(nblig<0) return -1; 								
	if(posz<0) posz=0;								
	if(posy<0 || posy>nblig) return -1;		// détecter un effet de bord
	if(posx<0 || posx>nbcol) return -1;		
	
	int offset=(posz*nbcol*nblig)+(posy*nbcol)+posx;
	pTab+=(offset*taille);																													// on effectue le déplacement
	*((long*)pTab)=*((long*)datas);																									// on assigne une valeur sur une adresse de 64bits
	pTab-=(offset*taille);																													// on revient à la position de départ
	return 0;
}

// ****************************************************************************
// SECTION : TODO (ce qui reste à faire)
// ****************************************************************************


