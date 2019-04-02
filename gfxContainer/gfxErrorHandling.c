/* 
 * File:   gfxErrorHandling.c
 * Author: Dimitri "Hurukan" <soundlord@gmail.com>
 *
 * Created on 23 juillet 2018, 18:31
 */

// ****************************************************************************
// HISTORIQUE
// ****************************************************************************


// ****************************************************************************
// SECTION : fichiers d'inclusions
// ****************************************************************************

#include <stdio.h>
#include <stdlib.h>

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
int packGFXError(int internal,int real,const char *message,const char *source)
{
	if(pGFXError==NULL) return -1;
	
	t_GFXError *objError=calloc(1,sizeof(t_GFXError));
	*objError=(t_GFXError){internal,real,message,source};
	return lc_add((void*)objError,pGFXError,cssmuserdef,sizeof(t_GFXError));
}

char* getGFXErrorMessage() 
{
	if(pGFXError==NULL) return NULL;
		
	lc_Datas *tmpDatas=lc_pop(pGFXError);
	t_GFXError *tmp=(t_GFXError*)tmpDatas->value;
	
	char *tricky=calloc(1,strlen(tmp->errormessage)+strlen(tmp->source)+5);	
	sprintf(tricky,"%s [%s]",tmp->errormessage,tmp->source);
	
	return tricky;
}

// ****************************************************************************
// SECTION : TODO (ce qui reste à faire)
// ****************************************************************************


