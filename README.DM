# Projet d'Analyseur Lexical et Syntaxique

Ce projet est un analyseur lexical et syntaxique écrit en langage C. Il vise à analyser un code source donné en entrée et à identifier les tokens ainsi que les erreurs syntaxiques possibles.

## Description

L'objectif principal de ce projet est de décomposer un code source en parties atomiques appelées "tokens" et d'effectuer une analyse syntaxique pour détecter les erreurs potentielles dans le code. Le code source est fourni dans un fichier texte et est analysé ligne par ligne.

## Fonctionnalités

- **Analyse lexicale :** Le code source est décomposé en tokens, en tenant compte des règles définies dans un dictionnaire spécifié.
- **Analyse syntaxique :** Le programme vérifie la structure syntaxique du code source, détecte les erreurs et affiche des messages appropriés en cas de problème.
- **Analyse sémantique :** Il effectue une analyse sémantique simple pour détecter les erreurs de déclaration de variables et d'utilisation incorrecte des types de données.

## Utilisation

1. Assurez-vous d'avoir un compilateur C installé sur votre système.
2. Clonez ce dépôt sur votre machine locale.
3. Compilez le programme à l'aide de la commande `gcc analyseur.c -o analyseur`.
4. Exécutez le programme en spécifiant le fichier source à analyser : `./analyseur code_source.txt`.

## Exemple

Supposons que nous ayons le fichier `code_source.txt` avec le contenu suivant :

```c
Debut
    Variable a : entier;
    Lire(a);
    Ecrire(a);
Fin
