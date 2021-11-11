//MIERLICH DANIEL 313 CB
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NMAX 39                            //dimensiunea vectorului de copii
#define CHAR_TO_IDX(c) ((int)c - (int)'*') //transform caracter in numar
#define isLeafNode(r) ((r->sfarsit) != 0)  //si cu ajutorul altror functii
                                           //aflu pozitia in vectorul children
#pragma pack(1)

typedef struct Tnod {
  struct Tnod *children[NMAX];  // copii
  struct Tnod *parinte;         // parintele directorului
  unsigned short sfarsit;       // 0 sau 1
} Tnod, *Pnod, **Anod;

#pragma pack()

int get_poz(int numar);

int get_char(int poz);

int nr_copii(Pnod nod);

Pnod getnode();

void mkdir(Pnod dir_curent, char *director);

void touch(Pnod dir_curent, char *director);

Pnod cd(Pnod root, Pnod dir_curent, char *cale);

Pnod get_root(Pnod nod);

void pwd(Pnod arb);

void ls(Pnod root, char *str, int level);

void ls_f(Pnod root, char *str, int level);

int search_fisier(Pnod root, char *key);

Pnod search_dir(Pnod root, char *key, int *exista);

int verifica_exist(Pnod dir, char *director);

int verifica_fisier(Pnod dir, char *director);

void gasesc_cale(Pnod arb, char *info, int indice);

Pnod gaseste_nod(Pnod director, char *cale);

void desparte(char *string);

int calculeaza(char *string);

void mv(Pnod dir_curent, char *fisier, char *cale);

void free_all(Pnod nod);

void cp(Pnod dir_curent, char *fisier, char *cale);

Pnod nod_necesar(Pnod dir, char *cale);

void rm(Pnod dir, char *cale);

void func_ajutor(Pnod dir_curent, char *str, int level, char *director);

void rmdir(Pnod dir_curent, char *director);