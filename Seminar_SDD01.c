#include<stdio.h>
#include<stdlib.h>

struct  Masina {
	int id;
	int nrLocuri;
	char* marca;
	float capacitate_c;
	char normaPoluare;
};

struct Masina initializare(int id, int nrLocuri,const char* marca, float capacitate_c,char  normaPoluare) {
	struct Masina m;
	m.id = 1;
	m.nrLocuri = nrLocuri;
	m.capacitate_c = capacitate_c;
	m.normaPoluare = normaPoluare;
	m.marca = (char*)malloc(strlen(marca) + 1);
	strcpy_s(m.marca, strlen(marca)+1,marca);
	return m;
}

void afisare(struct Masina m) {
	printf("AFISARE:\n");
	printf("Marca: %s\n", m.marca);
	printf("ID: %d\n", m.id);
	printf("Numar Locuri: %d\n", m.nrLocuri);
	printf("Capacitate cilindrica: %4f\n", m.capacitate_c);
	printf("Norma Poluare(EURO): %c\n", m.normaPoluare);
	printf("\n\n\n");
}

void modifica_Atribut(struct Masina* m, int valoare) {
	if(valoare>0)
	m->nrLocuri=valoare;


}

void dezalocare(struct Masina* m) {
	free(m->marca);
	(*m).marca = NULL;


}

int main() {
	struct Masina masina;
	masina = initializare(1, 5, "Dacia", 1.5, '4'); //"4" '4' 
	afisare(masina);

	modifica_Atribut(&masina, 10);


	printf( "gata");
	return 0;
}
