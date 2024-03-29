#ifndef __LINKEDLIST__
#include "linkedlist.h"
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <regex.h>
#endif

//*****************************************************************************
// HISTORIQUE perdu avant 2020 :{

// juillet 2020
// Je vais avoir des soucis pour mes fonctions de recherche si le nombre d'éléments est
// élevé...

// Il y a un soucis avec les variables globales qui pourraient être modifiées
// dans le cadre des threads... <TODO: essayer de "locker">


//*****************************************************************************

//*****************************************************************************
// IMPLEMENTATIONS
//*****************************************************************************

//*****************************************************************************
// lc_get
//
// ENTREE(S):
//
// 	pListe: 		pointeur sur un élément de type ListeChainee
// 	position: 	indice de l'élément que l'on veut "extraire" (base index = 0)
//
// SORTIE:
//	
//	un pointeur sur une donnée de type lc_Datas.
//
//*****************************************************************************

// En cas de présence de TRES nombreux éléments dans la liste
// EXEMPLE:

/* 
 * Searching for 81871                                                                                                                                                
       [FOUND] 1800604738                                                                                                                                             
       [PERF 0] 0,001047 second                                                                                                                                       
Searching for 05143                                                                                                                                                   
       [FOUND] 231994623                                                                                                                                              
       [PERF 1] 0,000050 second                                                                                                                                       
Searching for 08558                                                                                                                                                   
       [FOUND] 1962308680                                                                                                                                             
       [PERF 2] 0,000073 second                                                                                                                                       
Searching for 17589                                                                                                                                                   
       [FOUND] 1081889810                                                                                                                                             
       [PERF 3] 0,000147 second                                                                                                                                       
Searching for 79626                                                                                                                                                   
       [FOUND] 651197885                                                                                                                                              
       [PERF 4] 0,000741 second
*/

// Il serait préférable de commencer à chercher par la fin pTail
// au lieu de pHead !!!

// Juiller 2020: tentative de recherche en fonction de l'item
// TODO : exécuter un thread et collecter serait peut-être bénéfique

static unsigned int gen_Number=1000;                            // pour pouvoir différencier un élément de la liste par rapport à un autre

lc_Datas* lc_get(ListeChainee *pListe,int position) 
{
	int cptInternal=0;
  if(position<=pListe->NbElem/2)
	{
		if(pListe->pHead!=NULL)
		{	
			lc_Datas *lc_parcours=pListe->pHead;
			while(lc_parcours!=NULL)
			{
				if(cptInternal==position) return lc_parcours;
				lc_parcours=lc_parcours->pNext;
				cptInternal++;
			}
		}
	}
	else
	{
		return lc_lastget(pListe,position);
	}
	return (lc_Datas*)NULL;
}

//*****************************************************************************
// lc_lastget (lc_get mais à partir de la fin pTail au lieu du début pHead)
//
// ENTREE(S):
//
// 	pListe: 		pointeur sur un élément de type ListeChainee
// 	position: 	indice de l'élément que l'on veut "extraire" (base index 0)
//
// SORTIE:
//	
//	un pointeur sur une donnée de type lc_Datas.
//
//*****************************************************************************

lc_Datas* lc_lastget(ListeChainee *pListe,int position) 
{
	int cptInternal=pListe->NbElem-1; // juillet 2020
	if(pListe->pTail!=NULL)
	{
		lc_Datas *lc_parcours=pListe->pTail;
		while(lc_parcours!=NULL)
		{
			if(cptInternal==position) return lc_parcours;
			lc_parcours=lc_parcours->pPrevious;
			cptInternal--;
		}
	}
	return (lc_Datas*)NULL;
}


//*****************************************************************************
// lc_search
//
// ENTREE(S):
//
// 	pListe: 		pointeur sur un élément de type ListeChainee
// 	itemid: 		numéro d'identifiant de l'élément que l'on veut "extraire"
//
// SORTIE:
//	
//	un pointeur sur une donnée de type lc_Datas.
//
//*****************************************************************************

// Ici en matière de performances la recherche séquentielle n'est pas
// du tout efficace :{

// EXEMPLE

/* Searching for 81871           
┃       [FOUND] 1800604738       
┃       [PERF 0] 0,001047 second
*/

// Je n'ai rien en place pour améliorer celle-ci, une map 
// pourrait être mise en place (position,itemid)
// mais un tel tableau doit être gardé en mémoire :{

// Comme je connais la base des identifiants je peux définir
// par rapport au nombre d'éléments dans le tableau 
// à quel "endroit" de celui-ci j'ai plus de chances de
// chopper l'élément correspondant...

lc_Datas* lc_search(ListeChainee *pListe,int itemid) 
{
	if(itemid-1000<=pListe->NbElem/2)
	{
		if(pListe->pHead!=NULL)
		{
			lc_Datas *lc_parcours=pListe->pHead;
			while(lc_parcours!=NULL)
			{
				if(lc_parcours->item_Number==itemid) return lc_parcours;
				lc_parcours=lc_parcours->pNext;
			}
		}
	}
	else
	{
		if(pListe->pTail!=NULL)
		{		
			lc_Datas *lc_parcours=pListe->pTail;
			while(lc_parcours!=NULL)
			{
				if(lc_parcours->item_Number==itemid) return lc_parcours;
				lc_parcours=lc_parcours->pPrevious;
			}
		}	
	}
	return (lc_Datas*)NULL;
}

//*****************************************************************************
// lc_insert (LIFO: Last In First Out) [ajoût en tête de liste]
//
// ENTREE(S):
//
//	pData: 			pointeur sur un élément de type void
// 	pListe: 		pointeur sur un élément de type ListeChainee
// 	type: 			représente le type de la donnée ajoutée
//	taille 			représente la taille de la donnée ajoutée (en bytes)
//
// SORTIE:
//	
//	-1: en cas d'erreur
//	Le numéro d'enregistrement de l'élément inséré
//*****************************************************************************

int lc_insert(void *pData,ListeChainee *pListe,short type,short taille) 
{
	//lc_Datas *lc_new=(lc_Datas*)malloc(sizeof(lc_Datas));
	lc_Datas *lc_new=calloc(1,sizeof(lc_Datas));
	lc_Datas *seekLast;
	
	if(lc_new==NULL) return -1;							// modifié en juillet 2016
	
		// Avril 2018 
	
	lc_new->pFree=NULL;
	lc_new->pDisplay=NULL;
  
  // l'insert se fera toujours en tête...
  if(pListe->pHead!=NULL)
  {
		// Il y a un élément dans la liste
	
	
		pListe->pHead->pPrevious=lc_new;
	
		lc_new->pPrevious=NULL;
		lc_new->pNext=pListe->pHead;
		lc_new->dataSize=taille;
		lc_new->dataType=type;
		//lc_new->value=(void*)malloc(taille); dec2017
		lc_new->item_Number=gen_Number++;
		lc_new->value=pData;
		pListe->NbElem++;
		pListe->pHead=lc_new;

		// il faut pouvoir mettre à jour pTail qui devrait pointer sur le dernier élément de la liste... 
	
		seekLast=pListe->pHead;
		while(seekLast!=NULL)
		{
			if(seekLast->pNext==NULL) pListe->pTail=seekLast;
			seekLast=seekLast->pNext;
		}
  }
  else
  {
		// La liste est vide
		lc_new->pPrevious=NULL;
		lc_new->pNext=NULL;
		lc_new->dataSize=taille;
		lc_new->dataType=type;
		//lc_new->value=(void*)malloc(taille);  dec2017
		lc_new->value=pData;
		lc_new->item_Number=gen_Number++;
		pListe->NbElem++;
		pListe->pHead=lc_new;
		pListe->pTail=lc_new;
  }
	return lc_new->item_Number;
}

//*****************************************************************************
// lc_add (ajout en fin de liste)
//
//	pData: 			pointeur sur un élément de type void
// 	pListe: 		pointeur sur un élément de type ListeChainee
// 	type: 			représente le type de la donnée ajoutée
//	taille 			représente la taille de la donnée ajoutée (en bytes)
//
// SORTIE:
//	
//	-1: en cas d'erreur
//	Le numéro d'enregistrement du nouvel élément
//*****************************************************************************


int lc_add(void *pData,ListeChainee *pListe,short type,short taille)
{
  //lc_Datas *lc_new=(lc_Datas*)malloc(sizeof(lc_Datas));
	lc_Datas *lc_new=calloc(1,sizeof(lc_Datas));
	lc_Datas *seekFirst=NULL; // dec2017 puis juillet 2020
  if(lc_new==NULL || pListe==NULL) return -1; // juillet 2020
	
	// Avril 2018 
	
	lc_new->pFree=NULL;
	lc_new->pDisplay=NULL;
	
	// l'ajout se fera toujours en queue...
	
  if(pListe->pTail!=NULL)
  {
		// Il y a un élément dans la liste
		
		lc_new->dataSize=taille;
		lc_new->dataType=type;
		//lc_new->value=(void*)malloc(taille); dec2017
		lc_new->value=pData;
		lc_new->item_Number=gen_Number++;
		pListe->NbElem++;
		
		// chainage
		// le dernier élément ne pointe plus sur NULL mais sur le nouvel élément créé
		
		lc_new->pNext=NULL;
		lc_new->pPrevious=pListe->pTail; 	// indiquer que l'élément qui précède celui-ci était pointé par pTail
		pListe->pTail->pNext=lc_new; 			// indiquer que l'élément qui suit le dernier élément est maintenant le nouvel élément
		
		pListe->pTail=lc_new;							// indiquer que le dernier élément inséré est celui qui est inséré maintenant 
		
		// mettre à jour le pointeur pHead (tête de la liste)
		
		seekFirst=pListe->pTail;
		while(seekFirst!=NULL)
		{
			if(seekFirst->pPrevious==NULL) pListe->pHead=seekFirst;
			seekFirst=seekFirst->pPrevious;
		}
  }
  else
  {
		// La liste est vide
		lc_new->pPrevious=NULL;
		lc_new->pNext=NULL;
		lc_new->dataSize=taille;
		lc_new->dataType=type;
		//lc_new->value=(void*)malloc(taille); dec2017
		lc_new->value=pData;
		lc_new->item_Number=gen_Number++;
		pListe->NbElem++;
		pListe->pHead=lc_new;
		pListe->pTail=lc_new;
  }
	return lc_new->item_Number; 
}

//*****************************************************************************
// lc_delete (effacer un élément de la liste)
//
// ENTREE(S):
//	pListe: 	pointeur sur un élément de type ListeChainee
//	target:		l'identifiant d'un élément
//
// SORTIE:
//	x: 	effacement en milieu de chaine (on récupère l'id de l'élément précédent)
//	1: 	effacement en début de chaine
//  2:  effacement en fin de chaine
//  3: 	plus aucun élément (chaine vide)  
//	-1: une erreur est survenue
//
//*****************************************************************************

int lc_delete(ListeChainee *pListe,int targetID)
{
	// février 2017
	int elementprecedent=-1;
	
	lc_Datas *lc_parcours; // =(lc_Datas*)malloc(sizeof(lc_Datas)); dec2017
	if(pListe->pHead!=NULL)
	{
		lc_parcours=pListe->pHead;
		
		while(lc_parcours!=NULL)
		{
#ifdef DEBUG
			printf("\n[DEBUG %s]\nlc_parcours:\t\t%08x\n",__func__,lc_parcours,lc_parcours->item_Number);
			printf("\tpPrevious:\t%08x\n",lc_parcours->pPrevious);
			printf("\tpNext:\t\t%08x\n",lc_parcours->pNext);
#endif
			if(lc_parcours->item_Number==targetID)
			{
				if(lc_parcours->pPrevious!=NULL && lc_parcours->pNext!=NULL)
				{
#ifdef DEBUG
					printf("[delete] chainer pNext à pPrevious...\n");
#endif
					lc_parcours->pPrevious->pNext=lc_parcours->pNext;
					lc_parcours->pNext->pPrevious=lc_parcours->pPrevious;
					pListe->NbElem--;
					elementprecedent=lc_parcours->pPrevious->item_Number;
					if(lc_parcours->pFree!=NULL)			// 2018
					{
						lc_parcours->pFree(lc_parcours);
					}
					return elementprecedent;
				}
				if(lc_parcours->pPrevious==NULL && lc_parcours->pNext!=NULL)
				{
#ifdef DEBUG
					printf("[delete] élément de tête...\n");
#endif
					lc_parcours->pNext->pPrevious=NULL;
					pListe->pHead=lc_parcours->pNext;
					pListe->NbElem--;
					if(lc_parcours->pFree!=NULL) // 2018
					{
						lc_parcours->pFree(lc_parcours);
					}
					return 1;
				}
				if(lc_parcours->pNext==NULL && lc_parcours->pPrevious!=NULL)
				{
#ifdef DEBUG
					printf("[delete] arracher la queue...\n");
#endif
					lc_parcours->pPrevious->pNext=NULL;
					pListe->pTail=lc_parcours->pPrevious;	
					pListe->NbElem--;
					if(lc_parcours->pFree!=NULL) // 2018
					{
						lc_parcours->pFree(lc_parcours);
					}
					return 2;
				}
				// Cas du pNext ET pPrevious tous deux à NULL (il ne reste plus qu'un seul élément, on l'enlève)...
				pListe->pTail=pListe->pHead=NULL;
				pListe->NbElem=0;
				if(lc_parcours->pFree!=NULL) // 2018
				{
					lc_parcours->pFree(lc_parcours);
				}
				return 3;
			} // endif
			lc_parcours=lc_parcours->pNext;
		} //endwhile
	}
	return -1;
}

//*****************************************************************************
// lc_setDisplay
//
// Permet d'initialiser la fonction d'affichage des éléments de la liste
// 
// ENTREE(S):
//	pListe:		pointeur sur un élément de type ListeChainee
//	target:		INDICE de l'élément dont on doit initialiser la fonction d'affichage
//	ptrFct:		pointeur sur une fonction de type "void" ayant comme paramètre "void*"
//
// SORTIE:
//	-1: une erreur est survenue
//	0:	tout s'est bien passé
//*****************************************************************************

int lc_setDisplay(ListeChainee *pListe,int target,void (*ptrFct)(void*))
{
	lc_Datas *lc_Element; // =(lc_Datas*)malloc(sizeof(lc_Datas)); dec2017
	lc_Element=lc_get(pListe,target);
	if(lc_Element!=NULL) 
	{
		lc_Element->pDisplay=ptrFct;
		return 0;
	}
	else return -1;
}

int lc_setFreeItem(ListeChainee *pTarget,int position,void (*ptrFct)(void*))
{
		lc_Datas *lc_Element;
		lc_Element=lc_get(pTarget,position);
		if(lc_Element!=NULL)
		{
			fprintf(stderr,"[%s] set Free %05d (%08lx)\n",__func__,lc_Element->item_Number,ptrFct);
			lc_Element->pFree=ptrFct;
			return 0;
		}
		else return -1;
}


//*****************************************************************************
// lc_setDisplayByID
//
// Permet d'initialiser la fonction d'affichage des éléments de la liste 
// 
// ENTREE(S):
//	pListe:		pointeur sur un élément de type ListeChainee
//	itemID:		identifiant de l'élément dont on doit initialiser la fonction d'affichage
//	ptrFct:		pointeur sur une fonction de type "void" ayant comme paramètre "void*"
//
// SORTIE:
//	-1: une erreur est survenue
//	0:	tout s'est bien passé
//*****************************************************************************

int lc_setDisplayByID(ListeChainee *pListe, int itemID,void (*ptrFct)(void*))
{
	lc_Datas *lc_Element; //=(lc_Datas*)malloc(sizeof(lc_Datas)); dec2017
	lc_Element=lc_search(pListe,itemID);
	if(lc_Element!=NULL) 
	{
		lc_Element->pDisplay=ptrFct;
		return 0;
	}
	else return -1;
}

int lc_setpFreeItemByID(ListeChainee *pTarget,int itemID,void (*ptrFct)(void*))
{
		lc_Datas *lc_Element;
		lc_Element=lc_search(pTarget,itemID);
		if(lc_Element!=NULL)
		{
			lc_Element->pFree=ptrFct;
			return 0;
		}
		else return -1;
}

//*****************************************************************************
// lc_showList
//
// Permet d'afficher tous les éléments de la liste en utilisant la fonction
// initialisée via lc_setDisplay.
// 
// Si la fonction d'affichage n'est pas définie, la fonction n'affiche rien.
//
// ENTREE:
//	pListe:	pointeur sur un élément de type ListeChainee
//*****************************************************************************

void lc_showList(ListeChainee *pListe)
{
	if(pListe==NULL) return;
	if(pListe->pHead!=NULL)
	{
		lc_Datas *lc_parcours=pListe->pHead;
  
		while(lc_parcours!=NULL)
		{
			//printf("[ID:%d]",lc_parcours->item_Number);
			if(lc_parcours->pDisplay!=NULL)
				lc_parcours->pDisplay(lc_parcours->value);
			
			lc_parcours=lc_parcours->pNext;
			fflush(stdout);
		}
	}
}

//*****************************************************************************
// lc_empty
//
// Efface tous les éléments de la liste.
//
// ENTREE:	
//	pListe:	pointeur sur un élément de type ListeChainee
//
//*****************************************************************************

long lc_empty(ListeChainee *pListe)
{
	long	MemoryFreed=0L;
	// int		identifiant;
	
	while(pListe->pHead!=NULL)
	{
		MemoryFreed+=sizeof(lc_Datas);
		lc_Datas *todel=lc_pop(pListe);
		// juillet 2018 (décidé de faire mon possible pour éviter les memory leaks)
		MemoryFreed+=todel->dataSize;
		//free(todel->value);
		free(todel);
	}
	pListe->pTail=pListe->pHead=NULL;
	return MemoryFreed;
}

//*****************************************************************************
// lc_pop 
//
// Efface l'élément de tête et retourne son contenu.
//
// ENTREES:
//	pListe:		pointeur sur un élément de type ListeChainee
// SORTIE:
//	Un pointeur sur un élément de type lc_Datas.
//*****************************************************************************

lc_Datas* lc_pop(ListeChainee *pListe) 
{
	lc_Datas *ptmp=NULL;
	if(pListe->pHead!=NULL)
	{
		ptmp=pListe->pHead;
		pListe->NbElem--;
			
		pListe->pHead=pListe->pHead->pNext;
		if(pListe->pHead==NULL) pListe->pTail=(lc_Datas*)NULL;
	}
	return (lc_Datas*)ptmp;
}

//*****************************************************************************
// lc_getID
//
// Récupère la valeur de l'identifiant de l'élément de tête
//
// ENTREES:
//	pListe: pointeur sur un élément de type ListeChainee
// SORTIE:
//	un numéro d'identifiant 
//*****************************************************************************

unsigned int lc_getID(ListeChainee *pListe)
{
	if(pListe!=NULL)
	{
		if(pListe->pHead!=NULL)
			return pListe->pHead->item_Number;
	}
	return 0;
}

// WRAPPER

unsigned int lc_getHeadID(ListeChainee *pListe)
{
	// Je fais pas d'appel à lc_getID pour éviter de faire trop d'appels de fonctions
	if(pListe!=NULL)
	{
		if(pListe->pHead!=NULL)
			return pListe->pHead->item_Number;
	}
	return 0;
}

//*****************************************************************************
// lc_getTailID
//
// Récupère la valeur de l'identifiant de l'élément en fin de liste 
//
// ENTREES:
//	pListe: pointeur sur un élément de type ListeChainee
// SORTIE:
//	un numéro d'identifiant 
//*****************************************************************************
unsigned int lc_getTailID(ListeChainee *pListe)
{
	// Je fais pas d'appel à lc_getID pour éviter de faire trop d'appels de fonctions
	if(pListe!=NULL)
	{
		if(pListe->pTail!=NULL)
			return pListe->pTail->item_Number;
	}
	return 0;
}

//*****************************************************************************
// lc_getIDat
//
// Récupère la valeur de l'identifiant de l'élément spécifié à une position donnée
//
// ENTREES:
//	pListe: 	pointeur sur un élément de type ListeChainee
//  position: 	indice de l'élément dont nous voudrions extraire l'identifiant
// SORTIE:
//	un numéro d'identifiant 
//*****************************************************************************

unsigned int lc_getIDat(ListeChainee *pListe,int position)
{
	lc_Datas 	*tmp;
	int			internalpos=0;
	
	if(pListe!=NULL)
	{
		if(pListe->pHead!=NULL)
		{
			tmp=pListe->pHead;
			while(tmp!=NULL)
			{
				if(internalpos==position) return tmp->item_Number;
				internalpos++;
				tmp=tmp->pNext;
			}
		}
	}
	return 0;
}

//*****************************************************************************
// lc_setByID
//
// Permet de placer un élément dans la liste (remplacement)
//
// ENTREES:
//	pListe: pointeur sur un élément de type ListeChainee
// SORTIE:
//	-2: élément introuvable
//  -1: erreur (pointeur à NULL)
//	0: remplacement en plein milieu
//	1: remplacement en tête de liste
//	2: remplacement en fin de liste
//*****************************************************************************
int lc_setByID(void *targetElem,ListeChainee *pListe, int targetID)
{
	lc_Datas	*tmpParcours;
	
	if(targetElem==NULL) return -1;
	
	if(pListe!=NULL)
	{
		if(pListe->pHead!=NULL)
		{
			tmpParcours=pListe->pHead;
			while(tmpParcours!=NULL)
			{
				// Il ne faut SURTOUT PAS modifier le chainage !!
				
				if(targetID==tmpParcours->item_Number)
				{

					tmpParcours->value=targetElem;
					
					if(tmpParcours->pPrevious==NULL) return 1;	
					if(tmpParcours->pNext==NULL) return 2;
					return 0;
				}
				tmpParcours=tmpParcours->pNext;																						// nous passons à l'élément suivant...
			}
			return -2;
		}
	}
	return -1;
}
//*****************************************************************************
// lc_init
//
// Permet d'initialiser une liste chaînée
//
// ENTREES:
//	pListe: pointeur sur un élément de type ListeChainee
// SORTIE:
//	néant
//*****************************************************************************
LinkedList* lc_init(void)
{
	return calloc(1,sizeof(LinkedList)); // malloc
	/*if(pListe!=NULL)
	{
		pListe->NbElem=0;
		pListe->pHead=NULL;
		pListe->pTail=NULL;
	}
	return pListe;*/
}
//*****************************************************************************
// wraparray
//
// Permet d'initialiser un tableau de x dimensions
//
// ENTREES:
//	pArray: pointeur sur le premier élément d'un tableau de une, deux ou trois dimensions
// nbcols:	nombre de colonnes
// nblins:  nombre de lignes
// depth:		profondeur
// SORTIE:
//	un pointeur sur un élément de type t_cssmarray
//*****************************************************************************
t_cssmarray* wraparray(void *pArray,int nbcols,int nblins,int depth)
{
	t_cssmarray *tmp=calloc(1,sizeof(t_cssmarray));
	*tmp=(t_cssmarray){pArray,nbcols,nblins,depth};
	
	return tmp;
}

//*************************************
// lc_FindByValue (2020)
//
// Fonction de recherche dans une liste chaînée par valeur
//
// input:		depot -- la liste chaînée contenant les références
//					value -- le "pattern" à chercher
//					fctcmp -- une fonction de comparaison (dans le cadre d'une structure de données on ne peut pas comparer aussi facilement qu'avec les types prédéfinis)
//
//	Exemple:  dans le cadre de comparaison entre la source et destination d'un message ARP
//	bool compare(void *model,void *pattern)
//	{
//		t_MyARPInfos *tmpM=(t_MyARPInfos*)model;
//		t_MyARPInfos *tmpP=(t_MyARPInfos*)pattern;
//	
//		if(strcmp(tmpM->dMACAddr,tmpP->sMACAddr)!=0) return false;
//		return true;
// 	}
//
// Utilisation dans le code:
//	int resultat=lc_FindByValue(pARP,arpinfos,compare);
//
// output:	l'identifiant de l'objet dans la liste chaînée
//					ou -1 si pas trouvé (-2 si fonction de comparaison non fournie)
//
//

int lc_FindByValue(LinkedList *depot,void *value,bool (*fctcmp)(void*,void*))
{
	if(depot==NULL) return -1;
	if(value==NULL) return -1;
	
	lc_Datas *pSeek=depot->pHead;		// arbitrairement on commence par la tête de la liste
																	// cela risque d'être lent à crever :{
	
	while(pSeek!=NULL)
	{
		if(pSeek->dataType==cssmfloat) 
		{
			float epsilon;
			float delta;
				
			epsilon=1E-38;
			delta=fabsf(*((float*)pSeek->value)-*((float*)value));
			if(epsilon < delta) return pSeek->item_Number;
		}
		
		if(pSeek->dataType==cssmdouble) 
		{
			double epsilon;
			double delta;
				
			epsilon=1E-308;
			delta=fabs(*((double*)pSeek->value)-*((double*)value));
			if(epsilon < delta) return pSeek->item_Number;
		}
		
		if(pSeek->dataType==cssmldouble) 
		{
			long double epsilon;
			long double delta;
				
			epsilon=1E-308;
			delta=fabsl(*((long double*)pSeek->value)-*((long double*)value));
			if(epsilon < delta) return pSeek->item_Number;
		}
				
		switch(pSeek->dataType)
		{
			case cssmbyte:	if(*((char*)pSeek->value)==*((char*)value)) return pSeek->item_Number;
											break;
		  case cssmshort:	if(*((short*)pSeek->value)==*((short*)value)) return pSeek->item_Number;
											break;
			case cssmint:		if(*((int*)pSeek->value)==*((int*)value)) return pSeek->item_Number;
											break;
			case cssmlong:	if(*((long*)pSeek->value)==*((long*)value)) return pSeek->item_Number;
											break;
			case cssmllong:	if(*((long long*)pSeek->value)==*((long long*)value)) return pSeek->item_Number;
											break;
		}
		
		if(pSeek->dataType==cssmuserdef) // structure ou alphanumerique
		{
			if(fctcmp==NULL) return -2;
			bool result=fctcmp(pSeek->value,value);
			if(result) return pSeek->item_Number;
		}
		pSeek=pSeek->pNext;
	}
	return -1;
}

//*************************************
// lc_sort (2022)
// 
// Fonction qui permet de trier des données d'une liste chaînée
// Attention: il faut que les données soient du même type !!!!!
//
// Input:
//	source: une liste chaînée contenant des éléments du même type
//	fctcompare: fonction de comparaison pour les structures de données complexes
//	Ascending: à false indique le tri par ordre décroissant
//
// Output:
//	retourne un pointeur sur une structure de donnée de type LinkedList.

LinkedList* lc_sort(LinkedList *source,int (*fctcompare)(void*,void*),bool Ascending)
{
	LinkedList	*tmpList=lc_init();
	lc_Datas		*reference=NULL;
		
	int position=1;
	
	if(source==NULL) return NULL;
		
	
	lc_Datas		*compareme;
	lc_Datas		*parcours;
	do
	{
		parcours=source->pHead;
		if(parcours==NULL) break;																							
		
		compareme=lc_get(source,position);
		
		if(compareme==NULL) // cas du dernier élément non trié... automatiquement le plus petit/grand
		{
			int ItemID=lc_add(parcours->value,tmpList,parcours->dataType,parcours->dataSize);
			lc_setDisplayByID(tmpList,ItemID,parcours->pDisplay);
			lc_delete(source,parcours->item_Number);
			break;
		}
		reference=compareme;
		
		if(parcours->dataType!=compareme->dataType) break;															// on ne compare pas des pommes et des poires (ni C et Python)
		if(parcours->dataType==uepuserdef && fctcompare==NULL) break;										// si il n'y a pas de fonction de comparaison on quitte
			
		do
		{
			if(!Ascending)
			{
				switch(parcours->dataType)
				{
					case uepbyte:			if(*((char*)parcours->value)>*((char*)reference->value)) reference=parcours;
														break;
					case uepshort:    if(*((short*)parcours->value)>*((short*)reference->value)) reference=parcours;
														break;
					case uepint:			if(*((int*)parcours->value)>*((int*)reference->value)) reference=parcours;
														break;
					case uepfloat:		if(*((float*)parcours->value)>*((float*)reference->value)) reference=parcours;
														break;
					case ueplong:			if(*((long*)parcours->value)>*((long*)reference->value)) reference=parcours;
														break;
					case uepllong:	  if(*((long long*)parcours->value)>*((long long*)reference->value)) reference=parcours;
														break;
					case uepdouble:		if(*((double*)parcours->value)>*((double*)reference->value)) reference=parcours;
														break;
					case uepldouble:	if(*((long double*)parcours->value)>*((long double*)reference->value)) reference=parcours;
														break;	
					default:					if(fctcompare(parcours->value,reference->value)>0) reference=parcours;
				}
			}
			else
			{
				switch(parcours->dataType)
				{
					case uepbyte:			if(*((char*)parcours->value)<*((char*)reference->value)) reference=parcours;
														break;
					case uepshort:    if(*((short*)parcours->value)<*((short*)reference->value)) reference=parcours;
														break;
					case uepint:			if(*((int*)parcours->value)<*((int*)reference->value)) reference=parcours;
														break;
					case uepfloat:		if(*((float*)parcours->value)<*((float*)reference->value)) reference=parcours;
														break;
					case ueplong:			if(*((long*)parcours->value)<*((long*)reference->value)) reference=parcours;
														break;
					case uepllong:	  if(*((long long*)parcours->value)<*((long long*)reference->value)) reference=parcours;
														break;
					case uepdouble:		if(*((double*)parcours->value)<*((double*)reference->value)) reference=parcours;
														break;
					case uepldouble:	if(*((long double*)parcours->value)<*((long double*)reference->value)) reference=parcours;
														break;	
					default:					if(fctcompare(parcours->value,reference->value)<0) reference=parcours;
				}
			}
							
			parcours=parcours->pNext;
		}while(parcours!=NULL);
		// Ici nous avons l'élément le plus grand de la collection de valeurs...
		lc_Datas *copy=lc_search(source,reference->item_Number);
		int ItemID=lc_add(copy->value,tmpList,reference->dataType,reference->dataSize);
		lc_setDisplayByID(tmpList,ItemID,compareme->pDisplay);
		lc_delete(source,reference->item_Number);
		reference=NULL;
	}while(source->NbElem>0);
	// La liste devrait être triée
	return tmpList;
}

