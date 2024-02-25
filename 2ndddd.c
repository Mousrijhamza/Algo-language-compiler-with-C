#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct Jeton {
    char *data;
    int num_ligne;
    int num_colonne;
    int position;
    struct Jeton *suivant;
} *jeton;

typedef struct {
    char* name;
    char* type;
} variable;

//static variable* vars= (variable*)malloc(sizeof(variable) * 50);
// Déclaration de la variable globale
static variable* vars;

// Fonction d'initialisation appelée au début du programme
void initializeVars() {
    vars = (variable*)malloc(sizeof(variable) * 50);
    if (vars == NULL) {
        // Gérer l'échec de l'allocation mémoire si nécessaire
        exit(EXIT_FAILURE);
    }
}

static int cmp =0;


jeton deconstruction_code_source_en_partie_atomique(const char *nom_fichier, char *nom_fichier_dictionnaire);
jeton Add_jeton(jeton Liste, const char *tab, int num_ligne, int num_colonne, int position);
void Analyse_lix(char *str, char *dict, int ligne, int colonne);
jeton creerListeFils(jeton listeMere, int pointDepart, int pointArrivee);
void erreur(const char* message, int* ligne);
void Afficher_liste(jeton q);
void Analyse_synt(jeton token);
int Search_In_Liste(jeton tokens, const char *motSpecifique);

int does_var_exist(char* v, variable* v1);
void handle_Ecrire(jeton mot);
void handle_Lire(jeton mot);
void handle_Debut(jeton mot);
void handle_Declaration(jeton mot);
void handle_Si(jeton mot);




int main() {
    jeton Liste1 = NULL;
    char *dictionnaire = "dictionnaire.txt";
    Liste1 = deconstruction_code_source_en_partie_atomique("code_source.txt", dictionnaire);

    // Analyse lexicale
    printf("\nElements dans la liste :\n");
    Afficher_liste(Liste1);
    printf("\n");

    // Analyse syntaxique
    Analyse_synt(Liste1);
    initializeVars();
    //printf("\nphase symontique\n");
    //analyse_semontique(Liste1);

    return 0;
}

jeton Add_jeton(jeton Liste, const char *tab, int num_ligne, int num_colonne, int position) {
    jeton new_node = malloc(sizeof(struct Jeton));
    new_node->data = strdup(tab);
    new_node->num_ligne = num_ligne;
    new_node->num_colonne = num_colonne;
    new_node->suivant = NULL;

    if (Liste == NULL) {
        Liste = new_node;
    } else {
        jeton current = Liste;
        while (current->suivant != NULL) {
            current = current->suivant;
        }
        current->suivant = new_node;
    }
    return Liste;
}

// Fonction pour créer une nouvelle liste (fils) à partir d'une liste mère
jeton creerListeFils(jeton listeMere, int pointDepart, int pointArrivee) {
    jeton nouvelleListe = NULL;
    jeton tempMere = listeMere;

    // Parcourir la liste mère jusqu'au point de départ
    while (tempMere != NULL && tempMere->position != pointDepart) {
        tempMere = tempMere->suivant;
    }

    // Si le point de départ est trouvé
    if (tempMere != NULL) {
        // Ajouter les éléments à la nouvelle liste jusqu'au point d'arrivée
        while (tempMere != NULL && tempMere->position <= pointArrivee) {
            // Utiliser la fonction Add_jeton pour créer et ajouter un nouveau jeton
            nouvelleListe = Add_jeton(nouvelleListe, tempMere->data, tempMere->num_ligne, tempMere->num_colonne, pointArrivee - tempMere->position);

            tempMere = tempMere->suivant;
        }
    }

    return nouvelleListe;
}


void Analyse_lix(char *str, char *dict, int ligne, int colonne) {
    FILE *dictionnaire = fopen(dict, "r");
    char *lexique = (char *)malloc(sizeof(char) * 40);
    int i = 0;
    char c;
    int trouve = 0;
    while (!feof(dictionnaire)) {
        c = fgetc(dictionnaire);
        if (c != ' ' && c != '\n') {
            *(lexique + i) = c;
            i++;
        } else {
            *(lexique + i) = '\0';
            if (strcmp(str, lexique) != 0) {
                trouve = 0;
            } else {
                trouve = 1;
                break;
            }
            i = 0;
        }
    }

    //printf("%d\n", colonne);
    fclose(dictionnaire);
    if (trouve == 0) {
        FILE *errors = fopen("error1.txt", "a+");
        fprintf(errors, "Erreur ligne, colonne %d, %d : '%s' n'exist pas dans le dictionnaire de la langue.\n", ligne, colonne, str);
        fprintf(errors, "----------------------------------------------------------------------------------------\n");
        fclose(errors);
        printf("Erreur ligne, colonne %d, %d : '%s' n'exist pas dans le dictionnaire de la langue.\n", ligne, colonne, str);
    }
}

jeton deconstruction_code_source_en_partie_atomique(const char *nom_fichier, char *nom_fichier_dictionnaire) {
    jeton Liste = NULL;
    FILE *f = fopen(nom_fichier, "r");

    if (f == NULL) {
        printf("Impossible d'ouvrir le fichier.\n");
        exit(-1);
    }
    
    int counter_char = 0; //compteur charactere
    int counter_word = 0; // compteur mot 
    char letter;
    char tab[10] = " ";
    int i_memo_cln = 0; // variable qui compte tous les caracteres meme si les espaces dans une seul ligne
    int i = 0;
    int num_ligne = 1;
    int num_colonne = 0;// cette variable est attacher au mot dans le fichier code source indiquant la position du mot

    while ((letter = fgetc(f)) != EOF) {
        i_memo_cln++;

        if (letter == ' ' || letter == '\t' || letter == '\n') {
            if (i > 0) {
                counter_word++;
                num_colonne = i_memo_cln - strlen(tab);
                Analyse_lix(tab, nom_fichier_dictionnaire, num_ligne, num_colonne);
                Liste = Add_jeton(Liste, tab, num_ligne, num_colonne, counter_word);
                tab[0] = '\0';
                i = 0;
            }
            if (letter == '\n') {
                //nouvelle ligne du fichier code source
                num_ligne++;
                i_memo_cln = 0;
                i = 0;
            }

        } else {
            counter_char++;
            tab[i] = letter;
            i++;
        }
        tab[i] = '\0';
    }

    fclose(f);
    return Liste;
}

void Afficher_liste(jeton q) {
    jeton iter;
    for (iter = q; iter != NULL; iter = iter->suivant) {
        printf("%s ---> %d, %d\n", iter->data, iter->num_colonne, iter->num_ligne);
    }
}


// Fonction pour Afficher_liste les erreurs
void erreur(const char* message, int* ligne) {
    FILE *fichier = fopen("erreur_syntaxique.txt", "a+");  // "a" pour ajouter au fichier s'il existe, le créer sinon

    if (fichier != NULL) {
        if (ligne != NULL) {
            fprintf(fichier, "Erreur syntaxique : %s a la ligne %d .\n", message, *ligne);
            printf("Erreur syntaxique : %s a la ligne %d .\n", message, *ligne);
        
        } else {
            fprintf(fichier, "Erreur syntaxique : %s .\n", message);
            printf("Erreur syntaxique : %s .\n", message);
        
        }

        fclose(fichier);  // N'oubliez pas de fermer le fichier après avoir écrit
    } else {
        // Gestion de l'erreur d'ouverture du fichier
        printf("Erreur lors de l'ouverture du fichier erreur_syntaxique.txt.\n");
    }
}

int Search_In_Liste(jeton tokens, const char *motSpecifique) {
    jeton temp;
    for (temp = tokens; temp != NULL; temp = temp->suivant) {
        if (strcmp(temp->data, motSpecifique) == 0) {
            // Traitement lorsque le mot spécifique est trouvé
            //printf("%s trouvé à la ligne %d.\n", motSpecifique, temp->num_ligne);
            return temp->position;  // Sortir de la fonction dès que le mot est trouvé
        }
    }
    char message[100];
    sprintf(message, "%s manquante pour Si", motSpecifique);

    // Si le mot spécifique n'est pas trouvé, Afficher_liste une erreur et quiter
    erreur(message, (temp != NULL) ? &tokens->num_ligne : NULL);
    return 0;
}

//fonction pour tester si une variable est deja declarer
int does_var_exist(char* v, variable* v1) {
    int found = 0;

    for (int i = 0; i <= cmp && (vars + i)->name != NULL && (vars + i)->type != NULL; i++) {
        if (!strcmp((vars + i)->name, v)) {
            *v1 = *(vars + i);
            found = 1;
            break;
        } else {
            found = 0;
        }
    }

    return found;
}

// Fonction pour traiter le mot-clé "Ecrire"
void handle_Ecrire(jeton mot) {
    char *Ecrire_tab[] = {"Ecrire", "(", "<<", " ", ">>", ",", "<<", " ", ">>", ",", " ", ")", ";"};
    // le syntaxe d 'Ecrire
    //Ecrire ( << %E >> , << message >> , A ) ;
    jeton temp = mot ;
    int lenght;

    if (temp == NULL) {
        printf("Vide");
        return;
    }

    int i = 0;
    for(i = 0; i < 13 ; i++) {
        printf("%d\n", i);

        if (!strcmp(temp->data, Ecrire_tab[i]) && i != 3 && i == 7 && i == 10) {
            

        } else {
            if(i == 3 || i == 7 || i == 10) {
                
                if (i == 3 ) {
                    if (strcmp(temp->data, "%E") != 0 && strcmp(temp->data, "%C") != 0 && strcmp(temp->data, "%R") != 0) {

                        erreur(strcat(strcat(temp->data, "~"), Ecrire_tab[i]), (temp != NULL) ? &(temp->num_ligne) : NULL);
                        break;

                    }
                    
                }
                if (i == 7);
                
                if ( i == 10 ) {
                    lenght = strlen(temp->data) ;
                    if (lenght != 1)
                    {
                        erreur(strcat(strcat(temp->data, "~"), Ecrire_tab[i]), &(temp->num_ligne));
                        break;

                    }
                
                }

            } else {

                erreur(strcat(strcat(temp->data, "~"), Ecrire_tab[i]), &(temp->num_ligne));
                break;
                
            }
        }
        if (temp->suivant == NULL)
        {
            i++;
            erreur(Ecrire_tab[i], NULL);
            break;
        }
        temp = temp->suivant;
    }

}
//Fonction pour traiter le mot-cle "Lire"

void handle_Lire(jeton mot) {
    //le syntaxe de lire :
    // Lire ( V ) ;
    char * lire_tab[] = {"Lire", "(", " ", ")", ";"};
    jeton temp = mot ;

    if (temp == NULL) {
        printf("Vide");
        return;
    }

    int i = 0;
    while (temp != NULL) {
        
        if (!strcmp(temp->data, lire_tab[i]) && i != 2) {
            i++;
        } else {
            if(i == 2) {
        
                erreur(strcat(strcat(temp->data, "~"), lire_tab[i]), &(temp->num_ligne));
                i++;
        
            } else {
                erreur(strcat(strcat(temp->data, "~"), lire_tab[i]), &(temp->num_ligne));
                i++;
            }
        }
        temp = temp->suivant;
    }
}

//Fonction pour traiter le mot-cle "Debut"
void handle_Debut(jeton mot) {
    //le syntaxe de debut :
    // Debut 
    // tous le code jusqu'a
    // Fin
    char * debut_tab[] = {"Debut", " ","Fin"};
    jeton temp = mot ;

    if (temp == NULL) {
        printf("Vide");
        return;
    }

    temp = mot->suivant;
    int i = 0;
    while (temp != NULL) {
        
        if (!strcmp(temp->data, debut_tab[i]) && i != 1) {
            i++;
        } else {
            if(i == 1) {
        
                erreur(strcat(strcat(temp->data, "~"), debut_tab[i]), &(temp->num_ligne));
                i++;
        
            } else {
                erreur(strcat(strcat(temp->data, "~"), debut_tab[i]), &(temp->num_ligne));
                i++;
            }
        }
        temp = temp->suivant;
    }

}

//Fonction pour traiter le mot-cle "Declaration"

void handle_Declaration(jeton mot) {
    char *declaration_tab[] = {"Variable", " ", ":", " ", ";"};
    // le syntaxe d 'variable
    //Variable V : entier || char || reel ;
    jeton temp = mot ;

    if (temp == NULL) {
        printf("Vide");
        return;
    }

    int i = 0;
    while (temp != NULL) {
        
        if (!strcmp(temp->data, declaration_tab[i]) && i != 1 && i != 3) {
            i++;
        } else {
            if(i == 3 || i == 1) {
                
                if (i == 3 ) {
                    if (strcmp(temp->data, "entier") != 0 && strcmp(temp->data, "reel") != 0 && strcmp(temp->data, "char") != 0) {
                       
                        erreur(strcat(strcat(temp->data, "~"), declaration_tab[i]), (temp != NULL) ? &(temp->num_ligne) : NULL);

                    }
                    i++;
                }
                
                if (i == 1 && strlen(temp->data) != 1) {
                    erreur(strcat(strcat(temp->data, "~"), declaration_tab[i]), &(temp->num_ligne));
                    i++;
                }

            } else {
                erreur(strcat(strcat(temp->data, "~"), declaration_tab[i]), &(temp->num_ligne));
                i++;
            }
        }
        temp = temp->suivant;
    }

}

//Fonction pour traiter le mot-cle "Si"

void handle_Si(jeton mot) {
    char *Si_inst[] = {"Si", "(", " ", " ", " ", ")", "alors", "Sinon" ," ", "Finsi", ";"};
    
    // le syntaxe de si
    //Si ( V > 3 ) alors
    //Sinon
    // n'importe
    //Finsi

    jeton temp = mot;
    if (temp == NULL) {
        printf("Vide");
        return;
    } 
    int i = 0;
    while (temp != NULL) {
        
        if (!strcmp(temp->data, Si_inst[i]) && i != 2 && i == 3 && i == 4 && i == 8) {
            i++;
        } else {
            if(i == 2 || i == 3 || i == 4 || i == 8) {
                
                if (i == 3 ) {
                    if (strcmp(temp->data, "<") != 0 && strcmp(temp->data, ">") != 0 && strcmp(temp->data, "<=") != 0 && strcmp(temp->data, ">=") != 0 && strcmp(temp->data, "!=") != 0 && strcmp(temp->data, "==") != 0) {
                       
                        erreur(strcat(strcat(temp->data, "~"), Si_inst[i]), (temp != NULL) ? &(temp->num_ligne) : NULL);

                    }
                    i++;
                }
                if (i == 8) i++; //ignore, faut etre une instruction qui sera traiter ulterieument
                
                if ((i == 2 || i == 4) && strlen(temp->data) != 1) {
                    erreur(strcat(strcat(temp->data, "~"), Si_inst[i]), &(temp->num_ligne));
                    i++;
                }

            } else {
                erreur(strcat(strcat(temp->data, "~"), Si_inst[i]), &(temp->num_ligne));
                i++;
            }
        }
        temp = temp->suivant;
    }

}

void Analyse_synt(jeton token) 
{
    jeton tokens = token ;
    static int test_debut = 0 ;
    static int test_Si = 0 ;

    while (tokens != NULL) {
        
        if (strcmp(tokens->data, "Debut") == 0) {
            test_debut++;
            handle_Debut(tokens);

        } else if (strcmp(tokens->data, "Lire") == 0) {
            handle_Lire(tokens);

        } else if (strcmp(tokens->data, "Ecrire") == 0) {
            handle_Ecrire(tokens);

        } else if (strcmp(tokens->data, "Si") == 0) {
            test_Si += 3 ;
            int Finsi_position = Search_In_Liste(tokens, "Finsi");
            
            // Si "Finsi" est trouvé dans la liste
            if (Finsi_position ) {
                jeton temp = tokens;
                jeton Liste_de_si = creerListeFils(temp, tokens->position, Finsi_position);
                handle_Si(Liste_de_si);
            }

        } else if (strcmp(tokens->data, "Variable") == 0) {
            handle_Declaration(tokens);

        } else if (strcmp(tokens->data, "Fin") == 0) {
            if (test_debut == 0) {
                erreur("Debut manquant avant", (tokens != NULL) ? &tokens->num_ligne : NULL);
            }
            // Réinitialiser le test_debut pour une nouvelle instruction "Debut"
            test_debut = 0;

        } else if (!strcmp(tokens->data, "alors") || !strcmp(tokens->data, "Sinon") || !strcmp(tokens->data, "Finsi") ) {   
            if (test_Si == 0) {
                erreur("Si manquante avant", (tokens != NULL) ? &tokens->num_ligne : NULL);
                test_Si--;
            }
        }

        tokens = tokens->suivant;
    }
}



// Symbole getSymbole(const char* data) {
//     if (strcmp(data, "Si") == 0) {
//         return Si;
//     } else if (strcmp(data, "Declaration") == 0) {
//         return Declaration;
//     } else if (strcmp(data, "Lire") == 0) {
//         return Lire;
//     } else if (strcmp(data, "Ecrire") == 0) {
//         return Ecrire;
//     } else if (strcmp(data, "Debut") == 0) {
//         return Debut;
//     }
//     return 0;
// }

// Fonction principale pour l'analyse syntaxique
// void analyse_syntaxique(jeton token) {
//     jeton tokens = token;

//     while (tokens != NULL) {
//         Symbole symbole = getSymbole(tokens->data);

//         switch (symbole) {
//             case Si:
//                 handle_Si(tokens);
//                 continue;
//             case Lire:
//                 handle_Lire(tokens);
//                 continue;
//             case Ecrire:
//                 handle_Ecrire(tokens);
//                 continue;
//             case Debut:
//                 handle_Declaration(tokens);
//                 continue;                
//             default:
//                 // GÃ©rer le cas par dÃ©faut      nÃ©cessaire
//                 continue;
//         }

//         tokens = tokens->suivant;
//     }
// }

///////////////////////////////////////////////////////
int estChiffre(char c) {
    return c >= '0' && c <= '9';
}

int estEntier(const char* mot) {
    int longueur = strlen(mot);

    if (longueur == 1) {
        return estChiffre(*mot);
    } else {
        for (int i = 0; i < longueur; i++) {
            if (!estChiffre(mot[i])) {
                return 0;  // Faux
            }
        }
        return 1;  // Vrai
    }
}

int estMinuscule(char c) {
    return c >= 'a' && c <= 'z';
}

int estChar(const char* mot) {
    int longueur = strlen(mot);

    if (longueur == 1) {
        return estMinuscule(*mot);
    } else {
        return 0;  // Faux
    }
}

int estReel(const char* mot) {
    int longueur = strlen(mot);

    if (longueur == 1) {
        return estChiffre(*mot);
    } else {
        int aChiffre = 0;  // Faux
        for (int i = 0; i < longueur; i++) {
            if (estChiffre(mot[i]) || mot[i] == '.') {
                aChiffre = 1;  // Vrai
            } else {
                return 0;  // Faux
            }
        }
        return aChiffre;
    }
}








void handleComparison(jeton temp, variable *v1) {
    temp = temp->suivant;/*2*/
    
        if (estEntier(temp->data)) {
            handleIntComparison(temp, v1);

        } else if (estChar(temp->data)) {
            handleCharComparison(temp, v1);

        } else if (estReel(temp->data)) {
            handleFloatComparison(temp, v1);

        } else {
            printf(" erreur symontique  : value \'%s\' is not exact in line %d \n",temp->data, temp->num_ligne);

        }
   
}

// Fonction pour gérer la comparaison d'un entier
void handleIntComparison(jeton temp, variable *v1) {               
        if(!strcmp(v1->type, "entier")){
            temp = temp->suivant;
            if (!strcmp(temp->data, ";")) {
                temp = temp->suivant;
            
            }else
            {
                    erreur("Point-virgule manquant", (temp != NULL) ? &temp->num_ligne : NULL);
            }

        }else {
            printf(" erreur symontique  : type entier of variable \'%s\' is not exact in line %d \n",v1->name, temp->num_ligne);

        }
        
}

// Fonction pour gérer la comparaison d'un caractère
void handleCharComparison(jeton temp, variable *v1) {
        if (!strcmp(v1->type, "charcter")) {
            temp = temp->suivant;
            if (!strcmp(temp->data, ";")) {
            
            }
            else {
                    erreur("Point-virgule manquant", (temp != NULL) ? &temp->num_ligne : NULL);
            }
        }
        else {
            printf(" erreur symontique  : type char of variable \'%s\' is not exact in line %d \n",v1->name, temp->num_ligne);
        }
}

// Fonction pour gérer la comparaison d'un flottant
void handleFloatComparison(jeton temp, variable *v1) {
        if (!strcmp(v1->type, "reel")) {
            temp = temp->suivant;
            if (!strcmp(temp->data, ";")) {
                temp = temp->suivant;

            }
            else {
                erreur("Point-virgule manquant", (temp != NULL) ? &temp->num_ligne : NULL);

            }
        }
        else {

            printf(" erreur symontique  : type float of variable \'%s\' is not exact in line %d \n", v1->name, temp->num_ligne);
        }

}

void handleDeclaredVariable(jeton temp, variable *v1) {
    temp = temp->suivant;/*1*/
    
    if (strlen(temp->data) == 2 && strcmp(temp->data, ":=") == 0) {
        handleComparison(temp, v1);

    } else if (strlen(temp->data) == 1 || strcmp(temp->data, "=") == 0) {
        handleComparison(temp, v1);

    } else if (strcmp(temp->data, ">") == 0 || strcmp(temp->data, "<") == 0) {
        handleComparison(temp, v1);
        
    }else {
        printf(" erreur symontique in line %d \n", temp->num_ligne);

    }
}

void analyse_semontique(jeton liste) 
{
    variable *v1;
    v1 = (variable *)malloc(sizeof(variable) );
    jeton temp = liste;



    if (liste == NULL) {
        printf("Liste vide ");
        return;
    }

    while (temp != NULL) {
        if (!strcmp(temp->data, "Variable")) {
            temp = temp->suivant;

            if (strlen(temp->data) == 1 && isalpha(temp->data[0])) {
                
                (vars + cmp)->name = strdup(temp->data);
                temp = temp->suivant;

                if (strlen(temp->data) == 1 || strcmp(temp->data, ":") == 0) {
                    temp = temp->suivant;

                    if (!strcmp(temp->data, "entier") || !strcmp(temp->data, "reel") || !strcmp(temp->data, "character")) {
                        
                        (vars + cmp)->type = temp->data;  
                        cmp++;
                        temp = temp->suivant;

                        if (!strcmp(temp->data, ";")) {
                            temp = temp->suivant;
                        } else {
                            erreur("Point-virgule manquant", (temp != NULL) ? &temp->num_ligne : NULL);
                        }
                    } else {
                        erreur("Type de variable incorrect", (temp != NULL) ? &temp->num_ligne : NULL);
                    }
                } else {
                    erreur("Deux-points manquants", (temp != NULL) ? &temp->num_ligne : NULL);
                }
            } else {
                erreur("Variable manquante", (temp != NULL) ? &temp->num_ligne : NULL);
            }
        
        } else if (strlen(temp->data) == 1 && isalpha(temp->data[0])){

            if (does_var_exist(temp->data,v1)) {
                handleDeclaredVariable(temp, v1);
                /*precedent*/

            }
            else {
                printf(" erreur symontique  : variable \'%s\' not declared in line %d \n", temp->data, temp->num_ligne);
            }
        }
        else if (!strcmp(temp->data, "Lire"))
        {
            temp = temp->suivant;
            if (temp != NULL && !strcmp(temp->data, "(")) {
                temp = temp->suivant;

                if (temp != NULL && temp != NULL && strlen(temp->data) == 1 && isalpha(temp->data[0])) {
                    if (does_var_exist(temp->data, v1) ) {
                        temp = temp->suivant;

                        if (temp != NULL && temp != NULL && !strcmp(temp->data, ")")) {
                            printf("\nvoila voila %s", temp->data);
                            temp = temp->suivant;

                            if (temp != NULL && temp != NULL && strcmp(temp->data, ";") == 0) {
                                temp = temp->suivant;

                            }
                            else {
                                printf("error syntaxique missed \';\' in line %d \n", temp->num_ligne);

                            }
                        }
                        else {
                            printf("error syntaxique missed \')\' in line %d \n", temp->num_ligne);

                        }
                    } else {
                        printf(" erreur symontique  : variable \'%s\' not declared in line %d \n", temp->data, temp->num_ligne);

                    }
                } else {
                    printf("error syntaxique problem at variable in line %d \n", temp->num_ligne);

                }

            }

            else {
                printf("error syntaxique missed \'(\'  at line %d \n", temp->num_ligne + 1);
            }
        }
        else if (!strcmp(temp->data, "Ecrire"))
        {
        temp = temp->suivant;
        if (!strcmp(temp->data, "(")) {
            temp = temp->suivant;

            if (strlen(temp->data) == 1 && isalpha(temp->data[0])) {
                if (does_var_exist(temp->data, v1)) {

                    temp = temp->suivant;
                    if (!strcmp(temp->data, ")")) {
                        temp = temp->suivant;
                        if (!strcmp(temp->data, ";")) {
                            temp = temp->suivant;
                        }
                        else {
                            printf("error syntaxique missed \';\' in line %d \n", temp->num_ligne);
                        }
                    }
                    else {
                        printf("error syntaxique missed \')\' in line %d \n", temp->num_ligne);
                    }
                }
                else {
                    printf(" erreur symontique  : variable \'%s\' not declared in line %d \n", temp->data, temp->num_ligne);
                }
            }
            else {
                printf("error syntaxique problem at variable in line %d \n", temp->num_ligne);
            }

        }

        else {
            printf("error syntaxique missed \'(\'  at line %d \n", temp->num_ligne + 1);  
        }
        }
        if(temp != NULL){
            temp = temp->suivant;
        }
    }    

                
            
            
        
}
////////////////////////////////////////////////////
///////////////////////////////////////////////////
