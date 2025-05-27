#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Masina Masina;
typedef struct Nod Nod;
typedef struct Coada Coada;
struct Masina {
    int id;
    float pret;
    char* nume;
    char serie[3];
};

struct Nod {
    Masina info;
    Nod* next;
};

struct Coada {
    Nod* inceput;
    Nod* sfarsit;
};

Masina citireMasinaFisier(FILE* f) {
   char buffer[100];
    char sep[3]=",\n";
    Masina m;
    fgets(buffer,100,f);
    char* aux;
    aux=strtok(buffer,sep);
    m.id=atoi(aux);
    m.pret=atof(strtok(NULL,sep));
    aux=strtok(NULL,sep);
    m.nume=malloc(strlen(aux)+1);
    strcpy(m.nume,aux);
    aux=strtok(NULL,sep);
    strcpy(m.serie,aux);
    return m;

}



void afisareMasina(Masina m) {
    printf("ID: %d",m.id);
    printf("\nNUME: %s",m.nume);
    printf("\nPRET: %.2f",m.pret);
    printf("\nSERIE: %s",m.serie);
    printf("\n\n");
}


// void afisareMasina(Masina m) {
//     printf("ID: %d",m.id);
//     printf("\nNUME: %s",m.nume);
//     printf("\nPRET: %.2f",m.pret);
//     printf("\nSERIE: %s",m.serie);
//     printf("\n\n");
// }
//
//
void pushStack(Nod** stiva,Masina m) {
    Nod* nou=(Nod*)malloc(sizeof(Nod));
    nou->info=m;
    nou->next=(*stiva);
    (*stiva)=nou;
}

Masina popStack(Nod** stiva) {
    if ((*stiva)!=NULL) {
        Nod* aux=*stiva;
        Masina m=(*stiva)->info;
        (*stiva)=(*stiva)->next;
        free(aux);
        return m;
    }
    Masina m;
    m.id=-1;
    return m;
}

Nod* initializareStiva(const char* numeFisier) {
    FILE* f=fopen(numeFisier,"r");
    Nod* lista=NULL;
    while (!feof(f)) {
        Masina m=citireMasinaFisier(f);
        pushStack(&lista,m);
    }
    return lista;

}
//aici facem si dezalocare
 void afisareStiva(Nod** stiva) {
     while ((*stiva)!=NULL) {
         afisareMasina((*stiva)->info);
         Masina m=popStack(stiva);
         free(m.nume);
     }
     //popStack(&stiva);
 }

void dezalocareStiva(Nod** stiva) {
    while ((*stiva)!=NULL) {
        Masina m=popStack(stiva);
        free(m.nume);
    }
}

void dezalocareStiva2(Nod** stiva) {

    while ((*stiva)!=NULL) {
        Nod* aux=(*stiva);
        (*stiva)=(*stiva)->next;
        free(aux->info.nume);
        free(aux);
    }

}

//COADA CU LISTA SIMPLU INLANTUITA
 void enque(Coada* coada,Masina m) {
     Nod* nou=(Nod*)malloc(sizeof(Nod));
     nou->info=m;
     nou->next=NULL;
     if (coada->sfarsit!=NULL) {
         coada->sfarsit->next=nou;
     }
     else {
         coada->inceput=nou;
     }
     coada->sfarsit=nou;

 }

 Masina deque(Coada* coada) {
     Masina m;
     m.id=-1;
     if (coada->inceput!=NULL) {
         Nod* aux=coada->inceput;
         m=coada->inceput->info;
         if (coada->inceput->next!=NULL){
             coada->inceput=coada->inceput->next;
         }
         else {
             coada->inceput=NULL;
             coada->sfarsit=NULL;
         }
         free(aux);
     }
     return m;
 }

 void afisareCoada(Coada* coada) {
     while (coada->inceput) {
         afisareMasina(coada->inceput->info);
         Masina m=deque(coada);
         free(m.nume);
     }
    //coada->inceput=NULL;
    //coada->sfarsit=NULL;
 }



 Coada initializareCoada(const char* numeFisier) {
     FILE* f=fopen(numeFisier,"r");
     Coada coada;
     coada.inceput=NULL;
     coada.sfarsit=NULL;
     while (!feof(f)) {
         Masina m=citireMasinaFisier(f);
         enque(&coada,m);
     }
     fclose(f);
     return coada;

 }

 void dezalocareCoada(Coada* coada) {
     Nod* current=coada->inceput;
     while (current) {
         Nod* aux=current->next;
         if (current->info.nume!=NULL)
         free(current->info.nume);
         free(current);
         current=aux;
     }

     coada->inceput=NULL;
     coada->sfarsit=NULL;
 }



int main() {

    // FILE* f=fopen("/Users/mateiavram/Desktop/facultate/Structuri de Date/practice2/mas.txt","r");
    // Masina m=citireMasinaFisier(f);

     Nod* stiva=initializareStiva("/Users/mateiavram/Desktop/facultate/Structuri de Date/practice2/mas.txt");
     afisareStiva(&stiva);
   //dezalocareStiva(&stiva);
    //free(&stiva);


    //COADA SIMPLU INLANTUITA
     Coada coada=initializareCoada("/Users/mateiavram/Desktop/facultate/Structuri de Date/practice2/mas.txt");
     afisareCoada(&coada);


     //dezalocareCoada(&coada);





    return 0;
}
