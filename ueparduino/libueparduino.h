/* 
 * File:   libcssmarduino.h
 * Author: Dimitri "Hurukan" <soundlord@gmail.com>
 *
 * Créé le October 17, 2019, 10:17 PM
 *  
 */

#ifndef LIBUEPARDUINO_H
#define LIBUEPARDUINO_H

// ****************************************************************************
// SECTION : fichiers d'inclusions
// ****************************************************************************

#include <termios.h>
#include <stdio.h>
#include <stdbool.h>

// **************************************************************************** 
// Définition(s) des variables statiques/externes
// **************************************************************************** 

// **************************************************************************** 
// Définitions des constantes symboliques
// **************************************************************************** 

// **************************************************************************** 
// Définition(s) des structures, types et énumérations
// ****************************************************************************

typedef struct s_RGB
{
	short int Red;
	short int Green;
	short int Blue;
}t_RGB;

// Basé sur l'Arduino UNO R3

typedef struct s_ArduinoDatas
{
	// D'après le schéma du ATMEGA328
	
	// Les ports PWM () sont les ports 3,5,6 (portd) 9,10,11 (portb)
	// Pour les transmissions réseau	le RX se trouve en portd[0] (0)
	//																le TX se trouve en portd[1] (1)
	
	// INT0 se trouve sur portd[2]
	// INT1						sur portd[3]
 	
	// SPI
	//	SCK		--> portb[5] (13)
	//	MISO	--> portb[4] (12)
	//	MOSI	--> portb[3] (11)
	//	SS		--> portb[2] (10)
		
	char	portb[6];							// digital (8 à 13)
	float portc[6];							// analog  (A0 à A5) RESET -> portc[5] 
	char	portd[8];							// digital (0 à 7) 		
	
	t_RGB	Color;
	unsigned short szMessage;		// taille du message
	char *pMessage;							// message éventuel
	
}t_ArduinoDatas;

typedef enum{DIGD0=0,DIGD1,DIGD2,DIGD3,DIGD4,DIGD5,DIGD6,DIGD7,DIGB8=0,DIGB9,DIGB10,DIGB11,DIGB12,DIGB13}e_UNOR3_DIG;
typedef enum{ALGC0=0,ALGC1,ALGC2,ALGC3,ALGC4,RESET}e_UNOR3_ANLG;


// **************************************************************************** 
// Définition(s) des fonctions
// ****************************************************************************

bool	isArduinoDetected(char*);

int		ConfigureArduino(char*,speed_t);
int	SndMessage(int,void*,size_t);		

void UnlinkArduino(int);
void*	RcvMessage(int,size_t);

bool PackMessage(char*,t_ArduinoDatas*);
bool ProtocolSetArduinoStatus(int,t_ArduinoDatas*);

t_ArduinoDatas*	ProtocolGetArduinoStatus(int);
int setRTS(int,bool);
int setDTR(int,bool);

#endif /* LIBCSSMARDUINO_H */

