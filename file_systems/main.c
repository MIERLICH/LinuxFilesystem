//MIERLICH DANIEL 313 CB
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "define_vect.h"

int main() {  // punct de ref
  char str[64], str1[24], str2[24], err[8]; //stringuri
  char *pch;
  Pnod root = getnode(); //aloc nodul root
  if (!root) {
    return -1;
  }
  Pnod dir_curent = root; //setez dir_curent = root, la pornirea programului
  fgets(str, 63, stdin); //cu acest fgets ignor prima linie din fisiere
  while (fgets(str, 63, stdin)) {//
    pch = strtok(str, " \n");  //cu strtok separ 
    if (pch == NULL) return 0; //daca a fost enter
    if (strcmp(pch, "mkdir") == 0) { //daca a fost mkdir 
      pch = strtok(NULL, " \n"); //ignor spatiile trec la urmatorul
      if (!pch) fprintf(stderr, "%s: missing operand\n", pch);
      while (pch) {             //cat pot sa separ cu strtok apelez mkdir 
        mkdir(dir_curent, pch); //cu pch, pch este directorul care trebuie creat
        pch = strtok(NULL, " \n");
      }
    } else if (strcmp(pch, "touch") == 0) {
      pch = strtok(NULL, " \n");
      if (!pch) fprintf(stderr, "%s: missing opearnd\n", pch); //eroare
      while (pch) { //separ cu strtok
        touch(dir_curent, pch); //creez fisiere
        pch = strtok(NULL, " \n");
      }
    } else if (strcmp(pch, "pwd") == 0) {
      fprintf(stdout, "/");         //printez / si apelez pwd
      if (dir_curent->parinte) {    //care afiseaza unde ma aflu
        pwd(dir_curent->parinte);
      }
      fprintf(stdout, "\n");
    } else if (strcmp(pch, "cd") == 0) {
      pch = strtok(NULL, " \n");//apelez cd, schimb directorul
      dir_curent = cd(root, dir_curent, pch);
    } else if (strcmp(pch, "ls") == 0) {
      int level = 0;
      char str[32];
      pch = strtok(NULL, " \n"); //trec la urmatorul argument
      if (!pch) {
        ls(dir_curent, str, level); //apelez ls
        if (nr_copii(dir_curent) == 0) { //daca directorul
          fprintf(stdout, "\n");         //curent nu are copii
        }
      } else {
        if (strcmp(pch, "-F") != 0) {//inseamna ca e ls pe director anumit
          func_ajutor(dir_curent, str, level, pch); //functie ajutor
        } else {  // daca a ajuns aici => a fost dat ls -F ....
          pch = strtok(NULL, " \n");
          if (!pch) {  // inseamna ca e pur si simplu -F
            ls_f(dir_curent, str, level);
            if (nr_copii(dir_curent) == 0) {
              fprintf(stdout, "\n");
            }
          } else {  // daca a fost dat -F pe director
            strcpy(err, pch);  //apelez ls-f pe director cu pch
            if (strcmp(pch, ".") == 0) ls_f(dir_curent, str, level);
            pch = strtok(NULL, " \n");
            if (pch) { //in caz de prea multi argumenti
              fprintf(stderr, "ls -F %s %s: too many arguments\n", err, pch);
              continue;
            }
          }
        }
      }
      if (nr_copii(dir_curent) != 0) printf("\n");
    } else if (strcmp(pch, "mv") == 0) {
      pch = strtok(NULL, " \n");
      strcpy(str1, pch);
      pch = strtok(NULL, " \n");
      if (pch == NULL) { //daca nu are nr argumente necesar
        fprintf(stderr, "mv %s: missing operand\n", str1);
        continue;
      }
      strcpy(str2, pch);
      mv(dir_curent, str1, str2); //apelez mz
    } else if (strcmp(pch, "cp") == 0) {
      pch = strtok(NULL, " \n");
      strcpy(str1, pch);
      pch = strtok(NULL, " \n");
      strcpy(str2, pch);
      cp(dir_curent, str1, str2); //functia cp
    } else if (strcmp(pch, "rm") == 0) {
      pch = strtok(NULL, " \n"); //separ cu strtok
      while (pch) { //cat pch, pt oricate fisiere/directoare
        rm(dir_curent, pch); //rm
        pch = strtok(NULL, " \n");
      }
    } else if (strcmp(pch, "rmdir") == 0) {
      pch = strtok(NULL, " \n");
      while (pch) { //cat pch pt oricate fisiere/directoare
        rmdir(dir_curent, pch);
        pch = strtok(NULL, " \n");
      }
    } else {
      fprintf(stderr, "%s: command not found\n", pch);
    } //daca nu era nici o comanda cuoscuta => eroare
  }
  free_all(root); //fac dezalocare la memorie
  return 0;
}