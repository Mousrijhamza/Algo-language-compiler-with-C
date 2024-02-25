#include <stdio.h>
#include <stdlib.h>

// Définition de la structure d'un nœud dans l'ABR
struct Node {
    int key;
    struct Node* left;
    struct Node* right;
};

// Fonction pour créer un nouveau nœud
struct Node* newNode(int item) {
    struct Node* temp = (struct Node*)malloc(sizeof(struct Node));
    temp->key = item;
    temp->left = temp->right = NULL;
    return temp;
}

// Fonction récursive pour ajouter un nœud à l'ABR
struct Node* insert(struct Node* node, int key) {
    // Si l'arbre est vide, retourne un nouveau nœud
    if (node == NULL) return newNode(key);

    // Sinon, parcours l'arbre récursivement
    if (key < node->key) {
        // Si la clé est inférieure, insère dans le sous-arbre gauche
        node->left = insert(node->left, key);
    } else if (key > node->key) {
        // Si la clé est supérieure, insère dans le sous-arbre droit
        node->right = insert(node->right, key);
    }

    // Retourne le pointeur du nœud (inchangé)
    return node;
}

// Fonction utilitaire pour afficher l'ABR en ordre croissant et stocker les valeurs dans un tableau
void inorder(struct Node* root, int *arr, int *index) {
    if (root != NULL) {
        inorder(root->left, arr, index);
        
        arr[*index] = root->key;
        (*index)++;

        inorder(root->right, arr, index);
    }
}

// Fonction principale
int main() {
    struct Node* root = NULL;
    root = insert(root, 50);
    insert(root, 30);
    insert(root, 20);
    insert(root, 10);
    insert(root, 70);
    insert(root, 60);
    insert(root, 80);

    printf("Inorder traversal of the constructed BST is: ");

    int values[7];
    int index = 0; // Index pour suivre le prochain emplacement disponible dans le tableau

    // Appel de la fonction pour stocker les valeurs de l'ABR dans le tableau
    inorder(root, values, &index);
    for (int i = 0; i < 7; i++)
    {
        printf("%d ", values[i]);
    }
    
    return 0;
}
