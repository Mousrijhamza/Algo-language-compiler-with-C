#include <stdio.h>
#include <stdlib.h>

// Structure d'un nœud de l'arbre binaire
typedef struct Node {
    char *data;
    struct Node *left;
    struct Node *right;
} Node;

// Fonction pour créer un nouveau nœud
Node* newNode(char *data) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->data = data;
    node->left = NULL;
    node->right = NULL;
    return node;
    
}

// Fonction pour parcourir un arbre binaire de manière préfixée (non récursive)
void preorderTraversal(Node* root) {
    if (root == NULL)
        return;

    Node* stack[100]; // pile pour stocker les nœuds
    int top = -1;

    stack[++top] = root; // Empiler le nœud racine

    while (top >= 0) {
        // Dépiler un nœud et le visiter
        Node* current = stack[top--];
        printf("%s ", current->data);

        // Empiler le sous-arbre droit en premier pour qu'il soit visité en dernier
        if (current->right != NULL)
            stack[++top] = current->right;

        // Empiler le sous-arbre gauche pour qu'il soit visité en premier
        if (current->left != NULL)
            stack[++top] = current->left;
    }
}

int main() {
    // Création de l'arbre binaire
    Node* root = newNode("Ecrire");
    root->left = newNode("(");
    root->left->left = newNode("<<");
    root->left->right = newNode("message");
    root->right = newNode(">>");
    root->right->left = newNode(")");
    root->right->right = newNode(";");

    // Parcours de l'arbre binaire de manière préfixée
    printf("Parcours préfixé de l'arbre binaire:\n");
    preorderTraversal(root);
    printf("\n");

    return 0;
}
