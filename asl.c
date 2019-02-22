#include "listx.h"
#include "types_rikaya.h"
#include "const.h"
#include "asl.h"

/* array di semd con dimensione massima MAX PROC*/
HIDDEN semd_t semd_table[MAXPROC];

/*lista dei semd liberi o inutilizzati*/
/*HIDDEN struct list_head semdFree = LIST_HEAD_INIT(semdFree);
inizializzazione elemento sentinella*/

HIDDEN LIST_HEAD(semdFree_h);

/* Creo lista ASL e sentinella*/

HIDDEN LIST_HEAD(semd_h);

/*LIST_HEAD(semdFree_h);*/

void initASL () {
  int i=0;

  for (i;i<MAXPROC;i++){

    semd_t* semt= &semd_table[i];
        list_add_tail(&(semt.s_next),&(semdFree_h));
  }
}


/* DESCRIZIONE: 
restituisce il puntatore al SEMD nella ASL la cui chiave è pari a key. Se non 
esiste un elemento nella ASL con chiave eguale a key, viene restituito NULL. */

semd_t* getSemd(int *key){

	if (list_empty(&(semd_h->s_next)) return NULL; 		
	semd_t* p; 
	semd_t* res = NULL;
	list_for_each_entry(p, semd_h, s_next){		
		if( *key == p->s_key){
		 res = container_of(semd_h->next , semd_t, s_next);
	}
	return res; 
}

/* DESCRIZIONE: Viene inserito il PCB puntato da p nella coda dei processi bloccati 
associata al SEMD con chiave key. Se il semaforo corrispondente non è presente nella ASL, 
alloca un nuovo SEMD dalla lista di quelli liberi (semdFree) e lo inserisce nella ASL, 
settando I campi in maniera opportuna (i.e. key e s_procQ). Se non è possibile allocare 
un nuovo SEMD perché la lista di quelli liberi è vuota, restituisce TRUE. 
In tutti gli altri casi, restituisce FALSE.*/


int insertBlocked(int *key, pcb_t* p){
	bool trovato = FALSE;
	semd_t* semd =  getSemd(key);  //cerco semd con chiave key
	if (semd != NULL){ 
		trovato = TRUE; //semd cercato presente nella ASL
		insertProcQ( &(semd->s_procQ) , p);	//inserisco PCB puntato da p
		return FALSE;
	}

	if (list_empty(&(semdFree_h->s_next)) return TRUE;
	else { 
		if (!trovato) list_add_tail(&(semdFree_h->s_next), &(semd_h->s_next) { //caso in cui non è presente nella ASL, allora lo prendo dalla lista libera
			semdFree_h -> s_key = *key; //imposto parametri key e s_procQ
			semdFree_h -> s_procQ = p->s_procQ; 
		}
	}
}




/*
DESCRIZIONE: Rimuove il PCB puntato da p dalla coda del semaforo 
su cui è bloccato (indicato da p- >p_semKey). Se il PCB non compare in tale coda, 
allora restituisce NULL (condizione di errore). Altrimenti, restituisce p. */
pcb_t* outBlocked(pcb_t *p){
	semd_t* semd;
	bool trovato = FALSE;
	list_for_each(semd, semd_h){
		if (semd->s_key == p->p_semKey) list_del(p);
		trovato = TRUE;
	}
	if (!trovato) return NULL;
	else return p;
}


/*
DESCRIZIONE: 
Ritorna il primo PCB dalla coda dei processi bloccati (s_ProcQ) 
associata al SEMD della ASL con chiave key. Se tale descrittore 
non esiste nella ASL, restituisce NULL. Altrimenti, restituisce 
l’elemento rimosso. Se la coda dei processi bloccati per il semaforo diventa vuota, 
rimuove il descrittore corrispondente dalla ASL e lo inserisce 
nella coda dei descrittori liberi (semdFree).



pcb_t* removeBlocked(int *key){
	semd_t* semd;
	bool trovato = FALSE;
	list_for_each(semd, semd_h){ //cerco il semaforo nella ASL
		if (semd->s_key == key)  {
			trovato = TRUE; //trovato
			if (list_empty(semd->s_ProcQ)){//verifico che la coda dei processi bloccati non sia vuota
				list_del(semd); //tolgo semd dalla ASL
				list_add_tail(semd, semdFree_h);	//metto semd in coda alla lista libera
			} else {
				return container_of(semd, semd_t, s_ProcQ); //ritorno primo PCB 
			}	
		}

	if (!trovato) return NULL //se invece il semaforo non è nella ASL ritorno NULL
	}
}*/


pcb_t* removeBlocked(int *key){
		semd_t* semd = getSemd(&key);
		if (semd == NULL) return NULL; //Semd non è presente nella ASL
		pcb_t* pcb = headProcQ(semd -> s_procQ);
		if (pcb == NULL) ; //coda dei processi bloccati vuota
		else removeProcQ(semd->s_procQ);
		if (list_empty(semd->s_ProcQ)){
			list_del(semd); //tolgo semd dalla ASL
			list_add_tail(semd, semdFree_h);	//metto semd in coda alla lista libera
		}
		return pcb;
/* 
DESCRIZIONE: Restituisce (senza rimuovere) il puntatore al PCB 
che si trova in testa alla coda dei processi associata al SEMD con chiave key. 
Ritorna NULL se il SEMD non compare nella ASL oppure se compare ma la sua coda 
dei processi è vuota.

pcb_t* headBlocked(int *key){
		semd_t* semd;
	bool trovato = FALSE;
	list_for_each(semd, semd_h){ //cerco il semaforo nella ASL
		if (semd->s_key == key)  {
			trovato = TRUE; //trovato
			if (list_empty(semd->s_ProcQ)){//verifico che la coda dei processi bloccati non sia vuota
				list_del(semd); //tolgo semd dalla ASL
				list_add_tail(semd, semdFree_h);	//metto semd in coda alla lista libera
				return NULL;
			} else {
				return container_of(semd, semd_t, s_ProcQ); //ritorno primo PCB 
			}	
		}
	if (!trovato) return NULL //se invece il semaforo non è nella ASL ritorno NULL
	}
} VEDERE SE QUESTA VA BENE O MEGLIO QUELLA SOTTO (NON SAPEVO DELL'ESISTENZA DI HEADPROCQ)
*/

pcb_t* headBlocked(int *key){
		semd_t* semd = getSemd(&key);
		if (semd == NULL) return NULL; //Semd non è presente nella ASL
		pcb_t* pcb = headProcQ(semd -> s_procQ);
		if (pcb == NULL) return NULL; //coda dei processi bloccati vuota
		return pcb;
}

