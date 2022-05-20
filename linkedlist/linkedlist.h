#ifndef __LINKEDLIST__
#define __LINKEDLIST__

#include <stdbool.h>

enum{cssmuserdef=-1,cssmbyte=1,cssmshort,cssmint=4,cssmfloat,cssmlong,cssmllong=8,cssmdouble,cssmldouble};
enum{uepuserdef=-1,uepbyte=1,uepshort,uepint=4,uepfloat,ueplong,uepllong=8,uepdouble,uepldouble};

typedef struct item_Datas
{
	unsigned int        item_Number;
	unsigned int        dataSize;
	int                 dataType;
	struct item_Datas   *pPrevious;
	struct item_Datas   *pNext;
	
	void	*value;
  
  // pointeur de fonction
	void                (*pDisplay)(void*);												// pour pouvoir afficher des données du standard ANSI C et celles définies par l'utilisateur...
	void								(*pFree)(void*);
}lc_Datas;

typedef struct 
{
	unsigned int	NbElem;
	lc_Datas    *pHead;
	lc_Datas    *pTail;
}ListeChainee;

// juillet 2018
typedef struct s_array																					// encapsulation d'un tableau 
{
	void	*pBaseAddress;
	
	unsigned int		sizex;
	unsigned int		sizey;
	unsigned int		sizez;
		
}t_cssmarray;


typedef ListeChainee LinkedList;                                // un alias...

int lc_insert(void*,ListeChainee*,short,short);                 // insère en début de liste (LIFO) (push)
int lc_add(void*,ListeChainee*,short,short);                    // ajoute en fin de liste (FIFO)
int lc_delete(ListeChainee*,int);                               // ATTENTION: le "int" en question est un identifiant !! (TODO: renommer lc_delete en lc_deleteByID et permettre un lc_delete en fonction non pas de l'identifiant mais de la position dans la liste) 

int lc_setDisplay(ListeChainee*,int,void(*)(void*));            // permet de spécifier une fonction pour afficher le contenu de l'élément de la liste (int étant un indice)
int lc_setDisplayByID(ListeChainee*,int,void(*)(void*));        // permet de spécifier une fonction pour afficher le contenu de l'élément de la liste identifié non plus par sa position dans la liste mais par son identifiant...

int lc_setFreeItem(ListeChainee*,int,void(*)(void*));						// int est un indice
int lc_setFreeItemByID(ListeChainee*,int,void(*)(void*));				// int est un identifiant

unsigned int lc_getID(ListeChainee*);                           // permet de récupérer l'identifiant de l'élément en tête de liste (pHead)

// Décembre 2017

unsigned int lc_getHeadID(ListeChainee*);												// "Wrapper"pour éviter de devoir modifier tous les programmes qui utilisaient lc_getID() mais il fait la même chose 
unsigned int lc_getTailID(ListeChainee*);												// Permet de récupérer l'identifiant de l'élément de fin de file (pTail)

unsigned int lc_getIDat(ListeChainee*,int);                     // permet de récupérer l'identifiant de l'élément se trouvant à la position représentée par int

long  lc_empty(ListeChainee*);                                  // vide la liste...

void  lc_showList(ListeChainee*);                               // affiche le contenu de la liste (attention si lc_setDisplay n'a pas été appelé, le contenu de l'élément de liste ne sera pas affiché


int lc_setByID(void*,ListeChainee*,int);                    		// permet de placer à un endroit de la liste un élément dont la position dans celle-ci est représentée par un identifiant. 

lc_Datas* lc_pop(ListeChainee*);                                // extrait (sort de la liste) le premier élément de la liste (tête)
lc_Datas* lc_search(ListeChainee*,int);                         // ATTENTION: le "int" en question est un identifiant !!  (TODO: renommer lc_search en lc_searchByID)
lc_Datas* lc_get(ListeChainee*,int);														// ATTENTION: le "int" en question est un indice !!             
lc_Datas* lc_lastget(ListeChainee*,int);                        // récupère (sans l'effacer) l'élément se trouvant en fin de liste...

LinkedList*		lc_init(void);																		// initialise une liste chaînée
t_cssmarray*	wraparray(void*,int,int,int);											// encapsule un tableau de x dimensions de n'importe quel type

int lc_FindByValue(LinkedList*,void*,bool (*)(void*,void*));		// compare deux éléments par leur valeur dans une fonction fournie en dernier paramètre
LinkedList* lc_sort(LinkedList*,int(*)(void*,void*),bool);			// permet d'effectuer le tri décroissant/croissant en fonction de la valeur du bool

#endif


