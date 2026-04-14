#include "czas.h"
#pragma once

class Harmonogram {
public:
	Harmonogram();
	~Harmonogram();
	void dodajCzas(Czas& c);
	int ileCzasow();
	void wypisz();
	Czas& zwrocRef(int indeks);

	Harmonogram(const Harmonogram& inny);
	Harmonogram& operator=(const Harmonogram& inny);
	Harmonogram kopiuj(const Czas limit);
	Harmonogram& operator-=(int n);

private:
	Czas* tablica;
	int rozmiar;
};
