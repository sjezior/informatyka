dodac do operatorow const w nawiasie i na końcu

Harmonogram(const Harmonogram& inny);
Harmonogram& operator=(const Harmonogram& inny);
Harmonogram kopiuj(const Czas& limit);

Harmonogram Harmonogram::kopiuj(const Czas& limit) {
	Harmonogram nowy;
	Czas suma;

	for (int i = 0; i < rozmiar; i++) {
		if (limit < suma + tablica[i])
			break;

		suma = suma + tablica[i];
		nowy.dodajCzas(tablica[i]);
	}

	return nowy;
}

Harmonogram::Harmonogram(const Harmonogram& inny) {
	rozmiar = inny.rozmiar;
	if (rozmiar > 0) {
		tablica = new Czas[rozmiar];
		for (int i = 0; i < rozmiar; i++) {
			tablica[i] = inny.tablica[i];
		}
	}
	else {
		tablica = nullptr;
	}
}

Harmonogram& Harmonogram::operator=(const Harmonogram& inny) {
	if (&inny == this) {
		return *this;
	}

	delete[] tablica;

	rozmiar = inny.rozmiar;

	if (rozmiar > 0) {
		tablica = new Czas[rozmiar];
		for (int i = 0; i < rozmiar; i++) {
			tablica[i] = inny.tablica[i];
		}
	}
	else {
		tablica = nullptr;
	}
	
	return *this;
}
