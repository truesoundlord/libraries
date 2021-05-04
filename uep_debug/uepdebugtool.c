/* 
 * File:   cssmdebugtool.c
 * Author: Padawan Master <soundlord@gmail.com>
 *
 * Created on 4 décembre 2016, 1:36
 */

// Spécifiques à Linux

#ifdef __LINUX__
#include <sys/stat.h>
#include <unistd.h>
#endif

// Standards

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <locale.h>
#include <errno.h>

#include "uepdebugtool.h"

//*****************************************************************************
// Variables globales
//*****************************************************************************

static int	descFichier_stdout;
static int	descFichier_stderr;

static int	oldstdout_id;
static int	oldstderr_id;

char	path[255];

int	coderetour;

short getDescriptors(int *paramstdout, int *paramstderr, char *paramFile)
{
	//*****************************************************************************
	// Obtention des descripteurs pour stdout et stderr
	//*****************************************************************************
  
  //oldstdout_id=dup(fileno(stdout));
  //oldstderr_id=dup(fileno(stderr));
	
	umask(0);
  mkdir(LOGPATH, S_IRWXO|S_IXGRP|S_IXUSR);									// avec seulement S_IRWXU cela déclenche une exception si l'utilisateur n'est pas root...
	
	// printf("[DEBUG] %04d\n",errno);
	
	if(errno!=EEXIST && errno!=ENOENT && errno!=ESPIPE && errno!=11) 				// il faut ignorer le "no such file or directory" (errno 2) et errno 11 (resource temporarily unavailable)
	{
		perror("[cssmdebugtool] mkdir() failed to create folder ");
		return ERR_IOERROR;
	}
	
	if(strlen(paramFile)>255) return ERR_PATHTOOLONG;
	
	//*****************************************************************************
	// Obtention des descripteurs pour stdout et stderr
	//*****************************************************************************
  
  oldstdout_id=dup(fileno(stdout));
  oldstderr_id=dup(fileno(stderr));
	
	sprintf(path,"%s/%s",LOGPATH,paramFile);
	
	//*****************************************************************************
	// Ouverture du fichier (stdout) en mode création (O_CREAT) 
	//*****************************************************************************
	
	descFichier_stdout=open(path,O_CREAT|O_RDWR|S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP);
	if(descFichier_stdout==-1)
	{
		if(errno!=EEXIST)
		{
			perror("[cssmdebugtool] open() stdout O_CREAT failed !!");
			return ERR_IOERROR;
		}
		
		//*****************************************************************************
		// Ouverture du fichier (stdout) en mode ajout (O_APPEND) 
		//*****************************************************************************
		
		descFichier_stdout=open(path,O_APPEND|O_RDWR|S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP);
		if(descFichier_stdout==-1)
		{
			perror("[cssmdebugtool] open() stdout O_APPEND failed !!");
			return ERR_IOERROR;
		}
	}
	
	//*****************************************************************************
	// Ouverture du fichier (stderr) en mode création (O_CREAT) 
	//*****************************************************************************
	 
  descFichier_stderr=open(path,O_APPEND|O_RDWR|S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP);
  if(descFichier_stderr==-1)
	{
		perror("[cssmdebugtool] open() stderr O_APPEND failed !!");
		return ERR_IOERROR;
	}
	
	//***************************************************************************
	// Redirection proprement dite: le descripteur 1 (stdout) est redirigé
	// vers descFichier_stdout
	//***************************************************************************
	
  coderetour=dup2(descFichier_stdout,1);
  if(coderetour==-1)
  {
    perror("[cssmdebugtool] dup2() stdout failed !!");
    return ERR_IOERROR;
  }
  
  //***************************************************************************
	// Redirection proprement dite: le descripteur 2 (stderr) est redirigé
	// vers descFichier_stderr
	//***************************************************************************

  
  coderetour=dup2(descFichier_stderr,2);
  if(coderetour==-1)
  {
    perror("[cssmdebugtool] dup2() stderr failed !!");
    return ERR_IOERROR;
  }
  
  //***************************************************************************
	// Redirection définitive: nous redigions stderr et stdout sur descFichier_stdout 
	//***************************************************************************
  
  coderetour=dup2(descFichier_stdout,descFichier_stderr);  
	if(coderetour==-1)
  {
    perror("[cssmdebugtool] dup2() redirection failed !!");
    return ERR_IOERROR;
  }

	*paramstdout=descFichier_stdout;
	*paramstderr=descFichier_stderr;
	
	return 0;
}

short rollbackDescriptors()
{
	dup2(oldstdout_id,1);
  dup2(oldstderr_id,2);
	
	close(descFichier_stdout);
	close(descFichier_stderr);
}

