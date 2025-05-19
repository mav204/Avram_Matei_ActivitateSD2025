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
typedef struct Nod Nod;
struct Nod {
	Masina info;
	Nod* next;
};
//STACK
//Alegeti prin ce veti reprezenta stiva si creati structura necesara acestei stive
//putem reprezenta o stiva prin LSI, LDI sau vector
void pushStack(Nod** stiva, Masina masina) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = masina;
	nou->next = (*stiva);
	(*stiva) = nou;
}

Masina popStack(Nod** stiva) {
	if ((*stiva) != NULL) {
		Masina nou = (*stiva)->info;
		Nod* aux = (*stiva);
		(*stiva) = (*stiva)->next;
		free(aux);
		return nou;
	}
	Masina masina;
	masina.id = -1;
	return masina;
}

unsigned char emptyStack(Nod* stiva) {
	return stiva == NULL;
}

Nod* citireStackMasiniDinFisier(const char* numeFisier) {
	Nod* stiva = NULL;
	FILE* f = fopen(numeFisier, "r");
	while (!feof(f)) {
		Masina masina = citireMasinaDinFisier(f);
		pushStack(&stiva, masina);
	}
	fclose(f);
	return stiva;
}

void dezalocareStivaDeMasini(Nod** stiva) {
	while ((*stiva) != NULL) {
		Masina masina = popStack(stiva);
		free(masina.numeSofer);
		free(masina.model);
	}
}

int size(Nod* stiva) {
	int count = 0;
	while (stiva != NULL) {
		count++;
		stiva = stiva->next;
	}
	return count;
}

//QUEUE
//Alegeti prin ce veti reprezenta coada si creati structura necesara acestei cozi
//putem reprezenta o coada prin LSI, LDI sau vector
typedef struct NodDublu NodDublu;
struct NodDublu {
	Masina info;
	NodDublu* next;
	NodDublu* prev;
};

typedef struct ListaDubla ListaDubla;
struct ListaDubla {
	NodDublu* primul;
	NodDublu* ultimul;
};

void enqueue(ListaDubla* coada, Masina masina) {
	NodDublu* nou = (NodDublu*)malloc(sizeof(NodDublu));
	nou->info = masina;
	nou->prev = coada->ultimul;
	nou->next = NULL;
	if (coada->ultimul != NULL) {
		coada->ultimul->next = nou;
	}
	else {
		coada->primul = nou;
	}
	coada->ultimul = nou;
}

Masina dequeue(ListaDubla* lista) {
	Masina masina;
	masina.id = -1;
	if (lista->primul != NULL) {
		NodDublu* aux = lista->primul;
		masina = lista->primul->info;
		if (lista->primul->next != NULL) {
			lista->primul = lista->primul->next;
			lista->primul->prev = NULL;
		}
		else {
			lista->primul = NULL;
			lista->ultimul = NULL;
		}
		free(aux);
	}
	return masina;


}

ListaDubla citireCoadaDeMasiniDinFisier(const char* numeFisier) {
	ListaDubla lista;
	lista.primul = NULL;
	lista.ultimul = NULL;
	FILE* f = fopen(numeFisier, "r");
	while (!feof(f)) {
		Masina masina = citireMasinaDinFisier(f);
		enqueue(&lista, masina);
	}
	fclose(f);
	return lista;
}

void dezalocareCoadaDeMasini(ListaDubla* coada) {
	NodDublu* current = coada->primul;
	while (current != NULL) {
		NodDublu* next = current->next;
		if (current->info.model != NULL)
			free(current->info.model);
		if (current->info.numeSofer != NULL)
			free(current->info.numeSofer);
		free(current);
		current = next;
	}
	coada->primul = NULL;
	coada->ultimul = NULL;
}


//metode de procesare
Masina getMasinaByID(ListaDubla coada, int id) {
	NodDublu* aux = coada.primul;
	while (aux) {
		if (aux->info.id == id) {
			return aux->info;
		}
		aux = aux->next;
	}
	Masina masina;
	masina.id = -1;
	masina.numeSofer = NULL;
	masina.model = NULL;
	return masina;
}

float calculeazaPretTotal(ListaDubla* coada) {
	float pretTotal = 0;
	ListaDubla listaNoua;
	listaNoua.primul = NULL;
	listaNoua.ultimul = NULL;
	while (coada->primul) {
		Masina masina = dequeue(coada);
		pretTotal += masina.pret;
		enqueue(&listaNoua, masina);
	}
	coada->primul = listaNoua.primul;
	coada->ultimul = listaNoua.ultimul;
	return pretTotal;
}

int main() {
	Nod* stiva = citireStackMasiniDinFisier("/Users/alexantonescu/Documents/SEMINAR-SDD11/masini.txt");
	int nrElemente = size(stiva);
	printf("\n%d\n", nrElemente);
	Masina m = popStack(&stiva);
	afisareMasina(m);

	ListaDubla coada = citireCoadaDeMasiniDinFisier("/Users/alexantonescu/Documents/SEMINAR-SDD11/masini.txt");
	Masina masina = dequeue(&coada);
	afisareMasina(masina);
	printf("suma: %.2f ", calculeazaPretTotal(&coada));
	printf("\n\n\n");
	afisareMasina(getMasinaByID(coada, 5));
	dezalocareCoadaDeMasini(&coada);
	return 0;
}
