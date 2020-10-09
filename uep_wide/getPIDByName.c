/* 
 * File:   getPIDByName.c
 * Author: Padawan Master <soundlord@gmail.com>
 *
 * Created on 20 avril 2017, 14:21
 */

#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

/*
 * getPIDByName
 * 
 * Programme qui permet de récupérer le pid (numéro de processus au niveau du système UNIX)
 * d'un processus
 * 
 * Il faut passer le nom du programme que l'on recherche et la fonction va chercher
 * le processus dans le répertoire /proc (tout est fichiers dans UNIX) et retourner
 * le numéro du processus (représenté par un nom de répertoire ne contenant
 * que des chiffres).
 * 
 * Programme en provenance de seranmca (big up ^^) modifié par mes soins... 2005 - 2017
 */

//*****************************************************************************
// INPUT: le nom du programme dont nous cherchons le pid (process ID)
//
// OUTPUT: -1 le programme n'a pas été trouvé dans la table des programmes (/proc) 
//				 result (entier strictement positif différent de 0) le pid du programme
//*****************************************************************************

unsigned int getPIDByName(char *p_processname) 
{
	DIR *dir_p;
	struct dirent *dir_entry_p;
	char dir_name[FILENAME_MAX];										// ??? buffer overrun potential
	char target_name[FILENAME_MAX];									// ??? buffer overrun potential
	int target_result;
	char exe_link[FILENAME_MAX];
	//int errorcount;
	int result;

	//errorcount=0;
	result=-1;
	dir_p = opendir("/proc/"); 																// Open /proc/ directory
	while((dir_entry_p = readdir(dir_p))!=NULL) 
	{											// Reading /proc/ entries
		if (strspn(dir_entry_p->d_name, "0123456789") == strlen(dir_entry_p->d_name)) // Checking for numbered directories 
		{		
			strcpy(dir_name, "/proc/");
			strcat(dir_name, dir_entry_p->d_name);
			strcat(dir_name, "/"); 															// Obtaining the full-path eg: /proc/24657/ 
			exe_link[0] = 0;
			strcat(exe_link, dir_name);
			strcat(exe_link, "exe");													 	// Getting the full-path of that exe link (répertoire exe dans /proc)
			target_result = readlink(exe_link, target_name, sizeof(target_name)-1);			// Getting the target of the exe ie to which binary it points to
			if (target_result > 0) 
			{
				target_name[target_result] = 0;
				if (strstr(target_name, p_processname)!=NULL) // Searching for process name in the target name -- ??? could be a better search !!!
				{							
					result = atoi(dir_entry_p->d_name);
					//fprintf(stderr,"getProcessID(%s) :Found. id = %d\n", p_processname, result);
					closedir(dir_p);
					return result;
				}
			}
		}
	}
	closedir(dir_p);
	//fprintf(stderr,"getProcessID(%s) : id = 0 (could not find process)\n", p_processname);
	return result;
} 


