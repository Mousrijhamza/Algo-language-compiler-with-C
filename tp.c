#incldue<stdio.h>
#include<stdlib.h>

struct cellule
{
    int element;
    struct cellule * suivant;
};

typedef struct cellule* Cellule;

struct file
{
    int longueur;
    Cellule tete;
    Cellule Queu;
};

typedef struct file* File;

int fileEstVide(File liste)
{
    if(liste==NULL)
        return 1;
    else
        return 0;
}

File fileEstVide()
{
  File nouvellefile;
  nouvellefile = (File) malloc(sizeof(struct file));
  nouvellefile->longueur=0;
  nouvellefile->tete=NULL;
  nouvellefile->Queu=NULL;
  return nouvellefile;
}

int longueur(File unefile)
{
   return unefile.longueur; 
}


void enfiler(File* F, int e) {
    Cellule nouvelleCellule = (Cellule)malloc(sizeof(struct cellule));
    nouvelleCellule->element = e;
    nouvelleCellule->suivant = NULL;

    if (fileEstVide(F) == 1) {
        F->Tete = nouvelleCellule;
        F->Queue = nouvelleCellule;
        F->longueur++;
    } else {
        F->Queue->suivant = nouvelleCellule;
        F->Queue = nouvelleCellule;
        F->longueur++;
    }
}