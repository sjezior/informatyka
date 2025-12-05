#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <crtdbg.h>
#include <ctype.h>
#include <time.h>

struct obrazPGM
{
	char nazwa_pliku[100];
	char standard[3];
	int szerokosc;
	int wysokosc;
	int glebia;
	int** piksele;
};

void zwalnianiePamieci(struct obrazPGM* oryginal)
{
	for (int i = 0; i < oryginal->wysokosc; i++)
	{
		free(oryginal->piksele[i]);
	}
	free(oryginal->piksele);
}

void pomijanieKomentarzy(FILE* plik)
{
	int znak;
	while ((znak = fgetc(plik)) != EOF)
	{
		if (znak == '#')
		{
			while ((znak = fgetc(plik)) != '\n' && znak != EOF);
		}
		else if (!isspace(znak))
		{
			ungetc(znak, plik);
			break;
		}
	}
}

int lepszyScanfDoInta()
{
	int liczba;
	while (1)
	{
		if (!scanf("%d", &liczba))
		{
			while ('\n' != getchar());
			printf("Sprobuj ponownie: ");
		}
		else return liczba;
	}
}

void odczyt(struct obrazPGM* oryginal, int* czyOdczytany)
{
	printf("\nWprowadz nazwe pliku(np. mojObraz.pgm): ");
	scanf("%99s", oryginal->nazwa_pliku);
	FILE* plik = fopen(oryginal->nazwa_pliku, "r");

	if (plik == NULL)
	{
		printf("\nProgram napotkal problem podczas otwierania pliku!\n");
	}
	else
	{
		pomijanieKomentarzy(plik);
		fscanf(plik, "%2s", oryginal->standard);
		pomijanieKomentarzy(plik);
		fscanf(plik, "%d", &oryginal->szerokosc);
		pomijanieKomentarzy(plik);
		fscanf(plik, "%d", &oryginal->wysokosc);
		pomijanieKomentarzy(plik);
		fscanf(plik, "%d", &oryginal->glebia);

		oryginal->piksele = malloc(oryginal->wysokosc * sizeof(int*));

		for (int i = 0; i < oryginal->wysokosc; i++)
		{
			oryginal->piksele[i] = malloc(oryginal->szerokosc * sizeof(int));
		}

		for (int i = 0; i < oryginal->wysokosc; i++)
		{
			for (int j = 0; j < oryginal->szerokosc; j++)
			{
				pomijanieKomentarzy(plik);
				fscanf(plik, "%d", &oryginal->piksele[i][j]);
			}
		}
		fclose(plik);
		*czyOdczytany = 1;
		printf("\nObraz zostal odczytany.\n");
	}
}

void zapis(char* nazwa_pliku, struct obrazPGM* oryginal, int* czyOdczytany)
{
	FILE* plik = fopen(nazwa_pliku, "w");

	if (plik == NULL)
	{
		printf("\nProgram napotkal problem podczas otwierania pliku!\n");
	}
	else
	{
		fprintf(plik, "%s\n", oryginal->standard);
		fprintf(plik, "%d %d\n", oryginal->szerokosc, oryginal->wysokosc);
		fprintf(plik, "%d\n", oryginal->glebia);

		for (int i = 0; i < oryginal->wysokosc; i++)
		{
			for (int j = 0; j < oryginal->szerokosc; j++)
			{
				fprintf(plik, "%d ", oryginal->piksele[i][j]);
			}
			fprintf(plik, "\n");
		}
		fclose(plik);
		*czyOdczytany = 0;
		printf("\nObraz o nazwie nowy.pgm zostal utworzony.\n");
	}
}

void odbicie(struct obrazPGM* oryginal)
{
	int menu = -1;
	do
	{
		printf("\n----------- ODBCIE -----------\n1. Wzgledem osi poziomej\n2. Wzgledem osi pionowej\n0. Anuluj\n----------------------------\nWybierz opcje: ");
		menu = lepszyScanfDoInta();
		switch (menu)
		{
		case 0:
			break;
		case 1:
		{
			int** pomocnicza;
			pomocnicza = malloc(oryginal->wysokosc * sizeof(int*));

			for (int i = 0; i < oryginal->wysokosc; i++)
			{
				pomocnicza[i] = malloc(oryginal->szerokosc * sizeof(int));
			}

			for (int i = 0; i < oryginal->wysokosc; i++)
			{
				for (int j = 0; j < oryginal->szerokosc; j++)
				{
					pomocnicza[i][j] = oryginal->piksele[oryginal->wysokosc - 1 - i][j];
				}
			}

			zwalnianiePamieci(oryginal);
			oryginal->piksele = pomocnicza;
			printf("\nOdbicie obrazu wzgledem osi poziomej zostalo przeprowadzone.\n");
			break;
		}
		case 2:
		{
			int** pomocnicza;
			pomocnicza = malloc(oryginal->wysokosc * sizeof(int*));

			for (int i = 0; i < oryginal->wysokosc; i++)
			{
				pomocnicza[i] = malloc(oryginal->szerokosc * sizeof(int));
			}

			for (int i = 0; i < oryginal->wysokosc; i++)
			{
				for (int j = 0; j < oryginal->szerokosc; j++)
				{
					pomocnicza[i][j] = oryginal->piksele[i][oryginal->szerokosc - 1 - j];
				}
			}

			zwalnianiePamieci(oryginal);
			oryginal->piksele = pomocnicza;
			printf("\nOdbicie obrazu wzgledem osi pionowej zostalo przeprowadzone.\n");
			break;
		}
		default:
			printf("\nNieprawidlowa opcja!\n");
		}
	} while (menu == -1);
}

void szum(struct obrazPGM* oryginal)
{
	printf("Wprowadz procentowa wartosc prawdopodobienstwa zaszumienia: ");
	int prawdopodobienstwo = lepszyScanfDoInta();

	for (int i = 0; i < oryginal->wysokosc; i++)
	{
		for (int j = 0; j < oryginal->szerokosc; j++)
		{
			if ((rand() % 100) < prawdopodobienstwo)
			{
				if (rand() % 2)
				{
					oryginal->piksele[i][j] = 0;
				}
				else
				{
					oryginal->piksele[i][j] = oryginal->glebia;
				}
			}
		}
	}
	printf("\nObraz zostal zaszumiony.\n");
}

void negatyw(struct obrazPGM* oryginal)
{
	for (int i = 0; i < oryginal->wysokosc; i++)
	{
		for (int j = 0; j < oryginal->szerokosc; j++)
		{
			oryginal->piksele[i][j] = oryginal->glebia - oryginal->piksele[i][j];
		}
	}
}

void histogram(struct obrazPGM* oryginal, char* nazwa_pliku)
{
	FILE* plik = fopen(nazwa_pliku, "w");

	if (plik == NULL)
	{
		printf("\nProgram napotkal problem podczas otwierania pliku!\n");
	}
	else
	{
		int* tabGlebia = malloc((oryginal->glebia + 1) * sizeof(int));

		for (int i = 0; i <= oryginal->glebia; i++) {
			tabGlebia[i] = 0;
		}

		for (int i = 0; i < oryginal->wysokosc; i++)
		{
			for (int j = 0; j < oryginal->szerokosc; j++)
			{
				tabGlebia[oryginal->piksele[i][j]] += 1;
			}
		}

		for (int i = 0; i <= oryginal->glebia; i++)
		{
			fprintf(plik, "%d;%d\n", i, tabGlebia[i]);
		}

		fclose(plik);
		free(tabGlebia);
		printf("\nPlik histogram.csv zostal utworzony.\n");
	}
}

int main()
{
	srand(time(NULL));
	struct obrazPGM oryginal;
	oryginal.piksele = NULL;
	int czyOdczytany = 0;

	int menu = -1;
	while (menu != 0)
	{
		printf("\n----------- MENU -----------\n1. Odczyt\n2. Odbicie\n3. Wprowadzenie szumu\n4. Zapis\n5. Negatyw\n6. Histogram\n0. Zakonczenie programu\n----------------------------\nWybierz opcje: ");
		menu = lepszyScanfDoInta();
		switch (menu)
		{
		case 0:
		{
			printf("\nZakonczenie programu.\n");
			break;
		}
		case 1:
		{
			if (oryginal.piksele != NULL)
			{
				zwalnianiePamieci(&oryginal);
			}
			odczyt(&oryginal, &czyOdczytany);
			break;
		}
		case 2:
		{
			if (czyOdczytany)
			{
				odbicie(&oryginal);
			}
			else
			{
				printf("\nMusisz najpierw odczytac plik!\n");
			}
			break;
		}
		case 3:
		{
			if (czyOdczytany)
			{
				szum(&oryginal);
			}
			else
			{
				printf("\nMusisz najpierw odczytac plik!\n");
			}
			break;
		}
		case 4:
		{
			if (czyOdczytany)
			{
				zapis("nowy.pgm", &oryginal, &czyOdczytany);
				zwalnianiePamieci(&oryginal);
				_CrtDumpMemoryLeaks();
			}
			else
			{
				printf("\nMusisz najpierw odczytac plik!\n");
			}
			break;
		}
		case 5:
		{
			if (czyOdczytany)
			{
				negatyw(&oryginal);
			}
			else
			{
				printf("\nMusisz najpierw odczytac plik!\n");
			}
			break;
		}
		case 6:
		{
			if (czyOdczytany)
			{
				histogram(&oryginal, "histogram.csv");
			}
			else
			{
				printf("\nMusisz najpierw odczytac plik!\n");
			}
			break;
		}
		default:
		{
			printf("\nNieprawidlowa opcja!\n");
		}
		}
	}
}