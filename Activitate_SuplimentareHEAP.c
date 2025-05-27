#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Persoana Persoana;
typedef struct Heap Heap;
struct Persoana {
    int id;
    char* nume;
};

struct Heap {
    int lungime;
    Persoana* pacienti;
    int nrPacienti;
};

Persoana citirePersoanaFisier(FILE* f) {
    Persoana p;
    char buffer[100];
    char sep[3]=",\n";
    char* aux;
    fgets(buffer,100,f);
    aux=strtok(buffer,sep);
    p.id=atoi(aux);
    aux=strtok(NULL,sep);
    p.nume=(char*)malloc(strlen(aux)+1);
    strcpy(p.nume,aux);
    return p;
}

void afisarePacient(Persoana p) {
    printf("ID: %d",p.id);
    printf("\nNUME: %s",p.nume);
    printf("\n\n");
}

Heap initializareHeap(int lungime) {
    Heap heap;
    heap.lungime=lungime;
    heap.pacienti=(Persoana*)malloc(sizeof(Persoana)*lungime);
    heap.nrPacienti=0;
    return heap;
}

void filtreazaHeap(Heap heap,int pozitieNod) {
    if (pozitieNod>0 && pozitieNod<heap.nrPacienti) {
        int pozSt=2*pozitieNod+1;
        int pozDr=2*pozitieNod+2;
        int pozMax=pozitieNod;
        if (pozSt<heap.nrPacienti && heap.pacienti[pozSt].id>heap.pacienti[pozitieNod].id) {
            pozMax=pozSt;
        }
        if (pozDr<heap.nrPacienti && heap.pacienti[pozDr].id>heap.pacienti[pozitieNod].id) {
            pozMax=pozDr;
        }
        if (pozitieNod!=pozMax) {
            Persoana aux=heap.pacienti[pozitieNod];
            heap.pacienti[pozitieNod]=heap.pacienti[pozMax];
            heap.pacienti[pozMax]=aux;
            if (pozMax<(heap.nrPacienti-1)/2) {
                filtreazaHeap(heap,pozMax);
            }

        }
    }

}


Heap inserareHeap(const char* numef) {
    Heap heap=initializareHeap(10);
    FILE* f=fopen(numef,"r");
    int i=0;
    while (!feof(f)) {
        Persoana p=citirePersoanaFisier(f);
        heap.pacienti[i]=p;
        i++;
    }
    fclose(f);
    heap.nrPacienti=i;
    for (int j=(heap.nrPacienti-1)/2;j>=0;j--) {
        filtreazaHeap(heap,i);
    }

    return heap;
}


void afisareHeap(Heap heap) {
    for (int i=0;i<heap.nrPacienti;i++) {
        afisarePacient(heap.pacienti[i]);
    }
}

void dezalocareHeap(Heap* heap) {
    for (int i=0;i<heap->lungime;i++) {
        free(heap->pacienti[i].nume);
    }
    free(heap->pacienti);
    heap->lungime=0;
    heap->pacienti=NULL;
    heap->nrPacienti=0;
}

Persoana extragereDinHeap(Heap* heap) {
    Persoana p=heap->pacienti[0];
    heap->pacienti[0]=heap->pacienti[heap->nrPacienti-1];
    heap->pacienti[heap->nrPacienti-1]=p;
    heap->nrPacienti--;
    for (int i=(heap->nrPacienti-1)/2;i>=0;i--) {
        filtreazaHeap(*heap,i);
    }
    return p;

}



int main() {

    Heap heap=inserareHeap("/Users/mateiavram/Desktop/facultate/Structuri de Date/pr2/pac.txt");
    afisareHeap(heap);

    
    Persoana p=extragereDinHeap(&heap);
    afisarePacient(p);

    dezalocareHeap(&heap);
  //FISIER TEXT
  /* 
      1,Matei
      2,Alex
      3,Andrei
      4,Vlad
      5,Florin
      6,Ana
      7,Iulia
      8,Carmen
      9,Denisa
      10,Adina
  */

  

    return 0;
}
