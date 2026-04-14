
#include "czas.h"

bool Czas::operator<(Czas& _czas) {
	if (godzina < _czas.godzina) {
		return true;
	}
	else if ((godzina == _czas.godzina) && (minuta < _czas.minuta)) {
		return true;
	}
	else if ((godzina == _czas.godzina) && (minuta == _czas.minuta) && (sekunda < _czas.sekunda)) {
		return true;
	}
	else {
		return false;
	}
}

Czas Czas::operator+(Czas& _czas) {
	Czas temp = _czas;
	temp.godzina += godzina;
	temp.minuta += minuta;
	temp.sekunda += sekunda;
	if (temp.minuta > 59) {
		temp.minuta -= 60;
		temp.godzina += 1;
	}
	if (temp.sekunda > 59) {
		temp.sekunda -= 60;
		temp.minuta += 1;
	}
	return temp;
}

void Czas::ustawCzas(int _sekunda) {
	if (_sekunda <= 59 && _sekunda >= 0) {
		sekunda = _sekunda;
	}
	else {
		std::cout << "Wprowadzona ilosc sekund musi miescic sie w zakresie 0-59, pozostaje domyslna wartosc rowna 0." << std::endl;
	}
}

void Czas::ustawCzas(int _sekunda, int _minuta) {
	if (_sekunda <= 59 && _sekunda >= 0) {
		sekunda = _sekunda;
	}
	else {
		std::cout << "Wprowadzona ilosc sekund musi miescic sie w zakresie 0-59, pozostaje domyslna wartosc rowna 0." << std::endl;
	}

	if (_minuta <= 59 && _minuta >= 0) {
		minuta = _minuta;
	}
	else {
		std::cout << "Wprowadzona ilosc minut musi miescic sie w zakresie 0-59, pozostaje domyslna wartosc rowna 0." << std::endl;
	}
}

void Czas::ustawCzas(int _sekunda, int _minuta, int _godzina) {
	if (_sekunda <= 59 && _sekunda >= 0) {
		sekunda = _sekunda;
	}
	else {
		std::cout << "Wprowadzona ilosc sekund musi miescic sie w zakresie 0-59, pozostaje domyslna wartosc rowna 0." << std::endl;
	}

	if (_minuta <= 59 && _minuta >= 0) {
		minuta = _minuta;
	}
	else {
		std::cout << "Wprowadzona ilosc minut musi miescic sie w zakresie 0-59, pozostaje domyslna wartosc rowna 0." << std::endl;
	}

	if (_godzina >= 0) {
		godzina = _godzina;
	}
	else {
		std::cout << "Wprowadzona ilosc godzin musi byc liczba nieujemna, pozostaje domyslna wartosc rowna 0." << std::endl;
	}
}

void Czas::ustawGodzina(int _godzina) {
	if (_godzina < 0) {
		std::cout << "Wprowadzona ilosc godzin musi byc liczba nieujemna, pozostaje domyslna wartosc rowna 0." << std::endl;
		return;
	}
	godzina = _godzina;
}

void Czas::ustawMinuta(int _minuta) {
	if (_minuta > 59 && _minuta < 0) {
		std::cout << "Wprowadzona ilosc minut musi miescic sie w zakresie 0-59, pozostaje domyslna wartosc rowna 0." << std::endl;
		return;
	}
	minuta = _minuta;
}

void Czas::ustawSekunda(int _sekunda) {
	if (_sekunda > 59 && _sekunda < 0) {
		std::cout << "Wprowadzona ilosc sekund musi miescic sie w zakresie 0-59, pozostaje domyslna wartosc rowna 0." << std::endl;
		return;
	}
	sekunda = _sekunda;
}

int Czas::pokazGodzina() {
	return godzina;
}

int Czas::pokazMinuta() {
	return minuta;
}

int Czas::pokazSekunda() {
	return sekunda;
}

void Czas::wypiszCzas() {
	std::cout << "Czas: " << godzina << ":" << minuta << ":" << sekunda << std::endl;
}
