/*
 =================================================
 Nume        	: Trivia
 Autor       	: Chelaru Ionuț
 Grupa			: 6105
 Descriere   	: Joc Trivia
 Documentatie	: PDF atasat
 =================================================
*/
#include <stdio.h>

#include <string.h>

#include <unistd.h>

#include <stdlib.h>

#include <time.h>

#include <GL/glut.h>

#include <windows.h>

//FUNCTIIfISIER

void Deschide_fisier(FILE **fisier,
  const char ** numefisier, int numar,
    const char * mod);

void Inchide_fisier(FILE **fisier, int numar);

//FUNCTII GRAFICA

void afisare();

void afisare_text(float x,float y, void *font,
  const char * string);

//VARIABILE GLOBALE

struct Jucatori {
  int ultimul_scor;
  int puncte_level;
};

int subiect_corect_raspuns[10] = {0};

int main(int argc, char ** argv) {

  //VARIABILE MENIU
  int optiune_meniu, optiune_joc, optiune_profil, optiune_despre, scan, c;
  //VARIABILE CONT
  char username[20] = "\0", password[20] = "\0", cont_temp[20] = "\0";
  int optiune_logare, cont_sters = 1, logare_reusita = 0;
  //VARIABILE JOC
  int dificultate = 1, optiune_excludere, exclude[10] = {0},
    scor = 0, penalizare_scor = 0, nivel_curent, raspuns_gresit = 0;
  const int level_ups[10] = {
    10, //Puncte pentru nivelul 1
    30,
    60,
    90,
    120,
    150,
    180,
    210,
    220,
    250
  };

  //JUCATOR
  struct Jucatori Jucator;

  /*----------C-O-N-T--S-T-A-R-T---------*/
  do {
    //Initializam "Fereastra" pentru creearea\logarea contului
    printf("1. LOGARE\n2. INREGISTRARE \n3. OASPETE\n\nAlege: ");
   fflush(stdout);

    //Acceptam doar numere de la 1-3
    while ((scan = scanf("%d", & optiune_logare)) == 0) {
      scanf("%*1[^1-3\n]");
     fflush(stdout);
      while ((c = getchar()) != '\n' && c != EOF) {}
      printf("\n1. LOGARE\n2. INREGISTRARE \n3. OASPETE\n\nAlege: ");
     fflush(stdout);
    }

    if (scan == EOF) {
      printf("nu afost citit nimic");
    }

    //Optiunea de Inregistrare
    if (optiune_logare == 2) {
      //Verificam daca nu cumva exista deja un cont inregistrat
      if (strlen(username) != 0 && strlen(password) != 0) {
        //Atragem atentia ca nu putem suprascrie datele
        printf("Doriti sa creati alt cont?\n[ATENTIE!: Contul curent vafi sters permanent]\nDA (1)\tNU (0)\n\nAlege: ");
       fflush(stdout);
        scanf("%d", & cont_sters);
        //Golim sirurile daca asa se alege
        if (cont_sters == 1) {
          username[0] = '\0';
          password[0] = '\0';
          printf("\n[Contul precedent afost sters definitiv!]\n");

        }
      }
      //Dupa golire citim alte date
      if (strlen(username) == 0 && cont_sters == 1) {

        printf("\nUSERNAME: ");
       fflush(stdout);
        scanf("%s", cont_temp);
        strcpy(username, cont_temp);

        printf("PAROLA: ");
       fflush(stdout);
        scanf("%s", cont_temp);
        strcpy(password, cont_temp);
       fflush(stdout);

        printf("\n[Intregistrat cu Succes!]\n\n");
        optiune_logare = 0;
      }

      //Ne intoarcem in meniul cont daca refuzam stergerea contului
      else if (cont_sters == 0) {
        printf("\n");
        continue;
      }
    }

    //Optiunea Logare
    else if (optiune_logare == 1) {

      //Verificam daca suntem inregistrati
      if (strlen(username) != 0 && strlen(password) != 0) {
        printf("\nUSERNAME: ");
       fflush(stdout);
        scanf("%s", cont_temp);

        //Comparam imediat Username-ul cu cel Inregistrat
        if (strcmp(username, cont_temp) == 0) {
          printf("PAROLA: ");
         fflush(stdout);
          scanf("%s", cont_temp);

          //Daca Username-ul coincide comparam parola
          if (strcmp(password, cont_temp) == 0) {
            printf("\n[Autentificare reusita!]\n\n");
            Jucator.ultimul_scor = 0;
            Jucator.puncte_level = 0;

            cont_sters = 0;
            logare_reusita = 1;
          } else printf("\n[Autentificare eșuata!]\n\n");
        } else printf("\n[Contul nu exista!]\n\n");

      } else {

        if (optiune_logare) {

          printf("\n[Nu v-ati inregistrat inca!]\n\n");
          optiune_logare = 0;
          continue;
        }
      }

    } else if (optiune_logare == 3) {
      printf("\n[OBS!: Jucand ca oaspete nu veti putea progresa in joc]\n\n");
    }

    //Se va iesi din while loop numai daca se alege optiunea guest ori logare reusita
  } while (optiune_logare != 3 && logare_reusita == 0);

  /*----------C-O-N-T--S-T-O-P---------*/

  /*----------J-O-C--S-T-A-R-T---------*/
  do {
    //Se intra in meniul pricipal
    printf("---------TRIVIA---------\n\n++++[MENIU PRINCIPAL]++++\n\n1.\t Joaca\n2.\t Profil\n3.\t Despre\n0.\t Iesire\n\nAlege: ");
   fflush(stdout);
    while ((scan = scanf("%d", & optiune_meniu)) == 0) {
     fflush(stdout);
      while ((c = getchar()) != '\n' && c != EOF) {}
      scanf("%*[^0-3\n]");
     fflush(stdout);
    }

    //Optiunea Joc
    if (optiune_meniu == 1) {

      do {

        printf("\n1. Intra In Meci!\n2. Alege dificultatea (Default: Usor)\n3. Exclude subiecte (Default: Nimic)\n0. Inapoi\n\nAlege: ");
       fflush(stdout);
        //Citim doar numerele din meniu
        while ((scan = scanf("%d", & optiune_joc)) == 0) {
         fflush(stdout);
          while ((c = getchar()) != '\n' && c != EOF) {}
          scanf("%*[^0-3\n]");
         fflush(stdout);
        }
        //Aici incepem meciul
        if (optiune_joc == 1) {

          srand(time(NULL));

          //VARIABILE MECI
          const int Perioada = 10;
          int timp = 5, random[10];
          const char * intrebari[10] = {
            "./src/Usor/Intrebari/Arta_intrebari_usor.data",
            "./src/Usor/Intrebari/Geografie_intrebari_usor.data",
            "./src/Usor/Intrebari/Istorie_intrebari_usor.data",
            "./src/Usor/Intrebari/Literatura_intrebari_usor.data",
            "./src/Usor/Intrebari/Mancare_intrebari_usor.data",
            "./src/Usor/Intrebari/Mitologie_intrebari_usor.data",
            "./src/Usor/Intrebari/Religii_intrebari_usor.data",
            "./src/Usor/Intrebari/Sport_intrebari_usor.data",
            "./src/Usor/Intrebari/Stiinte_intrebari_usor.data",
            "./src/Usor/Intrebari/Tehnologie_intrebari_usor.data"
          };
          const char * optiuni[10] = {
            "./src/Usor/Optiuni/Arta_optiuni_usor.data",
            "./src/Usor/Optiuni/Geografie_optiuni_usor.data",
            "./src/Usor/Optiuni/Istorie_optiuni_usor.data",
            "./src/Usor/Optiuni/Literatura_optiuni_usor.data",
            "./src/Usor/Optiuni/Mancare_optiuni_usor.data",
            "./src/Usor/Optiuni/Mitologie_optiuni_usor.data",
            "./src/Usor/Optiuni/Religii_optiuni_usor.data",
            "./src/Usor/Optiuni/Sport_optiuni_usor.data",
            "./src/Usor/Optiuni/Stiinte_optiuni_usor.data",
            "./src/Usor/Optiuni/Tehnologie_optiuni_usor.data"
          };
          const char * raspunsuri[10] = {
            "./src/Usor/Raspunsuri/Arta_raspunsuri_usor.data",
            "./src/Usor/Raspunsuri/Geografie_raspunsuri_usor.data",
            "./src/Usor/Raspunsuri/Istorie_raspunsuri_usor.data",
            "./src/Usor/Raspunsuri/Literatura_raspunsuri_usor.data",
            "./src/Usor/Raspunsuri/Mancare_raspunsuri_usor.data",
            "./src/Usor/Raspunsuri/Mitologie_raspunsuri_usor.data",
            "./src/Usor/Raspunsuri/Religie_raspunsuri_usor.data",
            "./src/Usor/Raspunsuri/Sport_raspunsuri_usor.data",
            "./src/Usor/Raspunsuri/Stiinte_raspunsuri_usor.data",
            "./src/Usor/Raspunsuri/Tehnologie_raspunsuri_usor.data"
          };
         FILE *fisiere_intrebari[10], *fisiere_optiuni[10], *fisiere_raspunsuri[10], *fisier_jucator;
          char raspuns[11] = {0};

          //Count down
          printf("\nMeciul incepe in: ");
          while (timp >= 1) {
            printf("%d ", timp);
           fflush(stdout);
            sleep(1);
            timp--;
          }
          //deschidem toatefisierele
          Deschide_fisier(fisiere_intrebari, intrebari, Perioada, "r");
          Deschide_fisier(fisiere_optiuni, optiuni, Perioada, "r");
          Deschide_fisier(fisiere_raspunsuri, raspunsuri, Perioada, "r");

         for (int i = 0; i < Perioada; i++) {
            int linie_num = 0;
            char buffer[200];
            //folosimfgets pentru a citii intrevarile pe rand
            while (fgets(buffer, sizeof(buffer),fisiere_intrebari[i]) != NULL) {
              linie_num++;
            }
            //folosim un vector de numere aleatorii pentrufie care intrebare din cele 10fisiere
            random[i] = rand() % linie_num + 1;
          }
          //reinitializam numarul de raspunsuri gresite pentru o noua sesiune de meci
          raspuns_gresit = 0;
         for (int i = 0; i < Perioada; i++) {
            //verificam daca modul de joc ales corespunde cu numarul raspunsuri gresite
            //daca nu jocul se termina
            if ((dificultate == 1 && raspuns_gresit == 5) || (dificultate == 2 && raspuns_gresit == 3) || (dificultate == 3 && raspuns_gresit == 1)) {
              printf("\n\t[GAME OVER]\n");
              scor = 0;
              break;
            }
            //verificam daca avem un subiect exclus
            if (exclude[i] == 0) {

              char buffer[200];
              int linie = 0;
              rewind(fisiere_intrebari[i]);

              while (fgets(buffer, sizeof(buffer),fisiere_intrebari[i])) {
                linie++;

                if (linie == random[i]) {
                  printf("\nIntrebarea %d:\n-------------\n%s\n", i + 1, buffer);
                 fflush(stdout);
                  break;
                }
              }

              linie = 0;
              rewind(fisiere_optiuni[i]);
              while (fgets(buffer, sizeof(buffer),fisiere_optiuni[i])) {
                linie++;
                if (linie == random[i]) {
                  printf("%s\n\nRaspuns: ", buffer);
                 fflush(stdout);
                  char c;
                  do {
                    while (getchar() != '\n'); // curatam bufferul
                    scanf("  %c", & c);
                   fflush(stdin);

                    if (c != 'a' && c != 'b' && c != 'c') {
                      printf("Litera invalida. Alegeti dintre a, b si c.\n");
                     fflush(stdout);
                    } else {
                      raspuns[i] = c;
                      break;
                    }
                  } while (1);

                }
              }

              //Verificare raspuns corect
              char buffer2[200];
              linie = 0;
              while (fgets(buffer2, sizeof(buffer),fisiere_raspunsuri[i])) {
                linie++;
                //raspuns corect
                if (linie == random[i] && raspuns[i] - 32 == buffer2[0]) {
                  scor++;
                  //notam raspunsurile corecte pentrufie care subiect in parte
                  subiect_corect_raspuns[i]++;
                } else if (linie == random[i] && raspuns[i] - 32 != buffer2[0]) {
                  raspuns_gresit++;
                }
              }
            } else continue;
          }

          if (dificultate == 1)
            scor /= 2;
          if (dificultate == 3)
            scor += scor;
          //in caz de penalizari, ex: subiecte exluse
          scor = scor - penalizare_scor;
          printf("\n\t[SCORUL: %d]\n", scor);

          if (optiune_logare != 3) {

            //Actiuni speciale pentru jucator logat

            Jucator.ultimul_scor = scor;
            Jucator.puncte_level += scor;

           for (int i = 0; i < 10; i++) {
              if (Jucator.puncte_level > level_ups[i])
                nivel_curent = i + 1;
            }
            printf("\n   [Sunteti la nivelul %d]\n", nivel_curent);

           fisier_jucator =fopen("Date_jucator.data", "w");

            if (fisier_jucator == NULL) {
              printf("Fisierul date jucator nu exista");
              return 0;
            }

           fprintf(fisier_jucator, "Informatii ultima sesiune de joc\n\nNume jucator: %s\nUltimul scor: %d\nScor total acumulat: %d\nNivel: %d", username, Jucator.ultimul_scor, Jucator.puncte_level, nivel_curent);

           fclose(fisier_jucator);
          }

          //Inchide toatefisierele
          Inchide_fisier(fisiere_intrebari, Perioada);
          Inchide_fisier(fisiere_optiuni, Perioada);
          Inchide_fisier(fisiere_raspunsuri, Perioada);

        }
        if (optiune_joc == 3) {
          do {
            printf("\n1. Istorie\n2. Geografie\n3. Literatura\n4. Stiinta\n5. Sporturi\n6. Arta și arhitectura\n7. Tehnologie\n8. Mitologie\n9. Mancare si bautura\n10. Religii ale lumii\n\n0. Inapoi\n\nAlege: ");
           fflush(stdout);
            while ((scan = scanf("%d", & optiune_excludere)) == 0) {
             fflush(stdout);
              while ((c = getchar()) != '\n' && c != EOF) {}
              scanf("%*[^0-10\n]");
             fflush(stdout);
            }

            if (optiune_excludere != 0)
              printf("\n[Ati exclus optiunea %d! aceasta alegere vafi penalizata]\n", optiune_excludere);

            exclude[optiune_excludere - 1] = 1;

           for (int i = 0; i <= 9; i++) {
              if (exclude[i] != 0) {
                penalizare_scor++;
              }
            }

          } while (optiune_excludere != 0);
        } else if (optiune_joc == 2) {
          int op_dif;

          printf("\n1. Usor\n2. Greu\n3. Imposibil\n0. Inapoi\n\nAlege: ");
         fflush(stdout);
          while ((scan = scanf("%d", & op_dif)) == 0) {
           fflush(stdout);
            while ((c = getchar()) != '\n' && c != EOF) {}
            scanf("%*[^0-3\n]");
           fflush(stdout);
          }

          if (op_dif == 1) {
            if (dificultate == 1)
              printf("\n[Aceasta este deja dificultatea default sau cea curenta]\n");
            else {
              printf("\n[In modul usor vei primii doar jumatate din punctaj,\ndar aveti voie la 5 raspunsuri gresite, mult noroc!]\n");
              dificultate = 1;
            }

          } else if (op_dif == 2) {
            if (dificultate == 2)
              printf("\n[Aceasta este deja dificultatea curenta]\n");
            else {
              printf("\n[In modul greu ai voie la 3 raspunsuri gresite\nDe asemenea vei primii punctaj valoarea_maxima_vector_subiecteim,mult noroc!]\n");
              dificultate = 2;
            }
          } else if (op_dif == 3) {
            if (dificultate == 3)
              printf("\n[Aceasta este deja dificultatea curenta]\n");
            else {
              printf("\n[In modul Imposibil nu poti gresii nici odata\ndar vei primii 200%% punctaj ,ai grija!]\n");
              dificultate = 3;
            }
          }
        }

      } while (optiune_joc != 0);
    }
    //Optiunea profil, aici putem gasii sectiunea statistici unde se vor prelucra toate datele playerului
    if (optiune_meniu == 2) {
      do {
        printf("\n1. Informatii Profil\n2. Statistici\n0. Inapoi\n\nAlege: ");
       fflush(stdout);
        while ((scan = scanf("%d", & optiune_profil)) == 0) {
         fflush(stdout);
          scanf("%*[^0-2\n]");
         fflush(stdout);
        }

        if (optiune_profil == 1) {
          if (optiune_logare != 3) {

           FILE *fisier_jucator;
            char buffer[100];

           fisier_jucator =fopen("Date_jucator.data", "r");
            if (fisier_jucator == NULL) {
              printf("[Fisierul nu exista!]");
              return 1;
            }
            while (fgets(buffer, sizeof(buffer),fisier_jucator)) {
              printf("%s", buffer);
              printf("\n");
            }

           fclose(fisier_jucator);

          } else if (optiune_profil == 1 && optiune_logare == 3) printf("\n[Nu sunteti logat!]\n");
        }
        if (optiune_profil == 2 && optiune_logare != 3) {

          printf("\n0. Istorie\n1. Geografie\n2. Literatura\n3. Stiinta\n4. Sporturi\n5. Arta și arhitectura\n6. Tehnologie\n7. Mitologie\n8. Mancare si bautura\n9. Religii ale lumii");
          //printf("%d",*(pv+1));
         fflush(stdout);
          glutInit( & argc, argv);

          glutInitWindowSize(600, 600);
          glutCreateWindow("TRIVIA");

          glutDisplayFunc(afisare);

          glutMainLoop();

        } else if (optiune_profil == 2 && optiune_logare == 3) printf("\n[Nu sunteti logat!]\n");

      } while (optiune_profil != 0);
    }

    //Optiunea pentru informatii aditionale
    if (optiune_meniu == 3) {
      do {
        //VARIABILE NAVIGARE "DESPRE"
        int op_int;

        printf("\n1. Intrebarifrecvente\n2. Contact sifeedback\n0. Inapoi\n\nAlege: ");
       fflush(stdout);
        while ((scan = scanf("%d", & optiune_despre)) == 0) {
          while ((c = getchar()) != '\n' && c != EOF) {}
          scanf("%*[^0-2\n]");
         fflush(stdout);
        }

        if (optiune_despre == 1) {
          do {
            printf("\n1. Cumfunctioneaza jocul?\n2. De cine este creeat?\n0. Inapoi\n\nAlege: ");
           fflush(stdout);
            scanf("%d", & op_int);
            if (op_int == 1)
              printf("\nModul defunctionare, cat si regulile jocului sunt adresate in PDF-ul atasat\n\n");
            else if (op_int == 2)
              printf("\nJocul este creat de Chelaru Ionut\nStudent lafacultatea Inginerie Electrica,Energetica si Informatica Aplicata\nGrupa 6105-An I\n\n");

          } while (op_int != 0);
        }
        if (optiune_despre == 2)
          printf("\n[Contact sifeedback] Email: ionutchelaru2@student.tuiasi.ro\n");

      } while (optiune_despre != 0);
    }

  } while (optiune_meniu != 0);
  printf("\n\t[Joc Inchis]");

  /*----------J-O-C--S-T-O-P-----------*/

  return 0;
}

void Deschide_fisier(FILE **fisier,
  const char ** numefisier, int numar,
    const char * mod) {
 for (int i = 0; i < numar; i++) {
   fisier[i] =fopen(numefisier[i], mod);
    if (fisier[i] == NULL) {
      printf("Eroare:fisiernul %s nu sa deschis!\n", numefisier[i]);
      return;
    }
  }
}

void Inchide_fisier(FILE **fisier, int numar) {
 for (int i = 0; i < numar; i++) {
    if (fisier[i] != NULL) {
     fclose(fisier[i]);
    }
  }
}

void afisare_text(float x,float y, void *font,
  const char * string) {
  glRasterPos2f(x, y);
  const char * c;
 for (c = string;* c != '\0'; ++c) {
    glutBitmapCharacter(font, * c);
  }
}

void afisare() {
  int subiect;
  char subiect_nume[30];
  char numar_subiect[3];
 float coeficient_inaltime;
 float valoarea_maxima_vector_subiecte = subiect_corect_raspuns[0];
 for (int i = 0; i < 10; i++) {
    if (subiect_corect_raspuns[i] > valoarea_maxima_vector_subiecte) {
      valoarea_maxima_vector_subiecte = subiect_corect_raspuns[i];
      subiect = i + 1;
    }
  }
  coeficient_inaltime = 1.2 / valoarea_maxima_vector_subiecte;

  glClearColor(255 / 255.0f, 186 / 255.0f, 8 / 255.0f, 0.5f);
  glClear(GL_COLOR_BUFFER_BIT);

  glColor3f(3 / 255.0f, 7 / 255.0f, 30 / 255.0f);
  glBegin(GL_QUADS);
  glVertex2f(-1.0f, -1.0f);
  glVertex2f(-1.0f, 0.6f);
  glVertex2f(1.0f, 0.6f);
  glVertex2f(1.0f, -1.0f);

  glEnd();

  glBegin(GL_QUADS);
  glColor3f(244 / 255.0f, 140 / 255.0f, 6 / 255.0f);
  glVertex2f(-0.9f, -0.9f);
  glVertex2f(-0.9f, 0.5f);
  glColor3f(250 / 255.0f, 163 / 255.0f, 7 / 255.0f);
  glVertex2f(0.9f, 0.5f);
  glVertex2f(0.9f, -0.9f);

  glEnd();
  glColor3f(0.0f, 0.0f, 0.0f);
  afisare_text(-0.15f, 0.9f, GLUT_BITMAP_TIMES_ROMAN_24, "TRIVIA");
  afisare_text(-0.7f, 0.8f, GLUT_BITMAP_9_BY_15, "Diagrama arata raspunsurile corecte per subiect");
  switch (subiect) {
  case 1:
    strcpy(subiect_nume, "Istorie");
    break;
  case 2:
    strcpy(subiect_nume, "Geografie");
    break;
  case 3:
    strcpy(subiect_nume, "Literatura");
    break;
  case 4:
    strcpy(subiect_nume, "Stiinta");
    break;
  case 5:
    strcpy(subiect_nume, "Sporturi");
    break;
  case 6:
    strcpy(subiect_nume, "Arta și arhitectura");
    break;
  case 7:
    strcpy(subiect_nume, "Tehnologie");
    break;
  case 8:
    strcpy(subiect_nume, "Mitologie");
    break;
  case 9:
    strcpy(subiect_nume, "Mancare si bautura");
    break;
  case 10:
    strcpy(subiect_nume, "Religii ale lumii");
    break;

  }
  afisare_text(-0.6f, 0.7f, GLUT_BITMAP_9_BY_15, "Va pricepeti cel mai bine in: ");
  afisare_text(0.3, 0.7f, GLUT_BITMAP_9_BY_15, subiect_nume);
  glColor3f(1.0f, 1.0f, 1.0f);
  afisare_text(-0.55f, -0.95f, GLUT_BITMAP_HELVETICA_10, "Obs!: Numerele de jos sunt corespunzatoare subiecteleor afisate in consola");

 float grosime_dreptunghi = 0.125f;
 float spatiu_fix = 0.05f;

 for (int i = 0; i < 10; i++) {
   float spatiu = i * (grosime_dreptunghi + spatiu_fix);
    glColor3f(220 / 255.0f, 47 / 255.0f, 2 / 255.0f);
    glBegin(GL_QUADS);
    glVertex2f(-0.85f + spatiu, -0.85f);
    glColor3f(225 / 255.0f, 80 / 255.0f, 20 / 255.0f);
    glVertex2f(-0.85f + spatiu, -0.85f + subiect_corect_raspuns[i] * coeficient_inaltime);
    glVertex2f(-0.85f + spatiu + grosime_dreptunghi, -0.85f + subiect_corect_raspuns[i] * coeficient_inaltime);

    glVertex2f(-0.85f + spatiu + grosime_dreptunghi, -0.85f);

    glEnd();

    char numar = '0' + i;
    glColor3f(1 / 255.0f, 1 / 255.0f, 1 / 255.0f);
    // afisare_text(-0.8+spatiu,-0.8,GLUT_BITMAP_9_BY_15,numar);
    glRasterPos2f(-0.8 + spatiu, -0.8); // Position the raster positionfor each number
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, numar);

    snprintf(numar_subiect, sizeof(numar_subiect), "%d", subiect_corect_raspuns[i]);
   for (int j = 0; j < 2; j++) {
     float spatiu = i * (grosime_dreptunghi + spatiu_fix);

      glColor3f(0.0f, 0.0f, 1.0f);
      if (numar_subiect[2] != '\0') {
        glRasterPos2f(-0.8 + spatiu, 0.45);
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, numar_subiect[j]);
      } else {
        glRasterPos2f(-0.8 + spatiu, 0.45);
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, numar_subiect[0]);
        glRasterPos2f(-0.76 + spatiu, 0.45);
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, numar_subiect[1]);

      }
    }
  }

  glFlush();
  glutSwapBuffers();

}
