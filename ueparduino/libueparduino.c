/* 
 * File:   libcssmarduino.c
 * Author: Dimitri "Hurukan" <soundlord@gmail.com>
 *
 * Créé le October 17, 2019, 10:16 PM
 */

// ****************************************************************************
// HISTORIQUE
// ****************************************************************************

// 17 octobre 2019
// Ajout des fonctions 
// isArduinoConnected (en rapport avec le projet gtkarduino)
// ConfigureArduino (idem)
// RcvMessage
// SndMessage
// PackMessage

// Définition de la structure t_ArduinoDatas
// Définition des énumérations pour faciliter l'accès aux données...

// ****************************************************************************
// SECTION : fichiers d'inclusions
// ****************************************************************************

#include <stdio.h>
#include <stdlib.h>

#include <sys/ioctl.h>
#include <termios.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>



#include "libueparduino.h"

// ****************************************************************************
// SECTION : déclaration des constantes/types/structures/énumérations/...
// ****************************************************************************
// ****************************************************************************
// SECTION : déclaration des variables globales/externes
// ****************************************************************************

// ****************************************************************************
// SECTION : implémentation des fonctions
// ****************************************************************************

bool isArduinoDetected(char *pFichier)
{
	if(access(pFichier,F_OK)!=-1) return true;
	else return false;
}

// Les vitesses acceptées sont, pour les périphériques séries traditionnels

// B38400
// B57600
// B115200 <-- 
// B230400
// B460800

int ConfigureArduino(char *pFichier,speed_t vitesse)
{
	int portdesc;							// descripteur de l'Arduino
	struct termios options;		// caractéristiques d'un terminal
	
	portdesc=open(pFichier, O_RDWR | O_NOCTTY | O_SYNC);											// Ni la lecture ni l'écriture ne seront bloquantes
	if(portdesc>0)	
	{
		fcntl(portdesc,F_SETFL,0);		// réinitialiser le port série
		// Il faut que les deux équipements fonctionnent à la même "vitesse"...

		tcgetattr(portdesc, &options);
		//cfsetispeed(&options, vitesse);
		cfsetospeed(&options, vitesse);
		
		//options.c_cflag |= (CLOCAL | CREAD);    /* ignore modem controls */
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;         /* 8-bit characters */
    options.c_cflag &= ~PARENB;     /* no parity bit */
    options.c_cflag &= ~CSTOPB;     /* only need 1 stop bit */
    //options.c_cflag &= ~CRTSCTS;    /* no hardware flowcontrol */

    /* setup for non-canonical mode */
    //options.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
    options.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
    options.c_oflag &= ~OPOST;

    /* fetch bytes as they become available */
    options.c_cc[VMIN] = 1;
    options.c_cc[VTIME] = 1;
		
		tcsetattr(portdesc, TCSANOW, &options);
		


	}
	return portdesc;
}

void UnlinkArduino(int portdesc)
{
	if(portdesc>0)
	{
		fcntl(portdesc,F_SETFL,0);		// réinitialiser le port série (normalement il ne devrait plus rester de "crasses" dans le "buffer")
		tcflush(portdesc,TCIOFLUSH);
	}
	close(portdesc);
}

void*	RcvMessage(int descripteur,size_t szBloc)
{
	char	*pMessage=calloc(szBloc,sizeof(char));
	int		errcode=read(descripteur,pMessage,szBloc);
	if(errcode>0)
	{
		return (void*)pMessage;
	}
	else return NULL;
}

int SndMessage(int descripteur,void *pBuffer,size_t szBloc)
{
	if(descripteur<=0 || pBuffer==NULL) return -1;
	return write(descripteur,pBuffer,szBloc);
}

bool PackMessage(char *source, t_ArduinoDatas *target)
{
	if(source!=NULL)
	{
		target->pMessage=calloc(strlen(source),sizeof(char));
		if(target->pMessage==NULL) return false;
		strcpy(target->pMessage,source);
		target->szMessage=strlen(source);
	}
	else return false;
	return true;
}

// Le protocole consiste à d'abord envoyer tous les octets digitaux
// puis tous les floats puis la taille du message texte puis le message texte
// puis les données additionnelles éventuelles

t_ArduinoDatas* ProtocolGetArduinoStatus(int descripteur)
{
	
}

bool	ProtocolSetArduinoStatus(int descripteur,t_ArduinoDatas *source)
{
	
}


int setRTS(int portdesc, bool level)
{
		int Terminal_flag;
		if(level)
		{
			Terminal_flag |= (TIOCM_RTS); 
		}
		else
		 Terminal_flag &= ~(TIOCM_RTS);
	
		return ioctl(portdesc,TIOCMBIS,&Terminal_flag);
}

int setDTR(int portdesc, bool level)
{
		int Terminal_flag;
		if(level)
		{
			Terminal_flag |= (TIOCM_DTR); 
		}
		else
		 Terminal_flag &= ~(TIOCM_DTR);
	
		return ioctl(portdesc,TIOCMBIS,&Terminal_flag);
}

// ****************************************************************************
// SECTION : TODO (ce qui reste à faire)
// ****************************************************************************

// Vérifier si Windows respecte la norme POSIX 2.1 (unistd.h)



