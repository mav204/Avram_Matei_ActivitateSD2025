#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

typedef struct NodListaPrincipala NodListaPrincipala;
typedef struct NodListaSecundara NodListaSecundara;

struct NodListaPrincipala {
	NodListaPrincipala* next;
	NodListaSecundara* vecini;
	Masina info;
};


struct NodListaSecundara {
	NodListaPrincipala* info;
	NodListaSecundara* next;
};






Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy(m1.model, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy(m1.numeSofer, aux);

	m1.serie = *strtok(NULL, sep);
	return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

//1.
// structuri necesare
//dorim stocarea unui graf intr-o lista de liste
//astfel avem nod ListaPrincipala si NodListaSecundara

//2.
//functii de inserare in liste
//si in principala si in secundara

NodListaPrincipala* inserareListaPrincipala(NodListaPrincipala* cap,Masina info) {
	NodListaPrincipala* nou=(NodListaPrincipala*)malloc(sizeof(NodListaPrincipala));
	nou->info=info;
	nou->vecini=NULL;
	nou->next=NULL;
	if (!cap) return nou;
	NodListaPrincipala* p=(NodListaPrincipala*)malloc(sizeof(NodListaPrincipala));
	while (p->next) {
		p=p->next;
	}
	p->next=nou;
	return cap;
}

//inserare in lista secundara , o facem la inceput pentru ca e mai usor pentru ca n are importanta acolo unde avem lista de muchii
NodListaSecundara* inserareListaSecundara(NodListaSecundara* cap,NodListaPrincipala* info) {
	NodListaSecundara* nou=(NodListaSecundara*)malloc(sizeof(NodListaSecundara));
	nou->info=info;
	nou->next=NULL;

	nou->next=cap;
	cap=nou; //linia x
	return cap;
	//sau direct return nou si comentam linia x

}

//3.
//functie de cautarea in lista principala dupa ID
NodListaPrincipala* cautaNodDupaID(NodListaPrincipala* listaPrincipala, int id) {

	while (listaPrincipala!=NULL && listaPrincipala->info.id!=id) {
		listaPrincipala=listaPrincipala->next;
		}
	//daca nu gaseste va da null
	//ca sa nu mai verificam daca am gasit sau nu

	return listaPrincipala;
}

//4.
//inserare muchie
void inserareMuchie(NodListaPrincipala* listaPrincipala, int idStart, int idStop) {

	NodListaPrincipala* nodStart=cautaNodDupaID(listaPrincipala,idStart);
	NodListaPrincipala* nodStop=cautaNodDupaID(listaPrincipala,idStop);
	if (nodStart && nodStop) {
		nodStart=inserareListaSecundara(nodStart->vecini,nodStop);
		nodStop=inserareListaSecundara(nodStop->vecini,nodStart);
	}
}


NodListaPrincipala* citireNoduriMasiniDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaDinFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul

	NodListaPrincipala* lista=NULL;
	FILE* f=fopen(numeFisier,"r");
	while (!feof(f)) {
		Masina m=citireMasinaDinFisier(f);
		inserareListaPrincipala(lista,m);
	}
	fclose(f);
	return lista;
}

void citireMuchiiDinFisier(NodListaPrincipala* lista,const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste
	//toate id-urile de start si stop pentru fiecare muchie
	//ATENTIE - la final inchidem fisierul/stream-ul
	FILE* f=fopen(numeFisier,"r");
	while (!feof(f)) {
		int idStart;
		int idStop;
		fscanf(f,"%d %d",&idStart,&idStop);
		inserareMuchie(lista,idStart,idStop);
	}
	fclose(f);

}

void stergereListaSecundara(NodListaSecundara** cap) {
	NodListaSecundara* aux;
	while ((*cap)) {
		aux=(*cap)->next;
		free(*cap);
		(*cap)=aux;
	}

}

void dezalocareNoduriGraf(NodListaPrincipala** listaPrincipala) {
	//sunt dezalocate toate masinile din graf
	//si toate nodurile celor doua liste
	NodListaPrincipala* aux;
	while (*listaPrincipala) {
		aux=(*listaPrincipala)->next;
		stergereListaSecundara(&(*listaPrincipala)->vecini);
		if ((*listaPrincipala)->info.numeSofer) {
			free((*listaPrincipala)->info.numeSofer);
		}

		if ((*listaPrincipala)->info.model) {
			free((*listaPrincipala)->info.model);
		}
		free(*listaPrincipala);
		(*listaPrincipala)=aux;
	}
}

int main() {

	NodListaPrincipala* graf=citireNoduriMasiniDinFisier("/Users/mateiavram/Desktop/facultate/Structuri de Date/SEMINAR11_SDD_Grafuri/masini.txt");
	citireMuchiiDinFisier(graf,"/Users/mateiavram/Desktop/facultate/Structuri de Date/SEMINAR11_SDD_Grafuri/muchii.txt");
	dezalocareNoduriGraf(&graf);
	return 0;
}
