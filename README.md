# 🐍 C++ Console Snake Game

Un jeu de Snake classique et fluide, jouable directement dans le terminal Windows, entièrement développé en C++. 

Ce projet a été conçu de manière évolutive pour illustrer différentes méthodes de gestion des données et de la mémoire à travers trois versions distinctes.

## 🚀 Versions et Architecture

* **Version 1 (`snake_v1.cpp`) :** Utilisation de **tableaux statiques** (Arrays) de taille fixe pour stocker les coordonnées du corps du serpent.
* **Version 2 (`snake_v2.cpp`) :** Utilisation de **tableaux dynamiques** (`std::vector`) pour une structure de données plus flexible de la Standard Template Library (STL).
* **Version 3 (`snake_v3.cpp`) :** Implémentation via des **listes chaînées** (Pointeurs et `struct` personnalisée) pour une allocation dynamique et optimale de la mémoire à chaque fois que le serpent grandit.

## 🎮 Fonctionnalités et Niveaux
Chaque version propose un menu interactif au lancement avec 3 niveaux de difficulté :
1. **Facile :** Vitesse lente, les bordures de la carte permettent l'enroulement (traverser un mur fait réapparaître de l'autre côté).
2. **Moyen :** Vitesse normale, toucher une bordure entraîne un *Game Over*.
3. **Difficile :** Vitesse rapide, apparition d'obstacles fixes sur la carte et murs mortels.

**Sauvegarde :** Le jeu intègre un système de sauvegarde persistante du meilleur score dans un fichier local (`bestScore.txt`).

## 🛠️ Compilation et Exécution

Assurez-vous d'avoir un compilateur C++ (comme GCC/MinGW) installé.

**Depuis un terminal :**
```bash
# Exemple pour compiler et lancer la version 3
g++ -Wall -std=c++17 snake_v3.cpp -o snake_v3.exe
./snake_v3.exe
