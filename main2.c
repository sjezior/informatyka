#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>

void pobierz_rozmiar(int* rozmiar) //pobranie ilosci probek dla ustalenia rozmiaru tablicy
{
	printf("Podaj ilosc probek sygnalu (minimum 10): ");
	scanf("%d", rozmiar);
	while (*rozmiar < 10)
	{
		printf("Nieprawidlowa wartosc! Podaj ilosc probek sygnalu ponownie: ");
		scanf("%d", rozmiar);
	}

}

void pobierz_granice(float* dolna, float* gorna) //pobranie granic x
{
	printf("Podaj dolna granice: ");
	scanf("%f", dolna);

	printf("Podaj gorna granice: ");
	scanf("%f", gorna);

	while (*gorna <= *dolna)
	{
		printf("Gorna granica musi byc wieksza od dolnej. Wprowadz wartosc jeszcze raz: ");
		scanf("%f", gorna);
	}

}

void pobierz_wspolczynniki(float* wsp) //pobranie wspolczynnikow funkcji
{
	for (int i = 0; i < 4; i++)
	{
		printf("Podaj wartosc %d wspolczynnika: ", i+1);
		scanf("%f", &wsp[i]);
	}
}

void sygnal(float* wspolczynniki, float* probki, float min, float max, int n) //generowanie sygnalu podstawowego
{
	float dx = (max - min) / n;
	float x = min;
	for (int i = 0; i < n; i++)
	{
		float y = wspolczynniki[0] * sinf((x + wspolczynniki[2]) / wspolczynniki[1]) + wspolczynniki[3];
		probki[i] = y;
		x += dx;
	}
}

void szum(float* probki, float* zaszumiony, int n) //zaszumienie sygnalu
{
	float amplituda;
	printf("Podaj amplitude szumu: ");
	scanf("%f", &amplituda);

	for (int i = 0; i < n; i++)
	{
		zaszumiony[i] = probki[i];
	}

	for (int i = 0; i < n; i++)
	{
		if (rand() % 3 == 0)
		{
			zaszumiony[i] += (float)rand() / RAND_MAX * (2 * amplituda) - amplituda;
		}
	}
}

void filtrSrednia(float* probki, float* probki_filtrowane, int n) //filtr sredniej ruchomej
{
	probki_filtrowane[0] = probki[0];
	probki_filtrowane[1] = probki[1];
	probki_filtrowane[n - 1] = probki[n - 1];
	probki_filtrowane[n - 2] = probki[n - 2];
	for (int i = 2; i < n - 2; i++)
	{
		float srednia = (probki[i - 2] + probki[i - 1] + probki[i] + probki[i + 1] + probki[i + 2])/5;
		probki_filtrowane[i] = srednia;
	}
}

void porownanie(float* probki, float* probki_filtrowane, float* roznica, int n) //porownanie sygnalu podstawowego do sygnalu po filtracji
{
	float sredni_blad = 0;
	for (int i = 0; i < n; i++)
	{
		roznica[i] = probki_filtrowane[i] - probki[i];
		sredni_blad += roznica[i] / probki[i];
		
	}
	sredni_blad = fabs(sredni_blad / n)*100;
	printf("Sredni blad filtracji wyniosl: %f%%\n", sredni_blad);
}

void zapisz(char* nazwa_pliku, float* probki, float* probki_filtrowane, float* zaszumiony, float* roznica, int n, float min, float max) //zapis do pliku
{
	FILE* plik = fopen(nazwa_pliku, "w");

	if (plik == NULL)
	{
		printf("\nProgram napotkal problem podczas otwierania pliku!\n");
	}
	else
	{
		float dx = (max - min) / n;
		float x = min;

		for (int i = 0; i < n; i++)
		{
			fprintf(plik, "%f;%f;;%f;%f;;%f;%f;;%f\n", x, probki[i], x, zaszumiony[i], x, probki_filtrowane[i], roznica[i]);
			x += dx;
		}
		fclose(plik);
		printf("\nZapisano sygnal.\n");
	}
}

int main()
{
	setlocale(LC_ALL, "polish_poland");
	srand(time(NULL));
	float wspolczynniki[4];
	float xmin;
	float xmax;
	int rozmiar;
	float* probki = NULL;
	float* probki_filtrowane = NULL;
	float* zaszumiony = NULL;
	float* roznica = NULL;
	int czyJestSygnal = 0;
	int czyJestSzum = 0;

	int menu = -1;
	while (menu != 0) //menu programu
	{
		printf("\n----------- MENU -----------\n1. Generowanie sygnalu podstawowego\n2. Wprowadzanie szumu do sygnalu\n3. Filtrowanie za pomoca sredniej ruchomej\n4. Zapis do pliku\n0. Zakonczenie programu\n----------------------------\nWybierz opcje: ");
		scanf("%d", &menu);
		switch (menu)
		{
		case 0:
			printf("\nZakonczenie programu.\n");
			break;
		case 1:
			pobierz_rozmiar(&rozmiar);
			probki = malloc(rozmiar * sizeof(float));
			pobierz_granice(&xmin, &xmax);
			pobierz_wspolczynniki(wspolczynniki);
			sygnal(wspolczynniki, probki, xmin, xmax, rozmiar);
			printf("\nWygenerowano sygnal.\n");
			czyJestSygnal = 1;
			break;
		case 2:
			if (czyJestSygnal == 1)
			{
				zaszumiony = malloc(rozmiar * sizeof(float));
				szum(probki, zaszumiony, rozmiar);
				printf("\nSygnal zostal zaszumiony.\n");
				czyJestSzum = 1;
			}
			else
			{
				printf("\nMusisz najpierw wygenerowac sygnal!");
			}
			break;
		case 3:
			if (czyJestSzum == 1)
			{
				probki_filtrowane = malloc(rozmiar * sizeof(float));
				roznica = malloc(rozmiar * sizeof(float));
				filtrSrednia(probki, probki_filtrowane, rozmiar);
				printf("\nProces filtracji dla twojego sygnalu zostal przeprowadzony.\n");
				porownanie(probki, probki_filtrowane, roznica, rozmiar);
			}
			else
			{
				printf("\nNajpierw musisz wprowadzic szum do sygnalu!\n");
			}
			break;
		case 4:
			if (czyJestSygnal == 1)
			{
				zapisz("sygnal.csv", probki, probki_filtrowane, zaszumiony, roznica, rozmiar, xmin, xmax);
			}
			else
			{
				printf("\nMusisz najpierw wygenerowac sygnal!");
			}
			break;
		default:
			printf("\nNieprawidlowa opcja!\n");
		}
	}
	free(probki);
	free(zaszumiony);
	free(probki_filtrowane);
}