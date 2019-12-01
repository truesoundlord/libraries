#include <stdio.h>
#include <stdlib.h>

#include <linkedlist.h>

void printElem_int(void*);
void printElem_long(void*);
void printElem_float(void*);

void main(void)
{
  LinkedList *uneliste;
  int elem1=88;
  float elem2=9.9;
  long elem3=100L;
  
  uneliste=lc_init();
  
  printf("Programme de test de la librairie linkedlist...\n");
  printf("Test de la fonction \x1b[1msetByID\x1b[0m...\n");
  
  printf("Ajouter les éléments avec insert()...\n");
  
  int id_int=lc_insert((void*)&elem1,uneliste,cssmint,sizeof(int));
  int id_float=lc_insert((void*)&elem2,uneliste,cssmfloat,sizeof(float));
  int id_long=lc_insert((void*)&elem3,uneliste,cssmlong,sizeof(long));
  
	// insert c'est un push !! Il faut faire tout à l'envers
	
	lc_setDisplay(uneliste,2,printElem_int);
	lc_setDisplay(uneliste,1,printElem_float);
	lc_setDisplay(uneliste,0,printElem_long);
  
	lc_showList(uneliste);
	
	int newval=69;
	lc_setByID((void*)&newval,uneliste,id_int);
	
	lc_showList(uneliste);
	
	float fnewval=69.69;
	lc_setByID((void*)&fnewval,uneliste,id_float);
	
	lc_showList(uneliste);
	
	long lnewval=1L;
	lc_setByID((void*)&lnewval,uneliste,id_long);
	
	lc_showList(uneliste);
	
	
	printf("\n");
}

void printElem_int(void *datas)
{
	int *tmp=(int*)datas;
	
	printf("[%07d]\n",*tmp);
}

void printElem_float(void *datas)
{
	float *tmp=(float*)datas;
	
	printf("[%5.2f]\n",*tmp);
}

void printElem_long(void *datas)
{
	long *tmp=(long*)datas;
	
	printf("[%07ld]\n",*tmp);
}