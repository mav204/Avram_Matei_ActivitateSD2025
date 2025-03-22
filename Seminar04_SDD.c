#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul masini.txt
//sau va creati un alt fisier cu alte date

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;
typedef struct Nod Nod;
struct Nod {
	Masina info;
	Nod* urm;

};

//creare structura pentru un nod dintr-o lista simplu inlantuita

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
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

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

void afisareListaMasini(Nod* cap) {
	//afiseaza toate elemente de tip masina din lista simplu inlantuita
	//prin apelarea functiei afisareMasina()
	while (cap != NULL)
	{
		afisareMasina(cap->info);
		cap = cap->urm;

	}
}

void adaugaMasinaInLista(Nod** cap, Masina masinaNoua) {
	//adauga la final in lista primita o noua masina pe care o primim ca parametru
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masinaNoua;
	(*nou).urm = NULL;
	if ((*cap) == NULL)
	{
		*cap = nou;
	}
	else
	{
		Nod* p = *cap;
		while (p->urm)
		{
			p = p->urm;
		}
		p->urm = nou;
	}
}

void adaugaLaInceputInLista(Nod** cap, Masina masinaNoua) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->urm = *cap;
	*cap = nou;


	//adauga la inceputul listei o noua masina pe care o primim ca parametru
}

Nod* citireListaMasiniDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaDinFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul
	FILE* f = fopen(numeFisier,"r");
	Nod* lista = NULL;
	while (!feof(f))
	{
		adaugaMasinaInLista(&lista, citireMasinaDinFisier(f));
	}
	fclose(f);
	return lista;


}

void dezalocareListaMasini(Nod** cap) {
	//sunt dezalocate toate masinile si lista de elemente

	while ((*cap) != NULL)
	{
		Nod* aux = *cap;
		*cap = (*cap)->urm;
		free(aux->info.numeSofer);
		free(aux->info.model);
		free(aux);
	}
}

float calculeazaPretMediu(Nod* lista) {
	//calculeaza pretul mediu al masinilor din lista.
	if(!lista)
	return 0;
	float suma=0;
	int k=0;
	while (lista)
	{
		suma += lista->info.pret;
		k++;
		lista=lista->urm;

	}
	return suma / k;

		
}

void stergeMasiniDinSeria(Nod** cap, char serieCautata) {
	Nod *curent = *cap;
	Nod *prev = NULL;
	while (curent != NULL) {
		if (curent->info.serie == serieCautata) {
			Nod* aux = curent;
			if (prev == NULL) {
				*cap = curent->urm;
			}
			else {
				prev->urm = curent->urm;
			}
			curent = curent->urm;
			free(aux->info.numeSofer);
			free(aux->info.model);
			free(aux);
		}
		else {
			prev = curent;
			curent = curent->urm;
		}
	}
}

float calculeazaPretulMasinilorUnuiSofer(Nod* lista, const char* numeSofer) {
	//calculeaza pretul tuturor masinilor unui sofer.
	float suma = 0;
	while (!lista)
	{	

		if (strcmp(numeSofer, lista->info.numeSofer) == 0)
			suma += lista->info.pret;

		lista = lista->urm;

	}
	return suma;
}

char* getCeaMaiScumpaMasina(Nod* lista)
{
	float max=0;
	char* modelScump=NULL;

	while (lista)
	{
		if (lista->info.pret > max)
		{
			max = lista->info.pret;
			modelScump = lista->info.model;
		}

		lista = lista->urm;
	}
	if (max > 0)
	{
		char* aux = malloc(strlen(modelScump) + 1);
		strcpy_s(aux, strlen(modelScump) + 1, modelScump);
		return aux;
	}
	return NULL;
}

int main() {

	Nod* cap = NULL;
	cap=citireListaMasiniDinFisier("masini.txt");
	afisareListaMasini(cap);

	float k=calculeazaPretMediu(cap);
	printf("%.2f\n", k);
	
	//k = calculeazaPretulMasinilorUnuiSofer(cap, "Ionescu");
	printf("%.2f\n", calculeazaPretulMasinilorUnuiSofer(cap, "Ionescu"));

	printf("%s", getCeaMaiScumpaMasina(cap));
	
	dezalocareListaMasini(&cap);
	

 	return 0;
}
