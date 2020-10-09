/* 
 * File:   cssmdebugtool.h
 * Author: Padawan Master <soundlord@gmail.com>
 *
 * Created on 4 décembre 2016, 1:43
 */
 
#ifndef UEPDEBUGTOOL_H
#define UEPDEBUGTOOL_H

//*****************************************************************************
// Définition des constantes
//*****************************************************************************

#ifdef __LINUX__
#define LOGPATH	"./logs"
#endif

#define ERR_IOERROR		-1
#define ERR_PATHTOOLONG -2

// Définitions des fonctions

short getDescriptors(int*,int*,char*);	/*
                                         * input: descripteur de stdout (int*)
                                         *        descripteur de stderr (int*)
                                         *        fichier à utiliser pour les redirections (char*)
                                         * 
																				 * la fonction attribue un nouveau descripteur après
																				 * avoir redirigé les sorties standards...
																				 */

short rollbackDescriptors();             /*
																				 * la fonction restitue les descripteurs standards...
																				 */

                                                                                                                                                                 

#endif /* CSSMDEBUGTOOL_H */

