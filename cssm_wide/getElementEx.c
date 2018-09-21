/* 
 * File:   getElementEx.c
 * Author: Dimitri "Hurukan" <soundlord@gmail.com>
 *
 * Created on 5 janvier 2018, 7:31
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

/*
 * posy<=0 tableau à une dimension
 * posz<=0 tableau à deux dimensions
 */

void* getElementEx(void *pTab,int posx,int posy,int posz,int nbcol,int nblig,int taille)
{
	if(pTab==NULL) return 0;		// perror devrait être modifié pour signaler le pointeur invalide
	if(nbcol<0) return 0; 				
	if(nblig<0) return 0;					 
	
	if(posz<0) posz=0;
	
	if(posy<0 || posy>nblig) return 0;		// perror devrait être modifié pour signaler un effet de bord
	if(posx<0 || posx>nbcol) return 0;
  
	
	/*printf("Taille d'un void (%d), d'un void* (%d), d'un int (%d), d'un float (%d)...\n",sizeof(void),sizeof(void*),sizeof(int),sizeof(float));
	
	printf("[DEMONSTRATION] pTab: %08x\n",pTab);
	printf("[DEMONSTRATION] pTab+1: %08x\n",pTab+(1*taille));
	
	printf("[DEMONSTRATION] valeur trouvée à l'adresse de pTab: %4.2f\n",*((float*)pTab));
	printf("[DEMONSTRATION] valeur trouvée à l'adresse de pTab+1: %4.2f\n",*((float*)pTab+1));*/
	
	int offset=(posz*nbcol*nblig)+(posy*nbcol)+posx;
	return (void*)pTab+(offset*taille);
}

// ****************************************************************************
// SECTION : TODO (ce qui reste à faire)
// ****************************************************************************


