#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct Masina {
    int id;
    char* marca;
    float capacitate;
};
typedef struct Masina Masina;
Masina initializareMasina(int id,const char* nume, float capacitate) {
    Masina m1;
    m1.id=id;
    m1.capacitate=capacitate;
    if (nume==NULL) {
        m1.marca=NULL;
    }
    else {
        m1.marca=(char*)malloc(strlen(nume)+1);
        strcpy(m1.marca,nume);
    }
    return m1;
}
Masina citesteMasinaDinFisier(FILE* f) {
    char buffer[100];
    char sep[3]=",\n";
    fgets(buffer,100,f);
    char* aux;
    Masina m1;
    m1.id=atoi(strtok(buffer,sep));
    aux=strtok(NULL,sep);
    m1.marca=(char*)malloc(strlen(aux)+1);
    strcpy(m1.marca,aux);
    m1.capacitate=atof(strtok(NULL,sep));
    return m1;
}

void afisareMasina(Masina m) {
    printf("Id: %d\n",m.id);
    printf("Marca: %s\n",m.marca);
    printf("Capacitate: %.2f\n\n",m.capacitate);
}



//vector
/*void adaugaMasinaInVector(Masina** v,int *nrMasini,Masina nou) {
    Masina* aux=(Masina*)malloc(sizeof(Masina)*((*nrMasini)+1));
    for (int i=0;i< (*nrMasini) ;i++) {
        aux[i]=(*v)[i];
    }
    aux[(*nrMasini)]=nou;
    (*nrMasini)++;

    free(*v);
    *v=aux;


}
void afiseazaMasiniVector(Masina *v,int nr) {
    for (int i=0;i<nr;i++) {
        afisareMasina(v[i]);
    }
}*/






//lista simplu inlantuita

typedef struct Nod Nod;
struct Nod {
    Masina info;
    Nod* next;
};
void adaugaMasinaInListaSimpla(Nod** cap,Masina MasinaNoua) {
    Nod* nou=(Nod*)malloc(sizeof(Nod));
    nou->info=MasinaNoua;
    nou->next=NULL;
    if ((*cap)==NULL) {
        *cap=nou;
    }
    else {
        Nod* p=*cap;
        while (p->next) {
            p=p->next;
        }
        p->next=nou;
    }
}

void adaugaMasinaLaInceputLS(Nod** cap,Masina MasinaNoua) {
    Nod* nou=(Nod*)malloc(sizeof(Nod));
    nou->info=MasinaNoua;
    nou->next=NULL;
    if ((*cap)==NULL) {
        *cap=nou;
    }
    else {
        nou->next=*cap;
        *cap=nou;
    }
}


void stergeMasinaDupaSerieLS(Nod** cap,int id) {
    Nod* current=*cap;
    Nod* prev=NULL;
    while (current) {
        if (current->info.id==id) {
            Nod* aux=current;
            if (prev==NULL) {
                (*cap)=current->next;
            }
            else {
                current=current->next;
                prev->next=current;
            }
            free(aux->info.marca);
            free(aux);

        }
        else {
            prev=current;
            current=current->next;
        }
    }
}

void afisareListaSimplaMasini(Nod* cap) {

    while (cap) {
        afisareMasina(cap->info);
        cap=cap->next;
    }

}
void dezalocaListaSimpla(Nod** cap) {


        while ((*cap)) {
            Nod* aux=*cap;
            *cap=(*cap)->next;
            //sau Nod* aux=*cap;
            free(aux->info.marca);
            free(aux);
        }
}
//HashTable
typedef struct HashTable HashTable;
struct HashTable {
    int dim;
    Nod** v;
};

HashTable initializareHashTable(int dimensiune) {
    HashTable ht;
    ht.dim=dimensiune;
    ht.v=(Nod**)malloc(sizeof(Nod*)*dimensiune);
    for (int i=0;i<dimensiune;i++) {
        ht.v[i]=NULL;
    }

    return ht;
}

int calculeazaHash(const char* nume,int dimensiune) {
    if (dimensiune==0) {
        return 0;
    }
    else {

        int s=0;
        for (int i=0;i<strlen(nume);i++)
            s=s+nume[i];
            s=s%dimensiune;
        return s%dimensiune;

    }

}

void inserareMasinaInTabela(HashTable ht,Masina MasinaNoua) {
    int poz=calculeazaHash(MasinaNoua.marca,ht.dim);
    if (ht.v[poz]==NULL) {
        Nod* nou=(Nod*)malloc(sizeof(Nod));
        nou->info=MasinaNoua;
        nou->next=NULL;
        ht.v[poz]=nou;
        
    }
    else {
        adaugaMasinaInListaSimpla(&ht.v[poz],MasinaNoua);
    }
}

void dezalocareTabelaMasini(HashTable* ht) {
    for (int i=0;i<ht->dim;i++) {
        Nod* p=ht->v[i];
        while (p) {
            Nod* temp=p;
            p=p->next;
            free(temp->info.marca);
            free(temp);
        }
        ht->v[i]=NULL;

    }
    free(ht->v);
    ht->v=NULL;
    printf("AM DEZALOCAT HASHTABLE");
}

void afisareHashTable(HashTable ht) {
    for (int i=0;i<ht.dim;i++) {
        printf("\nClusterul: %d \n",i);
        afisareListaSimplaMasini(ht.v[i]);
    }
}





//lista dublu inlantuita
/*
typedef struct Nod Nod;
struct Nod {
    Masina info;
    Nod* prev;
    Nod* next;
};

typedef struct Lista Lista;
struct Lista {
    Nod* prim;
    Nod* ultim;
};

void adaugaMasinaInListaDublu(Lista* lista,Masina MasinaNoua) {
    Nod* nou=(Nod*)malloc(sizeof(Nod));
    nou->info=MasinaNoua;
    nou->next=NULL;
    nou->prev=lista->ultim;
    if (lista->ultim) {
        lista->ultim->next=nou;
    }
    else {
        lista->prim=nou;
    }
    lista->ultim=nou;
}

void adaugaMasinaLaInceputLD(Lista* lista,Masina MasinaNoua) {
    Nod* nou=(Nod*)malloc(sizeof(Nod));
    nou->info=MasinaNoua;
    nou->prev=NULL;
    nou->next=lista->prim;
    if (lista->prim) {
        lista->prim->prev=nou;
    }
    else {
        lista->ultim=nou;
    }
    lista->prim=nou;
}

void stergeDinListaDublaId(Lista* lista,int id) {
    Nod* temp=lista->prim;
    while (temp) {
        if (temp->info.id==id) {
            if (temp->prev) {
                temp->prev->next=temp->next;
            }
            else {
                lista->prim=temp->next;
            }
            if (temp->next) {
                temp->next->prev=temp->prev;
            }
            else {
                lista->ultim=temp->prev;
            }
            free(temp->info.marca);
            free(temp);
            return;
        }
        temp=temp->next;
    }
}
void afiseazaListaDublu(Lista lista) {
    Nod* aux=lista.prim;
    while (aux) {
        afisareMasina(aux->info);
        aux=aux->next;
    }
}
void dezalocaLD(Lista* lista) {
    Nod* temp=lista->prim;
    while (temp) {
        Nod* aux=temp;
        temp=temp->next;
        free(aux->info.marca);
        free(aux);
    }
    lista->prim=NULL;
    lista->ultim=NULL;

}
*/



int main() {
    FILE* f=fopen("/Users/mateiavram/Desktop/facultate/Structuri de Date/practice/masini.txt","r");
    if (f) {
        Masina m2=citesteMasinaDinFisier(f);
        afisareMasina(m2);

    }
    Masina m=initializareMasina(1,"BMW",3000.2222);
    Masina m1=initializareMasina(2,"Mercedes",2000);
    //afisareMasina(m);



    //vector
    //adaugaMasinaInVector(&v,&nr,m);
    //afiseazaMasiniVector(v,nr);


    //lista simplu inlantuita
    /*Nod* cap=NULL;
    adaugaMasinaInListaSimpla(&cap,m);
    adaugaMasinaInListaSimpla(&cap,m1);
    stergeMasinaDupaSerieLS(&cap,1);
    afisareListaSimplaMasini(cap);
    dezalocaListaSimpla(&cap);*/


    /*//HashTable
    HashTable ht;
    ht=initializareHashTable(5);
    inserareMasinaInTabela(ht,m);
    inserareMasinaInTabela(ht,m1);
    afisareHashTable(ht);
    dezalocareTabelaMasini(&ht);*/





    //lista dublu inlantuita
    /*Lista lista;
    lista.ultim=NULL;
    adaugaMasinaInListaDublu(&lista,m);
    adaugaMasinaInListaDublu(&lista,m1);

    stergeDinListaDublaId(&lista,2);

    afiseazaListaDublu(lista);
    dezalocaLD(&lista);*/








    return 0;


}
