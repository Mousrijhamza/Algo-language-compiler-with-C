#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure pour représenter un nœud de l'arbre
struct Node {
    char* data;
    struct Node* left;
    struct Node* right;
};

// Fonction pour créer un nouveau nœud
struct Node* newNode(char* data) {
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    node->data = strdup(data); // Dupliquer la chaîne pour éviter les problèmes de mémoire
    node->left = NULL;
    node->right = NULL;
    return node;
}

// Fonction pour comparer un nœud avec une chaîne
int compareNode(struct Node* node, char* str) {
    return strcmp(node->data, str) == 0;
}

// Fonction pour comparer un arbre avec un tableau
int compareTreeWithArray(struct Node* root, char* Tab[], int size) {
    if (root == NULL || size == 0)
        return 0;

    // File pour effectuer un parcours en largeur de l'arbre
    struct Node** queue = (struct Node**)malloc(size * sizeof(struct Node*));
    int front = 0, rear = 0;
    queue[rear++] = root;

    int i = 0; // Indice pour parcourir le tableau

    while (front != rear && i < size) {
        struct Node* current = queue[front++];

        // Comparer le nœud courant avec l'élément du tableau correspondant
        if (!compareNode(current, Tab[i++])) {
            free(queue); // Libérer la mémoire allouée à la file
            return 0;
        }

        // Enfiler les enfants du nœud courant
        if (current->left)
            queue[rear++] = current->left;
        if (current->right)
            queue[rear++] = current->right;
    }

    free(queue); // Libérer la mémoire allouée à la file

    // Vérifier si tous les éléments du tableau ont été comparés
    return i == size;
}

int main() {
    char* Tab[] = {"Si", "une condition", "Alors", "lire", "Sinon", "Ecrire", "Finsi"};

    // Construire l'arbre
    struct Node* root = NULL;
    root = newNode("Si");
    root->left = newNode("une condition");
    root->right = newNode("Alors");
    root->right->left = newNode("lire");
    root->right->right = newNode("Sinon");
    root->right->right->left = newNode("Ecrire");
    root->right->right->right = newNode("Finsi");

    // Comparer l'arbre avec le tableau
    if (compareTreeWithArray(root, Tab, sizeof(Tab) / sizeof(Tab[0])))
        printf("L'arbre correspond au tableau.\n");
    else
        printf("L'arbre ne correspond pas au tableau.\n");

    return 0;
}
