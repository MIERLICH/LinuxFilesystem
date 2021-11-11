//MIERLICH DANIEL 313 CB
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "define_vect.h"

int get_poz(int numar) {
  if (numar == 0) return 0; //indicile in vectorul de copii
  if (numar >= 4 && numar <= 15) return numar - 3;
  return numar - 42;
}

int get_char(int poz) {
  if (poz == 0) return 42; //transforma int in caracter
  if (poz >= 1 && poz <= 12) return poz + 45; //numar in (ASCII)
  return poz + 84;
}

Pnod getnode() {
  int i;            //alocare nod
  Pnod nod = NULL;  //setez initial pe NULL
  nod = (Pnod)malloc(sizeof(Tnod));
  if (nod) {
    nod->parinte = NULL; //NULL
    nod->sfarsit = 0;
    for (i = 0; i < NMAX; i++) {
      nod->children[i] = NULL; //initial nu este nici un copil
    }
  }
  return nod;
}

void pwd(Pnod arb) {
  int i = 0;
  if (arb) {
    Pnod aux = arb->parinte;
    for (i = 0; i < NMAX; i++) {
      if (aux) {
        if (aux->children[i] == arb) { 
          pwd(arb->parinte); //dupa pozitia in vectorul alocat
          printf("%c", get_char(i)); //stiu ce litera trebuie sa afisez
        }
      }
    }
  }
}
//
void mkdir(Pnod dir_curent, char *director) {
  int poz;
  char drum2[24];
  char copie[32];
  char *point;
  unsigned int i;
  Pnod aux;
  if (director[0] == '/') {
    aux = get_root(dir_curent); //obtin root
    strcpy(copie, director);          //
    point = copie + 1;    // scap de / din stringul "director"
    if (calculeaza(director) <= 1) {  /// aici ceva
      if (verifica_exist(aux, point) != 0) { //eroare daca exista
        fprintf(stderr, "%s: already exists\n", director);
        return;
      }
      strcpy(director, point);
    } else {
      if (calculeaza(director) < NMAX) {
        if (verifica_exist(aux, point) != 0) {//eroare exista
          fprintf(stderr, "%s: already exists\n", director);
          return;
        }
      }
      strcpy(drum2, point);  // pastrez stringul initial
      desparte(point);
      if (verifica_exist(aux, point) == 0) {
        fprintf(stderr, "%s: No such file or directory\n", director);
        return;
      }
      strcpy(director, drum2);
    }
  } else {
    strcpy(drum2, director);
    desparte(director); //iau din string tot pana la ultimul /
    if (calculeaza(drum2) == 0) {
      if (verifica_exist(dir_curent, drum2) != 0) {
        fprintf(stderr, "%s: already exists\n", director);
        return; //eroare, directorul exista
      }
      aux = dir_curent;
    } else {
      if (verifica_exist(dir_curent, director) == 0) {
        fprintf(stderr, "%s: No such file or directory\n", drum2);
        return; //eroare, calea nu este
      }
      aux = dir_curent;
      strcpy(director, drum2);
    }
  }
  for (i = 0; i < strlen(director); i++) {
    poz = get_poz(CHAR_TO_IDX(director[i]));
    if (!aux->children[poz]) { //aloc numai daca nu exista
      aux->children[poz] = getnode();
      if(!aux->children[poz]) return; //daca nu sa alocat
      aux->children[poz]->parinte = aux;
    }
    aux = aux->children[poz];
  }
  poz = get_poz(CHAR_TO_IDX('/')); //la director adaug /
  if (!aux->children[poz]) {        
    aux->sfarsit = 1; //setez sfarsit sa stiu ca sa terminat directorul
    aux->children[poz] = getnode();
    if(!aux->children[poz]) return;
    aux->children[poz]->parinte = aux;
  }
}

void touch(Pnod dir_curent, char *director) {
  Pnod aux = dir_curent;
  unsigned int i;
  int poz;
  char copie[24];
  // touch nu incepe cu /
  if (director[0] != '/') {  // daca fisier
    strcpy(copie, director);
    if (calculeaza(copie) < 1) {  // daca este in directorul curent
      if (verifica_fisier(dir_curent, copie) != 0) {  // inseamna ca este
        fprintf(stderr, "%s: already exists\n", copie);
        return;
      }

      strcpy(director, copie);
    } else {
      desparte(director);
      if (verifica_exist(dir_curent, director) == 0) {  // nui calea
        fprintf(stderr, "%s: No such file or directory\n", copie);
        return;
      }  // daca nui asta clar ca si fisierul nui
      strcpy(director, copie);
    }
  }
  for (i = 0; i < strlen(director); i++) { //pentru fiecare caracter
    poz = get_poz(CHAR_TO_IDX(director[i])); //din string
    if (!aux->children[poz]) {
      aux->children[poz] = getnode();
      if(!aux->children[poz]) return;
      aux->children[poz]->parinte = aux;
    }
    aux = aux->children[poz];
  }
  poz = get_poz(CHAR_TO_IDX('*'));
  if (!aux->children[poz]) {
    aux->sfarsit = 1;
    aux->children[poz] = getnode();
    if(!aux->children[poz]) return; //nu sa alocat
    aux->children[poz]->parinte = aux;
  }
}

int search_fisier(Pnod root, char *key) {
  int poz;
  unsigned int i;
  Pnod aux = root;
  for (i = 0; i < strlen(key); i++) {
    poz = get_poz(CHAR_TO_IDX(key[i]));
    if (!aux->children[poz]) {
      return 0;
    }
    aux = aux->children[poz];
  } //cauta fisier, daca nu este intoare 0
  poz = get_poz(CHAR_TO_IDX('*'));
  if (!aux->children[poz]) {
    return 0;
  } //intoare 1 daca exista 
  return (aux != NULL && aux->sfarsit);
}

Pnod search_dir(Pnod root, char *key, int *exista) {
  int poz;                          //Cea mai oribila functie
  unsigned int i;                   //era gandita sa intoarca
  Pnod aux = root;                  //daca exista calea 
  for (i = 0; i < strlen(key); i++) {   //dar daca exista calea 
    poz = get_poz(CHAR_TO_IDX(key[i])); //evident ca exista si director
    if (!aux->children[poz]) {
      *exista = 0;
      return NULL;
    }
    aux = aux->children[poz];
  }
  poz = get_poz(CHAR_TO_IDX('/'));
  if (!aux->children[poz]) {
    if (aux->children[0]) {
      *exista = 2;
    }
    return NULL;
  }
  *exista = (aux != NULL && aux->sfarsit);
  aux = aux->children[poz];
  return aux;
}

Pnod cd(Pnod root, Pnod dir_curent, char *cale) {
  Pnod copie = dir_curent;
  Pnod aux = NULL;
  unsigned int i = 0;
  int j, exista = 0, indice = 0, numar = 0, val = 0;
  char vect1[32], vect2[32], drum[24];
  char *point;
  if (strcmp(cale, "../..") == 0) {
    fprintf(stderr, "%s: No such file or directory\n", cale);
    return dir_curent; //caz aparte
  } else if (strcmp(cale, "..") == 0) {
    if (dir_curent == root) {
      fprintf(stderr, "%s: No such file or directory\n", cale);
      return dir_curent; //daca sunt deja in root
    }
    gasesc_cale(dir_curent, vect1, indice);
    j = strlen(vect1) - 1;
    for (; j >= 0; j--) {
      vect2[numar] = vect1[j];
      if (vect2[numar] == '/') val += 1;
      numar += 1;
    }
    vect2[numar] = '\0';
    if (val == 1) return get_root(dir_curent);
    for (i = 0; i < strlen(vect2); i++) {
      drum[i] = vect2[i];
      if (drum[i] == '/') exista++;
      if (val - 1 == exista) {
        break;
      }
    }
    drum[i + 1] = '\0';
    return gaseste_nod(dir_curent, drum);
  } else if (cale[0] == '/') {
    Pnod dir = get_root(dir_curent);
    point = cale + 1;  // scap de '/' de la inceputul stringului
    aux = search_dir(dir, point, &exista);
    if (!aux) {
      fprintf(stderr, "%s: No such file or directory\n", cale);
      return copie;
    }
    return aux;
  } else {
    aux = search_dir(dir_curent, cale, &exista);
    if (verifica_exist(dir_curent, cale) == 0 &&
        verifica_fisier(dir_curent, cale) != 0) {
      fprintf(stderr, "%s: Not a directory\n", cale);
      return copie;
    }
    if (!aux) {
      fprintf(stderr, "%s: No such file or directory\n", cale);
      return copie;
    }
    return dir_curent = aux;
  }
}

void ls(Pnod root, char *str, int level) {
  if (isLeafNode(root)) { //recursiva
    str[level] = '\0';    //memorez in string, afisez
    if (root->children[2]) printf("%s ", str);
    if (root->children[0]) printf("%s ", str);
  }
  int i = 0;
  for (i = 1; i < NMAX; i++) { //incep de le 1 pt ca scat cazul cu *(fisier)
    if ((root->children[i]) && i != 2) {
      str[level] = get_char(i);
      ls(root->children[i], str, level + 1);
    }
  }
}

void ls_f(Pnod root, char *str, int level) {
  int i = 0;
  if (isLeafNode(root)) {    //acelasi lucru ca si ls doar
    str[level] = '\0';       //ca mai pun "/" si "*"
    if (root->children[2]) printf("%s/ ", str);
    if (root->children[0]) printf("%s* ", str);
  }
  for (i = 1; i < NMAX; i++) {
    if ((root->children[i]) && i != 2) {
      str[level] = get_char(i);
      ls_f(root->children[i], str, level + 1);
    }
  }
}

int nr_copii(Pnod nod) {
  int i = 0, j = 0;
  if (!nod) return 0;
  for (i = 0; i < NMAX; i++) {
    if (nod->children[i]) j++;
  } //numar cati copii are un nod
  return j;
}

Pnod get_root(Pnod nod) {
  while (nod->parinte) {
    nod = nod->parinte;
  } //ma intorc in root unde nu mas afla
  return nod;
}

int verifica_exist(Pnod dir, char *director) {
  unsigned int i;
  int poz;                                  //functia verifica daca
  for (i = 0; i < strlen(director); i++) {  //este deja asa director
    poz = get_poz(CHAR_TO_IDX(director[i]));//1 daca exista, 0 in caz contrar
    if (!dir->children[poz]) {
      return 0;
    }
    dir = dir->children[poz];
  }
  poz = get_poz(CHAR_TO_IDX('/'));
  if (!dir->children[poz]) {
    return 0;
  }
  return 1;
}

int verifica_fisier(Pnod dir, char *director) {
  unsigned int i;
  int poz;
  for (i = 0; i < strlen(director); i++) {
    poz = get_poz(CHAR_TO_IDX(director[i]));
    if (!dir->children[poz]) {
      return 0;                        //verifica daca fisierul exista
    }                                  //1 daca exista, 0 in caz contrar
    dir = dir->children[poz];
  }
  poz = get_poz(CHAR_TO_IDX('*'));
  if (!dir->children[poz]) {
    return 0;
  }
  return 1;
}

void gasesc_cale(Pnod arb, char *info, int indice) {
  int i = 0;
  if (arb) {
    Pnod aux = arb->parinte;
    for (i = 0; i < NMAX; i++) {
      if (aux) {
        if (aux->children[i] == arb) {
          gasesc_cale(arb->parinte, info, indice + 1);
          info[indice] = get_char(i) + 0;
        }
      } else {
        info[indice] = '\0';
      }
    }
  } else {
    info[indice] = '\0';
  }
}

Pnod gaseste_nod(Pnod director, char *cale) {
  Pnod aux = get_root(director);
  unsigned int i;
  int poz;
  for (i = 0; i < strlen(cale); i++) {
    poz = get_poz(CHAR_TO_IDX(cale[i]));
    if (!aux->children[poz]) {
      return NULL;
    }
    aux = aux->children[poz];
  } //returnez cu un nod mai inainte de cel ce am nevoie
  return aux;   //sa vad daca se termina fisierul sau directorul
}

void desparte(char *string) {
  unsigned int i = 0;
  int s = -1; //despart stringul 
  while (i < strlen(string)) { //iau doar partea din fata
    if (string[i] == '/') s = i;
    i++;
  }
  if (s != -1) string[s] = '\0';
}

int calculeaza(char *string) {      //calculeaza cate "/"
  unsigned int i = 0;               //am in string
  int rez = 0;
  for (i = 0; i < strlen(string); i++) {
    if (string[i] == '/') rez++;
  }
  return rez;
}

void mv(Pnod dir_curent, char *fisier, char *cale) {
  Pnod root = get_root(dir_curent);
  Pnod unde;
  char copie[24];
  if (fisier[0] == '/') {  // caut fisierul din root //numaidecat fisier
    if (calculeaza(fisier) <= 1) {  // calculaza cate "/" apar in string
      if (verifica_fisier(root, fisier + 1) == 0) {  // verific existenta
        fprintf(stderr, "%s: No such file or directory\n", fisier);
        return;
      } else {
        fisier = fisier + 1;
        strcpy(copie, fisier);
      }
    } else {
      if (calculeaza(fisier) > 3) {
        if (verifica_fisier(root, fisier + 1) == 0) {  // verific existenta
          fprintf(stderr, "%s: No such file or directory\n", fisier);
          return;
        }
      }
    }
  } else {  // daca
    if (calculeaza(fisier) == 0) {
      if (verifica_fisier(dir_curent, fisier) == 0) {  // verific existenta
        fprintf(stderr, "%s: No such file or directory\n", fisier);
        return;
      }
    }
    if (verifica_fisier(dir_curent, fisier) == 0) {  // verific existenta
      fprintf(stderr, "%s: No such file or directory\n", fisier);
      return;
    }
  }
  if (cale[0] == '/') {
    // am deja root
    touch(root, copie); //fac fisierul in directorul care am nevoie
  } else {
    if (verifica_fisier(dir_curent, cale) == 1) {
      fprintf(stderr, "%s: Not a directory\n", cale);
      return; //nui director, eroare
    }
    if (verifica_exist(dir_curent, cale) == 0) {
      fprintf(stderr, "%s: No such file or directory\n", cale);
      return;
    } else {
      // calea
      unde = gaseste_nod(dir_curent, cale);
      unde = unde->children[2];  // ajung la "/"
      touch(unde, copie); //fac fisier in directorul "unde" care este cale data 
    }
  }
}

Pnod nod_necesar(Pnod dir, char *cale) {
  unsigned int i;
  int poz;
  for (i = 0; i < strlen(cale); i++) {
    poz = get_poz(CHAR_TO_IDX(cale[i]));
    if (!dir->children[poz]) {
      return NULL; //daca nu exista
    }
    dir = dir->children[poz];
  } //trec pe "/" el ocupa pozitia 2 in vector
  return dir->children[2];
}

void cp(Pnod dir_curent, char *fisier, char *cale) {
  Pnod root = get_root(dir_curent);
  Pnod aux;
  if (fisier[0] == '/') {  // caut fisierul din root //numaidecat fisier
    if (verifica_fisier(root, fisier + 1) == 0) {  // verific existenta
      fprintf(stderr, "%s: No such file or directory\n", fisier);
      return;
    }                     // inseamna ca este si calea si fisierul
    fisier = fisier + 7; //calculez
    aux = nod_necesar(dir_curent, cale);
  } else {
    if (verifica_fisier(dir_curent, fisier) == 0) {  // verific existenta
      fprintf(stderr, "%s: No such file or directory\n", fisier);
      return;
    }  // inseamna ca in directorul curent este fisierul
  }
  if (cale[0] != '/') {
    // exista_cale = verifica_exist(dir_curent, cale);
    if (verifica_exist(dir_curent, cale) == 0) {
      fprintf(stderr, "%s: No such file or directory\n", cale);
      return;
    }
    aux = nod_necesar(dir_curent, cale);
  }
  touch(aux, fisier);
}

void free_all(Pnod nod) {
  int i;
  for (i = 0; i < NMAX; i++) {
    if (nod->children[i]) {
      free_all(nod->children[i]);
    } //eliberare, recursiva
  }   //eliberez toata memoria alocata sau doar fisiere/directoare
  free(nod); //depinde de ce trimit ca parametru
}

void rm(Pnod dir_curent, char *cale) {
  Pnod aux;
  int i, dupa;
  if (cale[0] != '/') {
    if (verifica_fisier(dir_curent, cale) == 0) {
      fprintf(stderr, "%s: No such file or directory\n", cale);
      return;
    } else {
      if (cale[0] == 'f') {
        i = get_poz(CHAR_TO_IDX(
            cale[0]));  // pozitia //prima litera din string  //f1 f2
        dupa = get_poz(CHAR_TO_IDX(cale[1]));  // daca are mai multi copii
        free_all(dir_curent->children[i]->children[dupa]);
        dir_curent->children[i]->children[dupa] = NULL;
      } else if (cale[0] == 'a') {
        aux = gaseste_nod(dir_curent, cale);
        free_all(aux->children[0]);
        aux->children[0] = NULL;
      }
    }
  }
}

void func_ajutor(Pnod dir_curent, char *str, int level, char *director) {
  Pnod aux;
  if (strcmp(director, "..") == 0) {    //functia pentru ls 
    aux = get_root(dir_curent);         //pentru cazuri exeptie
    ls(aux, str, level);
  } else if (strcmp(director, "/") == 0) {
    aux = get_root(dir_curent);
    ls(aux, str, level);
  } else {
    aux = nod_necesar(dir_curent, director);
    ls(aux, str, level);
  }
}

void rmdir(Pnod dir_curent, char *director) {
  Pnod aux;
  int i;
  if (calculeaza(director) == 0) {
    if (verifica_fisier(dir_curent, director) != 0) {
      fprintf(stderr, "%s: Not a directory\n", director);
      return;
    }
    aux = gaseste_nod(dir_curent, director);
    if (!aux) {
      fprintf(stderr, "%s: No such file or directory\n", director);
      return;
    }
    aux = aux->children[2];
    for (i = 0; i < NMAX; i++) {
      if (aux->children[i]) {
        fprintf(stderr, "%s: Directory not empty\n", director);
        return;
      }
    }
  } else {
    char copie[24];
    strcpy(copie, director);
    desparte(director);
    Pnod nod = nod_necesar(dir_curent, director);
    for (i = 0; i < NMAX; i++) {
      if (nod->children[i]) {
        fprintf(stderr, "%s: Directory not empty\n", copie);
        return;
      }
    }
  }
}