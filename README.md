# TouLaMeche

## Description

Ce projet consiste à développer un logiciel capable de prédire le mot suivant dans un texte saisi par un utilisateur.  
La prédiction repose sur une base de données qui enregistre les séquences de mots rencontrées précédemment et leur fréquence d'apparition. Cette base de données s'enrichit dynamiquement à mesure que de nouveaux textes sont saisis.

### Contexte et méthode

Le logiciel utilise des **N-grammes**, c'est-à-dire des séquences de `N` mots consécutifs, pour effectuer les prédictions.  
L'idée repose sur l'observation que certains mots appellent naturellement d'autres mots. Par exemple :
- "le petit chaperon" → "rouge"
- "les salutations" → "distinguées"

### Fonctionnalités principales

Le logiciel propose trois modes de fonctionnement, sélectionnables via des options d'appel :

1. **Mode IHM (interface graphique)** :
   - Un éditeur multi-lignes permet de saisir le texte.
   - Le logiciel suggère des mots basés sur le N-gramme précédant la position du curseur.

2. **Mode terminal (`-t`)** :
   - Le texte est saisi dans le terminal.
   - Après chaque mot saisi (suivi d'un espace ou d'un retour chariot), une pression sur la touche `Tab` affiche la suggestion du mot suivant.
   - Si aucun N-gramme correspondant n'est trouvé, aucune suggestion n'est faite.
   - Le texte saisi est enregistré dans un fichier spécifié en paramètre.

3. **Mode construction de la base de données (`-b`)** :
   - Le logiciel analyse un texte (redirigé depuis l'entrée standard) pour extraire des N-grammes et leurs mots associés.
   - Ces informations enrichissent la base de données de prédiction.

### Base de données

- Par défaut, la base de données est stockée dans un fichier nommé **`predDB`**.
- L'option `-d` permet de spécifier un autre fichier de base de données.

## Installation

1. Clonez le dépôt :
   ```bash
   git clone https://github.com/ouiUnderscore/TouLaMeche.git
   cd TouLaMeche
   ```
2. Compilez le projet (si applicable) :
   ```bash
   make
   ```

## Utilisation

Exemples d'appel :

- Mode IHM (par défaut) :
  ```bash
  ./predictor
  ```
- Mode terminal :
  ```bash
  ./predictor -t fichier_sortie.txt
  ```
- Mode construction de la base de données :
  ```bash
  ./predictor -b < fichier_entree.txt
  ```

## Auteur

Sujet par Stéphane Rubini.

Développé par Emilie Zammit.