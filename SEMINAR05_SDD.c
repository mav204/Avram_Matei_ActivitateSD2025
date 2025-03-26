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

//creare structura pentru un nod dintr-o lista dublu inlantuita

struct Nod {
	Masina info;
	struct Nod* urmator;
	struct Nod* precedent;
};

typedef struct Nod Nod;

//creare structura pentru Lista Dubla

struct Lista {
	Nod* primul;
	Nod* ultimul;
};
typedef struct Lista Lista;

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

void afisareListaMasiniInceput(Lista lista) {
	Nod* aux = lista.primul;
	while (aux != NULL) {
		afisareMasina(aux->info);
		aux = aux->urmator;
	}

}

void afisareListaMasiniSF(Lista lista) {
	Nod* aux = lista.ultimul;
	while (aux != NULL) {
		afisareMasina(aux->info);
		aux = aux->precedent;
	}
}

void adaugaMasinaInLista(Lista* lista, Masina masinaNoua) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->precedent = lista->ultimul;
	nou->urmator = NULL;
	if (lista->ultimul) {
		lista->ultimul->urmator = nou;
	}
	else {
		lista->primul = nou;
	}
	lista->ultimul = nou;
}

void adaugaLaInceputInLista(Lista* lista, Masina masinaNoua) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masinaNoua;
	nou->precedent = NULL;
	nou->urmator = lista->primul;
	if (lista->primul) {
		lista->primul->precedent = nou;
	}
	else {
		lista->ultimul = nou;
	}
	lista->primul = nou;
}

Lista citireLDMasiniDinFisier(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	Lista lista;
	lista.primul = NULL;
	lista.ultimul = NULL;
	while (!feof(file)) {
		adaugaMasinaInLista(&lista, citireMasinaDinFisier(file));
	}
	fclose(file);
	return lista;
}

void dezalocareLDMasini(Lista lista) {
	Nod *aux = lista.primul;
	while (aux) {
		Nod *temp = aux;
		aux = aux->urmator;
		free(temp->info.model);
		free(temp->info.numeSofer);
		free(temp);
	}
	lista.primul = NULL;
	lista.ultimul = NULL;
}

float calculeazaPretMediu(Lista lista) {
	Nod* aux = lista.primul;
	float cnt = 0;
	float suma = 0;
	while (aux) {
		cnt++;
		suma += aux->info.pret;
		aux = aux->urmator;
	}
	return suma / cnt;
}

void stergeMasinaDupaID(Lista* lista, int id) {
	Nod *temp = lista->primul;
	while (temp) {
		if (temp->info.id == id) {
			if (temp->precedent) {
				temp->precedent->urmator = temp->urmator;
			} else {
				lista->primul = temp->urmator;
			}

			if (temp->urmator) {
				temp->urmator->precedent = temp->precedent;
			} else {
				lista->ultimul = temp->precedent;
			}

			free(temp->info.model);
			free(temp->info.numeSofer);
			free(temp);
			return;
		}
		temp = temp->urmator;
	}
}

char* getNumeSoferMasinaScumpa(Lista lista) {
	float pretMaxim = 0;
	char* numeSofer = NULL;
	Nod* aux = lista.primul;
	while (aux) {
		if (aux->info.pret > pretMaxim) {
			pretMaxim = aux->info.pret;
			if (numeSofer) {
				free(numeSofer);
			}
			numeSofer = (char*)malloc(strlen(aux->info.numeSofer) + 1);
			strcpy(numeSofer, aux->info.numeSofer);
		}
		aux = aux->urmator;
	}
	return numeSofer;
}

int main() {
	Lista lista;
	lista = citireLDMasiniDinFisier("/Users/alexantonescu/Documents/SDD-SEMINAR5/masini.txt");
	afisareListaMasiniInceput(lista);
	float pretMediu = calculeazaPretMediu(lista);
	printf("Pret mediu: %.2f\n", pretMediu);
	char* numeSofer = getNumeSoferMasinaScumpa(lista);
	printf("Nume sofer: %s\n", numeSofer);
	return 0;
}
