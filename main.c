#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DELKARC 10

enum { zena, muz }; // misto #define, hodnoty {0, 1}

// naplneni rodneho cisla
int naplneniRC(const char *text, int min, int max) {
    int cislo;
    int countOfChar;
    do {
        countOfChar = 0;
        printf("Zadej %s v intervalu %d a %d.\n", text, min, max);
        if (scanf("%d", &cislo) != 1) {
            printf("Nezadal jsi cele cislo, zadej znovu.\n");
            countOfChar = 1;
        }
        while (getchar() != '\n') {
            // vycistime buffer
            countOfChar = 1;
        }
        if (countOfChar == 0 && (cislo < min || cislo > max)) {
            printf("Nezadal jsi cislo v intervalu, zadej znovu.\n");
            countOfChar = 1;
        }
    } while (countOfChar != 0);
    return cislo;
}

// maximalni den v mesici
int zjisteniMaxDen(int mesic, int rok) {
    if (mesic == 2) {
        // unor
        return ((rok % 4 == 0 && rok % 100 != 0) || (rok % 400 == 0)) ? 29 : 28; // prestupny rok
    } else if (mesic == 4 || mesic == 6 || mesic == 9 || mesic == 11) {
        // 30 dni
        return 30;
    } else {
        // 31 dni
        return 31;
    }
}

// posledni dve cislice roku
int ziskejDveCislice(int rok) {
    return rok % 100;
}

// validace rodneho cisla podle delitelnosti 11
void validni(char *rodneCislo) {
    char prvni9Znaky[DELKARC];
    int zbytek;
    do {
        strncpy(prvni9Znaky, rodneCislo, 9); // ziskame prvnich 9 znaku z rodneho cisla do prvni9Znaky
        prvni9Znaky[9] = '\0'; // kvuli referenci nakonec pridame radeji binarni nulu.
        zbytek = atoi(prvni9Znaky) % 11;
        if (zbytek == 10) {
            int posledniTri = atoi(&rodneCislo[6]);
            posledniTri = (posledniTri + 1) % 1000; // pokud kontrolka presahne 999, vrati se na nulu
            sprintf(&rodneCislo[6], "%03d", posledniTri);
        }
    } while (zbytek == 10); // generovani (dopocitavani) do rodnehoCisla, aby bylo validni
    rodneCislo[9] = (zbytek + '0');
    // '0' je znakova nula (prevede zbytek na char), hodnota 48
}

int main() {
    char rodneCislo[DELKARC + 1] = ""; // +1 kvuli \0
    char str[6]; // pro formatovani cisel
    char enter; // pro kontrolu stisku Enteru
    int enterFlag = 1; // vlajka pro opakovani (1 = opakovat, 0 = ukoncit)

    do {
        printf("Generovani fiktivniho rodneho cisla\n-------------------------------------\n");

        int rok = naplneniRC("rok", 1954, 2053);
        int mesic = naplneniRC("mesic", 1, 12);
        int maxDen = zjisteniMaxDen(mesic, rok); // max den
        int den = naplneniRC("den", 1, maxDen);
        int pohlavi = naplneniRC("pohlavi (0 - zena, 1 - muz)", 0, 1);
        int kontrolka = naplneniRC("kontrolni cislici za lomitkem", 0, 999);
        int countOfChar = 0;

        rok = ziskejDveCislice(rok);
        sprintf(str, "%02d", rok); // sprintf je neco jako obraceny atoi
        strcat(rodneCislo, str);

        // pokud je zena, tak mesic je o 50 vetsi
        if (pohlavi == zena) {
            mesic += 50;
        }
        sprintf(str, "%02d", mesic);
        strcat(rodneCislo, str);

        sprintf(str, "%02d", den);
        strcat(rodneCislo, str);

        sprintf(str, "%03d", kontrolka);
        strcat(rodneCislo, str);

        // validace rodneho cisla
        validni(rodneCislo);

        printf("Bylo vytvoreno validni rodne cislo: %s\n", rodneCislo);

        // Před opakováním vyčistíme buffer a čekáme na Enter
        printf("Pro opakovane zadani rodneho cisla stiskni Enter.\n");
        scanf("%c", &enter); // Nacte skutecny stisk Enteru
        while (getchar() != '\n') {
            countOfChar++;
        }
        if (countOfChar != 0) {
            enterFlag = 0;
        } else if (enter != '\n') {
            enterFlag = 0;
        }
        // Pokud uzivatel stiskl Enter, opakujeme, jinak ukoncime
        // resetovani rodneho cisla pro opakovani
        memset(rodneCislo, 0, sizeof(rodneCislo)); /* neco jako free pro staticke pole, nula se zde
        pouziva jako NULL */
    } while (enterFlag == 1);
    return 0;
}
