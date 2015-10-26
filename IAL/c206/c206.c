	
/* c206.c **********************************************************}
{* T�ma: Dvousm�rn� v�zan� line�rn� seznam
**
**                   N�vrh a referen�n� implementace: Bohuslav K�ena, ��jen 2001
**                            P�epracovan� do jazyka C: Martin Tu�ek, ��jen 2004
**                                             �pravy: Bohuslav K�ena, z��� 2012
**
** Implementujte abstraktn� datov� typ dvousm�rn� v�zan� line�rn� seznam.
** U�ite�n�m obsahem prvku seznamu je hodnota typu int.
** Seznam bude jako datov� abstrakce reprezentov�n prom�nnou
** typu tDLList (DL znamen� Double-Linked a slou�� pro odli�en�
** jmen konstant, typ� a funkc� od jmen u jednosm�rn� v�zan�ho line�rn�ho
** seznamu). Definici konstant a typ� naleznete v hlavi�kov�m souboru c206.h.
**
** Va��m �kolem je implementovat n�sleduj�c� operace, kter� spolu
** s v��e uvedenou datovou ��st� abstrakce tvo�� abstraktn� datov� typ
** obousm�rn� v�zan� line�rn� seznam:
**
**      DLInitList ...... inicializace seznamu p�ed prvn�m pou�it�m,
**      DLDisposeList ... zru�en� v�ech prvk� seznamu,
**      DLInsertFirst ... vlo�en� prvku na za��tek seznamu,
**      DLInsertLast .... vlo�en� prvku na konec seznamu, 
**      DLFirst ......... nastaven� aktivity na prvn� prvek,
**      DLLast .......... nastaven� aktivity na posledn� prvek, 
**      DLCopyFirst ..... vrac� hodnotu prvn�ho prvku,
**      DLCopyLast ...... vrac� hodnotu posledn�ho prvku, 
**      DLDeleteFirst ... zru�� prvn� prvek seznamu,
**      DLDeleteLast .... zru�� posledn� prvek seznamu, 
**      DLPostDelete .... ru�� prvek za aktivn�m prvkem,
**      DLPreDelete ..... ru�� prvek p�ed aktivn�m prvkem, 
**      DLPostInsert .... vlo�� nov� prvek za aktivn� prvek seznamu,
**      DLPreInsert ..... vlo�� nov� prvek p�ed aktivn� prvek seznamu,
**      DLCopy .......... vrac� hodnotu aktivn�ho prvku,
**      DLActualize ..... p�ep�e obsah aktivn�ho prvku novou hodnotou,
**      DLSucc .......... posune aktivitu na dal�� prvek seznamu,
**      DLPred .......... posune aktivitu na p�edchoz� prvek seznamu, 
**      DLActive ........ zji��uje aktivitu seznamu.
**
** P�i implementaci jednotliv�ch funkc� nevolejte ��dnou z funkc�
** implementovan�ch v r�mci tohoto p��kladu, nen�-li u funkce
** explicitn� uvedeno n�co jin�ho.
**
** Nemus�te o�et�ovat situaci, kdy m�sto leg�ln�ho ukazatele na seznam 
** p�ed� n�kdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodn� komentujte!
**
** Terminologick� pozn�mka: Jazyk C nepou��v� pojem procedura.
** Proto zde pou��v�me pojem funkce i pro operace, kter� by byly
** v algoritmick�m jazyce Pascalovsk�ho typu implemenov�ny jako
** procedury (v jazyce C procedur�m odpov�daj� funkce vracej�c� typ void).
**/

#include "c206.h"

int solved;
int errflg;

void DLError() {
/*
** Vytiskne upozorn�n� na to, �e do�lo k chyb�.
** Tato funkce bude vol�na z n�kter�ch d�le implementovan�ch operac�.
**/	
    printf ("*ERROR* This program has performed an illegal operation.\n");
    errflg = TRUE;             /* glob�ln� prom�nn� -- p��znak o�et�en� chyby */
    return;
}

void DLInitList (tDLList *L)	{
/*
** Provede inicializaci seznamu L p�ed jeho prvn�m pou�it�m (tzn. ��dn�
** z n�sleduj�c�ch funkc� nebude vol�na nad neinicializovan�m seznamem).
** Tato inicializace se nikdy nebude prov�d�t nad ji� inicializovan�m
** seznamem, a proto tuto mo�nost neo�et�ujte. V�dy p�edpokl�dejte,
** �e neinicializovan� prom�nn� maj� nedefinovanou hodnotu.
**/

 	if(!L)
		return;
			
	L->First = NULL;
	L->Act = NULL;
	L->Last = NULL;   
}

void DLDisposeList (tDLList *L)	{
/*
** Zru�� v�echny prvky seznamu L a uvede seznam do stavu, v jak�m
** se nach�zel po inicializaci. Ru�en� prvky seznamu budou korektn�
** uvoln�ny vol�n�m operace free. 
**/

	if(!L)
		return;
	
	tDLElemPtr iterator;
	
	while((iterator = L->First)){
		L->First = iterator->rptr;
		
		free(iterator);
	}
		
	L->First = NULL;
	L->Act = NULL;
	L->Last = NULL;
}

void DLInsertFirst (tDLList *L, int val)	{
/*
** Vlo�� nov� prvek na za��tek seznamu L.
** V p��pad�, �e nen� dostatek pam�ti pro nov� prvek p�i operaci malloc,
** vol� funkci DLError().
**/

    if(!L)
		return;
    
    tDLElemPtr item = malloc(sizeof(struct tDLElem));
    
    if(!item){
		DLError();
		return;
	}
	
	item->data = val;
	item->lptr = NULL;
	item->rptr = L->First;
	
	//If nonempty
	if(L->First)
		L->First->lptr = item;	
	else
		L->Last = item;
	
	L->First = item;	
}

void DLInsertLast(tDLList *L, int val)	{
/*
** Vlo�� nov� prvek na konec seznamu L (symetrick� operace k DLInsertFirst).
** V p��pad�, �e nen� dostatek pam�ti pro nov� prvek p�i operaci malloc,
** vol� funkci DLError().
**/ 	

    if(!L)
		return;
    
    tDLElemPtr item = malloc(sizeof(struct tDLElem));
    
    if(!item){
		DLError();
		return;
	}
	
	item->data = val;
	item->lptr = L->Last;
	item->rptr = NULL;
	
	//If empty
	if(!L->First)
		L->First = item;
	else
		L->Last->rptr = item;
	
	L->Last = item;	
}

void DLFirst (tDLList *L)	{
/*
** Nastav� aktivitu na prvn� prvek seznamu L.
** Funkci implementujte jako jedin� p��kaz (nepo��t�me-li return),
** ani� byste testovali, zda je seznam L pr�zdn�.
**/

	L->Act = L->First;
}

void DLLast (tDLList *L)	{
/*
** Nastav� aktivitu na posledn� prvek seznamu L.
** Funkci implementujte jako jedin� p��kaz (nepo��t�me-li return),
** ani� byste testovali, zda je seznam L pr�zdn�.
**/
	
	L->Act = L->Last;
}

void DLCopyFirst (tDLList *L, int *val)	{
/*
** Prost�ednictv�m parametru val vr�t� hodnotu prvn�ho prvku seznamu L.
** Pokud je seznam L pr�zdn�, vol� funkci DLError().
**/

	if(!L)
		return;
	
	if(!L->First){
		DLError();
		return;
	}
	
	*val = L->First->data;	
}

void DLCopyLast (tDLList *L, int *val)	{
/*
** Prost�ednictv�m parametru val vr�t� hodnotu posledn�ho prvku seznamu L.
** Pokud je seznam L pr�zdn�, vol� funkci DLError().
**/
	
	if(!L)
		return;
	
	if(!L->First){
		DLError();
		return;
	}
	
	*val = L->Last->data;	
}

void DLDeleteFirst (tDLList *L)	{
/*
** Zru�� prvn� prvek seznamu L. Pokud byl prvn� prvek aktivn�, aktivita 
** se ztr�c�. Pokud byl seznam L pr�zdn�, nic se ned�je.
**/

	if(!L || !L->First)
		return;
	
	tDLElemPtr iterator = L->First->rptr;
	
	if(L->First == L->Act)
		L->Act = NULL;
	
	free(L->First);
	
	L->First = iterator;
	L->First->lptr = NULL;
	
	if(!iterator->rptr)
		L->Last = L->First;
}	

void DLDeleteLast (tDLList *L)	{
/*
** Zru�� posledn� prvek seznamu L. Pokud byl posledn� prvek aktivn�,
** aktivita seznamu se ztr�c�. Pokud byl seznam L pr�zdn�, nic se ned�je.
**/ 

	if(!L || !L->First)
		return;		
		
	if(L->Last == L->Act)
		L->Act = NULL;
		
	tDLElemPtr iterator = L->Last;		

	L->Last = L->Last->lptr;
	
	free(iterator);
	
	//If Empty
	if(!L->Last)
		L->First = NULL;
	else
		L->Last->rptr = NULL;
}

void DLPostDelete (tDLList *L)	{
/*
** Zru�� prvek seznamu L za aktivn�m prvkem.
** Pokud je seznam L neaktivn� nebo pokud je aktivn� prvek
** posledn�m prvkem seznamu, nic se ned�je.
**/

	if(!L || !L->Act || L->Act == L->Last)
		return;
		
	tDLElemPtr iterator = L->Act->rptr;
	
	//In the middle
	if(iterator->rptr){
		L->Act->rptr = iterator->rptr;
		iterator->rptr->lptr = L->Act;		
	}
	//On the end
	else{
		L->Act->rptr = NULL;
		L->Last = L->Act;
	}
	
	free(iterator);
}

void DLPreDelete (tDLList *L)	{
/*
** Zru�� prvek p�ed aktivn�m prvkem seznamu L .
** Pokud je seznam L neaktivn� nebo pokud je aktivn� prvek
** prvn�m prvkem seznamu, nic se ned�je.
**/

	if(!L || !L->Act || L->Act == L->First)
		return;
		
	tDLElemPtr iterator = L->Act->lptr;
	
	//In the middle
	if(iterator->lptr){
		L->Act->lptr = iterator->lptr;
		iterator->lptr->rptr = L->Act;		
	}
	//On the beginning
	else{
		L->Act->lptr = NULL;
		L->First = L->Act;
	}
	
	free(iterator);
}

void DLPostInsert (tDLList *L, int val) {
/*
** Vlo�� prvek za aktivn� prvek seznamu L.
** Pokud nebyl seznam L aktivn�, nic se ned�je.
** V p��pad�, �e nen� dostatek pam�ti pro nov� prvek p�i operaci malloc,
** vol� funkci DLError().
**/

    if(!L || !L->Act)
		return;
    
    tDLElemPtr item = malloc(sizeof(struct tDLElem));
    
    if(!item){
		DLError();
		return;
	}
	
	item->data = val;
	item->lptr = L->Act;
	item->rptr = NULL;
	
	//In the middle
	if(L->Act->rptr){
		item->rptr = L->Act->rptr;
		L->Act->rptr->lptr = item;	
	}
	//On the end
	else
		L->Last = item;	
	
	L->Act->rptr = item;	
}

void DLPreInsert (tDLList *L, int val)		{
/*
** Vlo�� prvek p�ed aktivn� prvek seznamu L.
** Pokud nebyl seznam L aktivn�, nic se ned�je.
** V p��pad�, �e nen� dostatek pam�ti pro nov� prvek p�i operaci malloc,
** vol� funkci DLError().
**/

    if(!L || !L->Act)
		return;
		
    tDLElemPtr item = malloc(sizeof(struct tDLElem));
    
    if(!item){
		DLError();
		return;
	}
	
	item->data = val;
	item->lptr = NULL;
	item->rptr = L->Act;
	
	//In the middle
	if(L->Act->lptr){
		item->lptr = L->Act->lptr;
		
		L->Act->lptr->rptr = item;
	}
	else
		L->First = item;
		
	L->Act->lptr = item;
}

void DLCopy (tDLList *L, int *val)	{
/*
** Prost�ednictv�m parametru val vr�t� hodnotu aktivn�ho prvku seznamu L.
** Pokud seznam L nen� aktivn�, vol� funkci DLError ().
**/
		
	if(!L)
		return;
	
	if(!L->Act){
		DLError();
		return;
	}
	
	*val = L->Act->data;	
}

void DLActualize (tDLList *L, int val) {
/*
** P�ep�e obsah aktivn�ho prvku seznamu L.
** Pokud seznam L nen� aktivn�, ned�l� nic.
**/
	
	if(!L || !L->Act)
		return;
		
	L->Act->data = val;
}

void DLSucc (tDLList *L)	{
/*
** Posune aktivitu na n�sleduj�c� prvek seznamu L.
** Nen�-li seznam aktivn�, ned�l� nic.
** V�imn�te si, �e p�i aktivit� na posledn�m prvku se seznam stane neaktivn�m.
**/

	if(!L || !L->Act)
		return;
		
	L->Act = L->Act->rptr;
}


void DLPred (tDLList *L)	{
/*
** Posune aktivitu na p�edchoz� prvek seznamu L.
** Nen�-li seznam aktivn�, ned�l� nic.
** V�imn�te si, �e p�i aktivit� na prvn�m prvku se seznam stane neaktivn�m.
**/
	
	if(!L || !L->Act)
		return;
		
	L->Act = L->Act->lptr;
}

int DLActive (tDLList *L) {		
/*
** Je-li seznam aktivn�, vrac� true. V opa�n�m p��pad� vrac� false.
** Funkci implementujte jako jedin� p��kaz.
**/

		return (L->Act)? TRUE : FALSE;
}

/* Konec c206.c*/
