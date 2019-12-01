/* 
 * File:   dosnapshot.c
 * Author: Dimitri "Hurukan" <soundlord@gmail.com>
 *
 * Created on 26 mars 2018, 14:34
 */

// ****************************************************************************
// HISTORIQUE
// ****************************************************************************

// ****************************************************************************
// SECTION : fichiers d'inclusions
// ****************************************************************************

#include <stdio.h>
#include <time.h>
#include <SDL2/SDL_image.h>


// ****************************************************************************
// SECTION : déclaration des constantes/types/structures/énumérations/...
// ****************************************************************************

#ifndef SNAPDIR
#define SNAPDIR "./snapshots"
#endif

#ifndef FILENAME_MAX
#define FILENAME_MAX 255
#endif

// ****************************************************************************
// SECTION : déclaration des variables globales
// ****************************************************************************

// ****************************************************************************
// SECTION : implémentation des fonctions
// ****************************************************************************

void dosnapshot(SDL_Renderer *target)
{
	int screenHeight;
	int screenWidth;
	int returncode;
	
	SDL_Surface *s_SnapSource;
	
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		Uint32 rmask = 0xff000000;
		Uint32 gmask = 0x00ff0000;
		Uint32 bmask = 0x0000ff00;
		Uint32 amask = 0x000000ff;  
#else
	  Uint32 rmask = 0x000000ff;
		Uint32 gmask = 0x0000ff00;
		Uint32 bmask = 0x00ff0000;
		Uint32 amask = 0xff000000;
#endif
		
	if(target!=NULL)
	{
		returncode=SDL_GetRendererOutputSize(target,&screenWidth,&screenHeight);
		if(returncode==-1)
		{
			fprintf(stderr,"[%s] %s\n",__func__,SDL_GetError());
			return;
		}
		s_SnapSource=SDL_CreateRGBSurface(0,screenWidth,screenHeight,32,rmask,gmask,bmask,amask);
		if(s_SnapSource==NULL)
		{
			fprintf(stderr,"[%s] %s\n",__func__,SDL_GetError());
			return;
		}
		SDL_LockSurface(s_SnapSource);
		SDL_RenderReadPixels(target,NULL,s_SnapSource->format->format,s_SnapSource->pixels,s_SnapSource->pitch);
		
		char NomFichier[FILENAME_MAX];
		time_t		epoch;
		struct tm *date;
		
		epoch=time(NULL);
		date=gmtime(&epoch);
		sprintf(NomFichier,"./%s/mmry_snap%02d%02d_%08ld.png",SNAPDIR,date->tm_mday,date->tm_yday,clock());
		
		umask(0);
		mkdir(SNAPDIR,S_IRWXO);
				
		SDL_SaveBMP(s_SnapSource,NomFichier);
		SDL_UnlockSurface(s_SnapSource);
		SDL_FreeSurface(s_SnapSource);
	}									
}

// ****************************************************************************
// SECTION : TODO (ce qui reste à faire)
// ****************************************************************************


