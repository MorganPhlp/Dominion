# 🎮 Jeu Dominion en C++ (Interface Ncurses)

Ce projet est une implémentation complète et interactive en C++ du célèbre jeu de cartes de deck-building **Dominion**. Le jeu se déroule entièrement dans le terminal et tire parti de la bibliothèque **ncurses** pour proposer une interface textuelle colorée, structurée et réactive.

---

## 🃏 Concept du Jeu

**Dominion** est un jeu de cartes basé sur le mécanisme de **deck-building** (construction de pioche) :
1. Chaque joueur commence la partie avec un deck identique et très simple (composé de cartes de monnaie *Cuivre* et de cartes de victoire de base *Domaine*).
2. Au fil des tours, les joueurs accumulent de la monnaie pour acheter des cartes plus puissantes disponibles sur le plateau de jeu central (de meilleures cartes Trésor, des cartes d'Action aux effets variés, ou des cartes Victoire de grande valeur).
3. Le but ultime est d'optimiser son deck afin d'y incorporer le maximum de points de victoire (cartes *Province*, *Duché*, *Domaine*, *Jardins*) avant la fin de la partie.

---

## ✨ Fonctionnalités Clés

- **Modes de sélection des cartes Action :**
  - **Set aléatoire :** Sélection automatique de 10 cartes Action au hasard pour diversifier chaque partie.
  - **Set de base :** Utilisation des 10 premières cartes d'action classiques pour une partie équilibrée.
  - **Création personnalisée :** Interface interactive permettant aux joueurs de choisir manuellement les 10 cartes Action qui composeront le plateau.

- **Moteur de jeu complet avec trois phases par tour :**
  - **Phase Action :** Possibilité de jouer une ou plusieurs cartes Action de sa main (octroyant des bonus d'actions, d'achats, de pioches ou des pièces).
  - **Phase Achat :** Activation des cartes Trésor en main pour générer de la monnaie, puis achat de nouvelles cartes sur le Plateau.
  - **Phase Ajustement (Clean-up) :** Toutes les cartes jouées et les cartes restantes en main sont envoyées dans la défausse. Le joueur pioche ensuite une nouvelle main de 5 cartes. Si sa pioche est vide, sa défausse est automatiquement mélangée pour reformer le deck.

- **Interface Graphique en Terminal (`ncurses`) :**
  - Affichage stylisé des cartes avec bordures rectangulaires et descriptions bien alignées.
  - Système de couleurs dynamique pour différencier instantanément les types de cartes :
    - 🟡 **Jaune** : Cartes Trésor
    - 🟢 **Vert** : Cartes Victoire (et cartes spéciales comme Jardins / Malédiction)
    - 🔴 **Rouge** : Cartes Action classiques
    - 🔵 **Bleu** : Cartes Action de type *Réaction*
    - 🟣 **Magenta** : Cartes Action de type *Attaque*
  - Popups d'aide, de description détaillée des cartes (via la touche `TAB` et la saisie de l'index de la carte), et fenêtres flottantes de confirmation ou de messages d'erreur.

- **Interactions Avancées et Effets Spéciaux :**
  - Gestion des attaques (ex. *Milice*, *Sorcière*, *Voleur*, *Bandit*, *Espion*) qui ciblent les adversaires.
  - Prise en charge de la carte de réaction *Douve* qui, si elle est présente dans la main d'un joueur attaqué, lui permet de s'immuniser contre les effets néfastes.
  - Calcul dynamique des points de victoire complexes (p. ex. les cartes *Jardins* dont la valeur dépend de la taille totale du deck).

- **Simulation & Fin de partie :**
  - Fin de partie déclenchée si la pile de cartes *Province* est épuisée ou si au moins 3 piles de cartes du plateau sont vides.
  - Affichage soigné d'un tableau final des scores récapitulant les points de chaque joueur et déclarant le gagnant.

---

## 🏗️ Architecture Logicielle

Le projet est conçu de manière modulaire en C++ avec un fort accent sur l'orienté objet, le polymorphisme et le découpage logique des responsabilités :

### 1. Structure des Cartes (Polymorphisme)
- **`Carte` (Classe de base abstraite) :** Contient les attributs communs à toutes les cartes (nom, description, coût, type) et déclare l'interface virtuelle pure `play()` et `printCard()`.
- **`CarteTresor` :** Représente les ressources financières (*Cuivre*, *Argent*, *Or*) ajoutant des pièces lors de la phase d'achat.
- **`CarteVictoire` :** Représente les cartes rapportant des points de score (*Domaine*, *Duché*, *Province*, *Malédiction*). Ces cartes ne peuvent pas être jouées activement.
- **`CarteAction` :** Représente les cartes d'action. En plus de fournir des bonus cumulatifs (pioche, action, achat, pièces), elle gère les actions spécifiques de chaque carte via une méthode de routage interne `playDescription()`.

### 2. Moteur de Jeu et États
- **`Joueur` :** Gère le cycle de vie des cartes propres à chaque joueur (main, deck, défausse, rebut), le calcul de son score et les modifications de ses ressources temporaires de tour.
- **`Plateau` :** Gère les piles de cartes disponibles sur la table. Il prend en charge le chargement dynamique de la base de données des cartes depuis le fichier CSV.
- **`Jeu` :** Orchestre le déroulement global de la partie, alternant les tours des joueurs, gérant la vérification des conditions de fin de jeu et l'affichage des scores finaux.

---

## 📋 Cartes Implémentées dans le Projet

Toutes les caractéristiques des cartes du jeu sont chargées dynamiquement depuis le fichier **`ExcelActionCards.csv`** :

| Type | Nom de la Carte | Coût | Description / Effets |
| :--- | :--- | :---: | :--- |
| **Trésor** | Cuivre | 0 | +1 Pièce |
| **Trésor** | Argent | 3 | +2 Pièces |
| **Trésor** | Or | 6 | +3 Pièces |
| **Victoire** | Domaine | 2 | +1 Point de Victoire |
| **Victoire** | Duché | 5 | +3 Points de Victoire |
| **Victoire** | Province | 8 | +6 Points de Victoire |
| **Victoire** | Malédiction | 0 | -1 Point de Victoire |
| **Action** | Village | 3 | +1 Carte, +2 Actions |
| **Action** | Bûcheron | 3 | +2 Pièces, +1 Achat |
| **Action** | Forgeron | 4 | +3 Cartes |
| **Action** | Marché | 5 | +1 Carte, +1 Action, +1 Achat, +1 Pièce |
| **Action** | Atelier | 3 | Recevez une carte coûtant jusqu'à 4 pièces. |
| **Action** | Chapelle | 2 | Écartez (jetez au rebut) jusqu'à 4 cartes de votre main. |
| **Action** | Rénovation | 4 | Écartez une carte de votre main. Recevez une carte coûtant jusqu'à 2 pièces de plus. |
| **Action** | Mine | 5 | Écartez une carte Trésor de votre main et gagnez directement en main une carte Trésor coûtant jusqu'à 3 pièces de plus. |
| **Action** | Vassal | 3 | Défaussez la première carte de votre deck. Si c'est une Action, vous pouvez la jouer. |
| **Attaque** | Sorcière | 5 | +2 Cartes. Tous les autres joueurs reçoivent une carte Malédiction dans leur défausse. |
| **Attaque** | Milice | 4 | +2 Pièces. Les autres joueurs doivent défausser des cartes pour n'en garder que 3 en main. |
| **Attaque** | Voleur | 4 | Révèle les 2 premières cartes du deck des adversaires. Vous pouvez écarter et voler l'une de leurs cartes Trésor. |
| **Réaction** | Douve | 2 | +2 Cartes. Dévoiler cette carte depuis votre main annule les effets de toute carte Attaque jouée contre vous. |

---

## 🛠️ Compilation et Exécution

### Prérequis
- Un compilateur C++ supportant au minimum la norme **C++11** (comme `g++`).
- La bibliothèque **ncurses** installée sur votre système.
  - **Sur Debian/Ubuntu et dérivés :**
    ```bash
    sudo apt-get install libncurses5-dev libncursesw5-dev
    ```
  - **Sur macOS (via Homebrew) :**
    ```bash
    brew install ncurses
    ```
  - **Sur Windows :** Configurez la bibliothèque sous MSYS2, WSL ou MinGW.

### Commandes de Compilation (`Makefile`)

Le projet est livré avec un `Makefile` complet pour faciliter la gestion de l'application :

1. **Compiler le projet :**
   ```bash
   make
   ```
   ou
   ```bash
   make exec
   ```
   *Génère les fichiers objets (`.o`) et l'exécutable final nommé `exec`.*

2. **Lancer le jeu :**
   ```bash
   make run
   ```

3. **Lancer en mode Débogage (avec GDB) :**
   ```bash
   make debug
   ```

4. **Nettoyer les fichiers compilés temporaires :**
   ```bash
   make clean
   ```

---

## 📂 Structure du Projet

```
Dominion/
├── Carte.h / Carte.cpp               # Classe de base abstraite représentant une carte
├── CarteAction.h / CarteAction.cpp   # Cartes d'Action avec effets et bonus
├── CarteTresor.h / CarteTresor.cpp   # Cartes de Trésor (Cuivre, Argent, Or)
├── CarteVictoire.h / CarteVictoire.cpp # Cartes de Victoire (Points de score, Malédictions)
├── Joueur.h / Joueur.cpp             # État du joueur (deck, main, défausse) et actions associées
├── Plateau.h / Plateau.cpp           # Gestion des piles d'achat du plateau et rendu visuel
├── Jeu.h / Jeu.cpp                   # Déroulement du jeu, tours et phases de jeu
├── ExcelActionCards.csv              # Base de données CSV listant toutes les cartes et leurs attributs
├── test.cpp                          # Point d'entrée de l'application (fonction main)
├── Makefile                          # Script d'automatisation de la compilation
├── doctest.h                         # Framework léger de tests unitaires
└── Saves/                            # Dossier de sauvegarde (état en cours de développement)
```
