#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <locale.h>
#include <cctype>
#include <cstring>
#define ampmax 2.0

struct punkt
{
	double x; // wspolrzedna x
	double y; // wspolrzedna y
	double s; // wspolrzedna y zaszumiona
	double w; // wspolrzedna robocza
};
struct tablica  // tablica wynikowa wraz z rozmiarem
{
	punkt *tabRES;
	int tabRESsize;
};
double czytanie(char* lancuch)
{
	double v_ret;
	char smiec[100];
	int v_cnt;
	printf("%s\n", lancuch);
	do
	{
		v_cnt = scanf_s("%lf", &v_ret);
		if (v_cnt != 1)
		{
			printf("Podany lancuch nie jest liczba typu double\n%s\n", lancuch);
			while (getchar() != '\n');
		}
	} while (v_cnt != 1);
	return v_ret;
}
void wspolczynniki(double* tabWSP)
{
	printf("Podaj wspolczynniki funkcji matematycznej\n");
	tabWSP[0]=czytanie("Podaj wspolczynnik 'a' funkcji");
	tabWSP[1]=czytanie("Podaj wspolczynnik 'b' funkcji");	
	tabWSP[2]=czytanie("Podaj wspolczynnik 'c' funkcji");	
	do
	{
		tabWSP[3]=czytanie("Podaj wspolczynnik 'd' funkcji, ktory powinien byc rozny od 0");
	} while (tabWSP[3] == 0);
}
void dziedzina(double* tabIKS)
{
	tabIKS[0]=czytanie("Podaj dolny zakres dziedziny");
	tabIKS[1]=czytanie("Podaj gorny zakres dziedziny");
}
tablica filtr(tablica tab)
{
	int i;
	double a1, a2, a3, tmp;
	if (tab.tabRESsize <= 0)
	{
		puts("uruchom generowanie lub odczytaj z pliku dane");
		system("pause");
		return tab;
	}
	for (i = 0; i < tab.tabRESsize; i++)
	{
		if (i == 0) a1 = tab.tabRES[i].s;
		else a1 = tab.tabRES[i - 1].s;
		a2 = tab.tabRES[i].s;
		if (i == tab.tabRESsize - 1) a3 = tab.tabRES[i].s;
		else a3 = tab.tabRES[i + 1].s;
		if (a1 > a2)
		{
			tmp = a1;
			a1 = a2;
			a2 = tmp;
		}
		if (a3 < a1)
		{
			tmp = a3;
			a3 = a2;
			a2 = a1;
			a1 = tmp;
		}
		if (a3 < a2)
		{
			tmp = a3;
			a3 = a2;
			a2 = tmp;
		}
		tab.tabRES[i].w = a2;
	}
	for (i = 0; i < tab.tabRESsize; i++)
	{
		if (i == 0) a1 = tab.tabRES[i].w;
		else a1 = tab.tabRES[i - 1].w;
		a2 = tab.tabRES[i].w;
		if (i == tab.tabRESsize - 1) a3 = tab.tabRES[i].w;
		else a3 = tab.tabRES[i + 1].w;
		tab.tabRES[i].y = (a1+a2+a3)/3;
	}
	puts("Filtrowanie zostalo zakonczone.");
	system("pause");
	return tab;
}
int rozmiarwynikow()
{
	int ret;
	int p;
	do
	{
		ret=czytanie("Podaj rozmiar tablicy wynikow");
	} while (ret <= 0);
	return ret;
}
tablica generuj(tablica tab)
{
	double tabWSP[4];
	double tabIKS[2];
	int i = 0;
	double stepx;
	double x;
	fflush(stdin);
	tab.tabRESsize = rozmiarwynikow();
	tab.tabRES = (punkt*)realloc(tab.tabRES, tab.tabRESsize * sizeof(punkt));
	wspolczynniki(tabWSP);
	dziedzina(tabIKS);
	x = tabIKS[0];
	stepx = (tabIKS[1] - tabIKS[0]) / (tab.tabRESsize - 1);
	for (i = 0; i < tab.tabRESsize; i++)
	{
		if (x == 0)
		{
			double xx;
			xx = x + 0.00001;
			tab.tabRES[i].x = xx;
			tab.tabRES[i].y = (tabWSP[0] * xx*xx*sin((tabWSP[1] * xx) + tabWSP[2])) / (tabWSP[3] * xx);
			tab.tabRES[i].s = tab.tabRES[i].y;
		}
		else
		{
			tab.tabRES[i].x = x;
			tab.tabRES[i].y = (tabWSP[0] * x*x*sin((tabWSP[1] * x) + tabWSP[2])) / (tabWSP[3] * x);
			tab.tabRES[i].s = tab.tabRES[i].y;
		}
		x = x + stepx;
	}
	puts("generowanie przebieglo pomyslnie");
	system("pause");
	return tab;
}

double randomdouble(double min, double max)
{
	double tmp;
	if (max >= min)
		max -= min;
	else
	{
		tmp = min - max;
		min = max;
		max = tmp;
	}
	return max ? (rand() / (RAND_MAX + 1.0) * max + min) : min;
}
int randomint(int min, int max)
{
	int tmp;
	if (max >= min)
		max -= min;
	else
	{
		tmp = min - max;
		min = max;
		max = tmp;
	}
	return max ? (rand() % max + min) : min;
}
tablica szumowanie(tablica tab)
{
	int iloscprobek, i, nrElTabRES;
	double amplituda;
	if (tab.tabRESsize <= 0)
	{
		puts("uruchom generowanie lub odczytaj z pliku dane");
		system("pause");
		return tab;
	}
	iloscprobek = randomint(0, tab.tabRESsize - 1);
	amplituda = randomdouble(0, ampmax);
	for (i = 0; i < iloscprobek; i++)
	{
		do
		{
			nrElTabRES = randomint(0, tab.tabRESsize - 1);
		} while (tab.tabRES[nrElTabRES].y != tab.tabRES[nrElTabRES].s);
		tab.tabRES[nrElTabRES].s = tab.tabRES[nrElTabRES].y + randomdouble(-amplituda, amplituda);
	}
	puts("Zaszumianie zostalo zakonczone.");
	system("pause");
	return tab;
}
void zapis(tablica tab)
{
	FILE *plik;
	char nazwa[256];
	char wybor[100];
	char znak;
	int i, error;
	if (tab.tabRESsize <= 0)
	{
		puts("uruchom generowanie lub odczytaj z pliku dane");
		system("pause");
		return;
	}
	do
	{
		printf("Podaj nazwe pliku (.csv)\n");
		scanf("%s", &nazwa);
		plik = fopen(nazwa, "w");
		if (plik == 0) puts("Podales niepoprawna nazwe pliku");
	} while (plik == 0);
	do
	{
		error = 0; //semafor wystapienia bledu
		puts("Czy wyniki (N)ormalne czy (Z)aszumione zapisac w pliku. Podaj 'Z' lub 'N'\n");
		scanf("%s", &wybor);
		if (strlen(wybor) != 1) error = 1;
		if (error == 0) znak = toupper(wybor[0]);
		if (error == 0 && znak != 'Z' && znak != 'N') error = 1;
		if (error == 1) puts("Podaj 'Z' lub 'N'");
	} while (error == 1);
	for (i = 0; i < tab.tabRESsize; i++)
	{
		fprintf(plik, "%lf;%lf\n", tab.tabRES[i].x, znak == 'Z' ? tab.tabRES[i].s : tab.tabRES[i].y);
	}
	fclose(plik);
}
tablica odczyt(tablica tab)
{
	FILE *plik;
	char nazwa[256];
	int rezultat;
	int cnt;
	double x, y;
	do
	{
		printf("Podaj nazwe pliku (.csv). Wcisniej '0' jesli chcesz sie wycofac.\n");
		cnt=scanf("%s", &nazwa);
		if (strcmp(nazwa, "0") == 0) return tab;
		plik = fopen(nazwa, "r");
		if (plik == 0) puts("Podales niepoprawna nazwe pliku");
	} while (plik == 0);
	free(tab.tabRES);
	tab.tabRESsize = 0; // wyzerowanie struktury tab, part1
	tab.tabRES = NULL; // wyzerowanie struktury tab, part2. w celu unikniecia smieci z lokowania pamieci reallockiem
	do
	{
		rezultat = fscanf(plik, "%lf;%lf\n", &x, &y);
		if (rezultat != EOF && rezultat == 2)
		{
			tab.tabRESsize = tab.tabRESsize + 1;
			tab.tabRES = (punkt*)realloc(tab.tabRES, tab.tabRESsize * sizeof(punkt));
			tab.tabRES[tab.tabRESsize - 1].x = x;
			tab.tabRES[tab.tabRESsize - 1].y = y;
			tab.tabRES[tab.tabRESsize - 1].s = y;
		}
		else if (rezultat != 2 && rezultat != EOF) printf("Blad wewnatrz pliku w wierszu %d: ", tab.tabRESsize);
	} while (rezultat != EOF);
	fclose(plik);
	return tab;
}
int main()
{
	int opcja;
	int t = 0;
	tablica tbl;
	setlocale(LC_ALL, "");
	tbl.tabRES = (punkt*)malloc(0);
	tbl.tabRESsize = 0;
	do
	{
		system("cls");
		puts("Witaj, wybierz:\n'1' Aby wygenerowac sygnal.\n'2' Aby wczytac plik.\n'3' Aby wprowadzic szum.\n'4' Aby odfiltorwac sygnal.\n'5' Aby zapisac aktualny stan sygnalu.\n'0' Wyjscie");
		scanf("%d", &opcja);
		switch (opcja)
		{
		case 1 : tbl = generuj(tbl); break;
		case 2 : tbl = odczyt(tbl); break;
		case 3 : tbl = szumowanie(tbl); break;
		case 4 : tbl = filtr(tbl); break;
		case 5 : zapis(tbl); break;
		case 0:; break;
		default : czytanie("Podana niepoprawna wartosc"); system("pause"); break;
		}
	} while (opcja != 0);
	free(tbl.tabRES);
	return 0;
}