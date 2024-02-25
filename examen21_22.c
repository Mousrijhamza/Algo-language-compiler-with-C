#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct instruction {
    char *Mt;
    int ordr;
    struct instruction *suiv;
    struct instruction *prec;
};

typedef struct instruction* istr;

static istr liste = NULL;

istr InnsererMot(istr it, char *m) {
    istr new_node = (istr)malloc(sizeof(struct instruction));
    new_node->Mt = strdup(m);
    new_node->prec = new_node->suiv = NULL;

    if (it == NULL) {
        it = new_node;
        new_node->ordr = 1;
        return new_node;
    }

    istr temp = it;
    while (temp->suiv != NULL) {
        temp = temp->suiv;
    }

    temp->suiv = new_node;
    new_node->prec = temp;
    new_node->ordr = (temp->ordr) + 1;

    return it;
}

void Erreur(const char* message, int* ligne) {
    FILE *fichier = fopen("erreur_syntaxique.txt", "a+");  // "a" pour ajouter au fichier s'il existe, le créer sinon

    if (fichier != NULL) {
        if (ligne != NULL) {
            fprintf(fichier, "Erreur syntaxique : %s num : %d .\n", message, *ligne);
            printf("Erreur syntaxique : [ %s ] num : %d.\n", message, *ligne);
        
        } else {
            fprintf(fichier, "Erreur syntaxique : %s .\n", message);
            printf("Erreur syntaxique : [ %s ] .\n", message);
        
        }

        fclose(fichier);  // N'oubliez pas de fermer le fichier après avoir écrit
    } else {
        // Gestion de l'erreur d'ouverture du fichier
        printf("Erreur lors de l'ouverture du fichier erreur_syntaxique.txt.\n");
    }
}

void afficherChar(istr M) {
    

    if (M == NULL) {
        return;
    }
    printf("%s+ ", M->Mt);
    afficherChar(M->suiv);

}

void analyse_syntaxique() {
    FILE *erreur = fopen("erreur_syntaxique.txt", "a+"); // Open the error file
    int lenght = 0;
    char *tab[] = {"Ecrire", "(", "<<", " ", ">>", ",", "<<", " ", ">>", ",", " ", ")", ";"};

    if (liste == NULL) {
        printf("Vide");
        fclose(erreur); // Close the error file
        return;
    }

    istr temp = liste;
    int i = 0;

    for(i = 0; i < 13 ; i++) {
        printf("%s == %s ==         %d \n", temp->Mt, tab[i], i);
        

        
        if (!strcmp(temp->Mt, tab[i]) && i != 3 && i != 7 && i != 10) {
            
        } else {

            if (i == 3 || i == 7 || i == 10 ) {
            
                if (i == 3 ) {
                    if (strcmp(temp->Mt, "%E") != 0 && strcmp(temp->Mt, "%C") != 0 && strcmp(temp->Mt, "%R") != 0) {
                        
                        Erreur(strcat(strcat(temp->Mt, "~"), tab[i]), (temp != NULL) ? &(temp->ordr) : NULL);
                        break;
                    }

                }
                if (i == 7);
                
                if ( i == 10 ) {
                    lenght = strlen(temp->Mt) ;
                    if (lenght != 1)
                    {
                        Erreur(strcat(strcat(temp->Mt, "~"), tab[i]), &(temp->ordr));
                        break;
                    }
                
                }

            } else {
            
                Erreur(strcat(strcat(temp->Mt, " missed ~ "), tab[i]), &(temp->ordr));
                break;
                // i++;
            }
            
        }
        
        if (temp->suiv == NULL)
        {
            i++;
            Erreur(tab[i], &i);
            break;
        }
        temp = temp->suiv;
        
        
    }

    fclose(erreur); // Close the error file
}


int main() {
    FILE *fic = fopen("mousrij.txt", "r");
    char letter;
    char tab[256];
    int i = 0;

    while ((letter = fgetc(fic)) != EOF) {
        if (letter == '\n' || letter == '\t' || letter == ' ') {
            tab[i] = '\0';
            if (i > 0) {
                liste = InnsererMot(liste, tab);
                i = 0;
            }
        } else {
            tab[i++] = letter;
        }
    }
    fclose(fic);

    // Affichage de la liste
    afficherChar(liste);
    printf("\n");

    // Analyse syntaxique
    analyse_syntaxique();

    return 0;
}