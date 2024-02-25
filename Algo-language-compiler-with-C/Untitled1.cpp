#include <stdio.h>
#include <stdlib.h>

// Structure pour repr�senter un n�ud dans la liste d'adjacence
struct Node {
    int vertex;
    struct Node* next;
};

// Structure pour repr�senter un graphe
struct Graph {
    int numVertices;
    struct Node** adjLists;
};

// Fonction pour cr�er un nouveau n�ud
struct Node* createNode(int v) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->vertex = v;
    newNode->next = NULL;
    return newNode;
}

// Fonction pour cr�er un graphe avec un nombre donn� de sommets
struct Graph* createGraph(int vertices) {
    struct Graph* graph = (struct Graph*)malloc(sizeof(struct Graph));
    graph->numVertices = vertices;
    graph->adjLists = (struct Node**)malloc(vertices * sizeof(struct Node*));
    for (int i = 0; i < vertices; i++) {
        graph->adjLists[i] = NULL;
    }
    return graph;
}

// Fonction pour ajouter une ar�te au graphe
void addEdge(struct Graph* graph, int src, int dest) {
    // Ajouter une ar�te de src � dest
    struct Node* newNode = createNode(dest);
    newNode->next = graph->adjLists[src];
    graph->adjLists[src] = newNode;

    // Comme le graphe est non-dirig�, ajouter �galement une ar�te de dest � src
    newNode = createNode(src);
    newNode->next = graph->adjLists[dest];
    graph->adjLists[dest] = newNode;
}

// Fonction pour afficher le graphe
void printGraph(struct Graph* graph) {
    for (int v = 0; v < graph->numVertices; v++) {
        struct Node* temp = graph->adjLists[v];
        printf("Adjacency list of vertex %d\n", v);
        while (temp) {
            printf("%d -> ", temp->vertex);
            temp = temp->next;
        }
        printf("NULL\n");
    }
}

int main() {
    int V = 5; // Nombre de sommets du graphe
    struct Graph* graph = createGraph(V);
    addEdge(graph, 0, 1);
    addEdge(graph, 0, 4);
    addEdge(graph, 1, 2);
    addEdge(graph, 1, 3);
    addEdge(graph, 1, 4);
    addEdge(graph, 2, 3);
    addEdge(graph, 3, 4);
    printGraph(graph);
    return 0;
}

