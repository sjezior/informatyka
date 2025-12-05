#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include <crtdbg.h>

struct obrazPGM
{
    char nazwa_pliku[100];
    char standard[3];
    int szerokosc;
    int wysokosc;
    int glebia;
    int** piksele;
};

/* -- oryginalne funkcje z minimalnymi korektami stylistycznymi, logika bez zmian -- */
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

void odczyt(struct obrazPGM* oryginal)
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
        oryginal->piksele = (int**)malloc(oryginal->wysokosc * sizeof(int*));
        for (int i = 0; i < oryginal->wysokosc; i++)
        {
            oryginal->piksele[i] = (int*)malloc(oryginal->szerokosc * sizeof(int));
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
        printf("\nObraz zostal odczytany.\n");
    }
}

void zapis(char* nazwa_pliku, struct obrazPGM* oryginal)
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
        printf("\nObraz o nazwie %s zostal utworzony.\n", nazwa_pliku);
    }
}

void odbicie(struct obrazPGM* oryginal)
{
    int menu = -1;
    do
    {
        printf("\n----------- ODBCIE -----------\n1. Wzgledem osi poziomej\n2. Wzgledem osi pionowej\n0. Anuluj\n------------------------------\nWybierz opcje: ");
        menu = lepszyScanfDoInta();
        switch (menu)
        {
        case 0:
            break;
        case 1:
        {
            int** pomocnicza;
            pomocnicza = (int**)malloc(oryginal->wysokosc * sizeof(int*));
            for (int i = 0; i < oryginal->wysokosc; i++)
            {
                pomocnicza[i] = (int*)malloc(oryginal->szerokosc * sizeof(int));
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
            pomocnicza = (int**)malloc(oryginal->wysokosc * sizeof(int*));
            for (int i = 0; i < oryginal->wysokosc; i++)
            {
                pomocnicza[i] = (int*)malloc(oryginal->szerokosc * sizeof(int));
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

/* ================== DODANE: minimalna baza obrazow ================== */

void listaObrazow(struct obrazPGM* baza, int rozmiar, int wybrany)
{
    if (rozmiar == 0) { printf("Baza jest pusta.\n"); return; }
    printf("\nLista obrazow:\n");
    for (int i = 0; i < rozmiar; ++i)
    {
        printf(" [%d] %s  %dx%d\n", i, baza[i].nazwa_pliku, baza[i].szerokosc, baza[i].wysokosc);
    }
}

int dodajObrazDoBazy(struct obrazPGM** baza, int* rozmiar, int* wybrany)
{
    struct obrazPGM nowy; nowy.piksele = NULL; nowy.nazwa_pliku[0] = '\0';
    odczyt(&nowy); // korzystamy z Twojej funkcji (pobiera nazwe z wejscia)
    if (!nowy.piksele) return 0;

    struct obrazPGM* temp = (struct obrazPGM*)realloc(*baza, (*rozmiar + 1) * sizeof(struct obrazPGM));
    if (!temp)
    {
        // brak miejsca – sprzatanie wczytanego obrazu
        zwalnianiePamieci(&nowy);
        return 0;
    }
    *baza = temp;
    (*baza)[*rozmiar] = nowy; // przeniesienie (plytka kopia z wlasnoscia pikseli)
    *wybrany = *rozmiar;
    (*rozmiar)++;
    return 1;
}

int usunObrazZBazy(struct obrazPGM** baza, int* rozmiar, int* wybrany, int idx)
{
    if (!baza || !*baza || idx < 0 || idx >= *rozmiar) return 0;
    int ostatni = *rozmiar - 1;

    // 1) zwolnij pamiec usuwanego elementu
    zwalnianiePamieci(&(*baza)[idx]);

    // 2) przepisz ostatni w miejsce usuwanego (jesli inny)
    if (idx != ostatni) (*baza)[idx] = (*baza)[ostatni];

    // 3) zmniejsz rozmiar
    (*rozmiar)--;

    // 4) korekta wybranego
    if (*wybrany == idx) *wybrany = -1;
    else if (*wybrany == ostatni) *wybrany = idx;

    if (*rozmiar == 0)
    {
        free(*baza); *baza = NULL; *wybrany = -1; return 1;
    }

    // 5) bezpieczna realokacja do mniejszego rozmiaru (opcjonalnie)
    struct obrazPGM* temp = (struct obrazPGM*)realloc(*baza, (*rozmiar) * sizeof(struct obrazPGM));
    if (temp) *baza = temp; // jesli nie, zostaw wiekszy bufor do kolejnych dodan
    return 1;
}

int wybierzObrazZBazy(int rozmiar, int* wybrany, int idx)
{
    if (idx < 0 || idx >= rozmiar) return 0;
    *wybrany = idx; return 1;
}

void freeBaza(struct obrazPGM** baza, int* rozmiar)
{
    if (!baza || !*baza) return;
    for (int i = 0; i < *rozmiar; ++i) zwalnianiePamieci(&(*baza)[i]);
    free(*baza); *baza = NULL; *rozmiar = 0;
}

int main()
{
    srand(time(NULL));

    struct obrazPGM* baza = NULL;
    int rozmiar = 0;
    int wybrany = -1;

    int menu = -1;
    while (menu != 0)
    {
        printf("\n----------- menu -----------\n");
        printf("1. Dodaj obraz (odczyt)\n");
        printf("2. Lista obrazow\n");
        printf("3. Wybierz obraz\n");
        printf("4. Odbicie wybranego\n");
        printf("5. Wprowadzenie szumu do wybranego\n");
        printf("6. Zapis wybranego\n");
        printf("7. Usun obraz\n");
        printf("0. Zakonczenie\n");
        printf("Wybierz opcje: ");
        menu = lepszyScanfDoInta();

        switch (menu)
        {
        case 0:
            printf("\nZakonczenie programu.\n");
            _CrtDumpMemoryLeaks();
            break;
        case 1:
        {
            if (dodajObrazDoBazy(&baza, &rozmiar, &wybrany))
                printf("Dodano obraz i ustawiono jako wybrany (indeks %d).\n", wybrany);
            else
                printf("Nie udalo sie dodac obrazu.\n");
            break;
        }
        case 2:
            listaObrazow(baza, rozmiar, wybrany);
            break;
        case 3:
        {
            if (rozmiar == 0) { printf("Baza pusta.\n"); break; }
            printf("Podaj indeks [0..%d]: ", rozmiar - 1);
            int idx = lepszyScanfDoInta();
            if (wybierzObrazZBazy(rozmiar, &wybrany, idx)) printf("Wybrano obraz %d.\n", wybrany);
            else printf("Bledny indeks.\n");
            break;
        }
        case 4:
        {
            if (wybrany < 0) { printf("Najpierw wybierz obraz.\n"); break; }
            odbicie(&baza[wybrany]);
            break;
        }
        case 5:
        {
            if (wybrany < 0) { printf("Najpierw wybierz obraz.\n"); break; }
            szum(&baza[wybrany]);
            break;
        }
        case 6:
        {
            if (wybrany < 0) { printf("Najpierw wybierz obraz.\n"); break; }
            char out[100];
            printf("Podaj nazwe pliku wyjsciowego (np. nowy.pgm): ");
            scanf("%99s", out);
            zapis(out, &baza[wybrany]);
            break;
        }
        case 7:
        {
            if (rozmiar == 0) { printf("Baza pusta.\n"); break; }
            printf("Podaj indeks do usuniecia [0..%d]: ", rozmiar - 1);
            int idx = lepszyScanfDoInta();
            if (usunObrazZBazy(&baza, &rozmiar, &wybrany, idx)) printf("Usunieto obraz.\n");
            else printf("Bledny indeks.\n");
            break;
        }
        default:
            printf("\nNieprawidlowa opcja!\n");
        }
    }

    freeBaza(&baza, &rozmiar);
    return 0;
}
