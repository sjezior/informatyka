#include "har.h"

Harmonogram& Harmonogram::operator-=(int n) {
	if (n <= 0) {
		return *this;
	}

	int nowyRozmiar = rozmiar - n;

	if (nowyRozmiar <= 0) {
		delete[] tablica;
		tablica = nullptr;
		rozmiar = 0;
	}
	else {
		Czas* nowaTablica = new Czas[nowyRozmiar];

		for (int i = 0; i < nowyRozmiar; i++) {
			nowaTablica[i] = tablica[i];
		}

		delete[] tablica;
		tablica = nowaTablica;
		rozmiar = nowyRozmiar;
	}
}

Harmonogram Harmonogram::kopiuj(Czas limit) {
	Harmonogram nowy;
	Czas suma;

	for (int i = 0; i < rozmiar; i++) {
		suma = suma + tablica[i];
		if (!(limit < suma)) {
			nowy.dodajCzas(tablica[i]);
		}
		else break;
	}
	return nowy;
}

Harmonogram& Harmonogram::operator=(const Harmonogram& inny) {
	if (&inny == this) {
		return *this;
	}
	delete[] tablica;
	rozmiar = inny.rozmiar;
	tablica = new Czas[rozmiar];
	for (int i = 0; i < rozmiar; i++) {
		tablica[i] = inny.tablica[i];
	}
}

Harmonogram::Harmonogram(const Harmonogram& inny) {
	rozmiar = inny.rozmiar;
	tablica = new Czas[rozmiar];
	for (int i = 0; i < rozmiar; i++) {
		tablica[i] = inny.tablica[i];
	}
}

Czas& Harmonogram::zwrocRef(int indeks) {
	return 	 tablica[indeks];
}

void Harmonogram::dodajCzas(Czas& c) {
	Czas* nowa = new Czas[rozmiar + 1];
	for (int i = 0; i < rozmiar; i++) {
		nowa[i] = tablica[i];
	}
	nowa[rozmiar] = c;
	delete[] tablica;
	tablica = nowa;
	rozmiar++;
}

int Harmonogram::ileCzasow() {
	return rozmiar;
}

void Harmonogram::wypisz() {
	for (int i = 0; i < rozmiar; i++) {
		std::cout << "(" << i + 1 << ") ";
		tablica[i].wypiszCzas();
	}
}

Harmonogram::Harmonogram() {
	tablica = nullptr;
	rozmiar = 0;
}

Harmonogram::~Harmonogram() {
	delete[] tablica;
}
