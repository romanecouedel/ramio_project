# ramio_project
Un jeu de plateforme inspiré de Mario, développé en C++ avec SFML. Il est possible d'effectuer seul ou à deux un parcours semé d’obstacles et tenter d’atteindre la fin du niveau sans tomber dans les pièges.

## Table des matières
1. [Introduction](#-introduction)  
2. [Installation](#-installation)  
3. [Utilisation](#-utilisation)  
4. [Structure du projet](#-structure-du-projet)  
5. [Contributeurs](#-contributeurs)  
6. [Licence](#-licence)  

## Introduction 
Ramio est un jeu de plateforme en C++ avec SFML, jouable en solo ou en coopération (2 joueurs sur le même écranmario et luigi). On peut également jouer avec un IA symbolique simplifiée. Les objectifs sont simples :

- Évitez les obstacles (trous, ennemis)
- Atteignez la ligne d’arrivée en un seul morceau

Le jeu est conçu pour créer facilement des niveaux et y intégrer facilement une IA plus poussée.

## Installation
### Prérequis
GNU/Linux

G++ (>= 11)

CMake

SFML 2.5+

### Commandes
```sh
git clone https://github.com/romanecouedel/ramio_project
cd ramio_project
rm -rf build # si besoin 
mkdir build && cd build  
cmake ..  
make  
./mario

```

## Utilisation
### Naviguer dans le menu :
Il y a plusieurs couches de menu. Le choix sélectionné apparait en rouge, il faut appuyer sur la 'touche entrer' pour le confirmer.

- **Première couche** : on peut choisir entre jouer et quitter le jeu. Si Quitter est confirmé alors la fenètre se ferme. Lorsque Jouer est confirmé, on arrive à la deuxième couche du menu.
- **Deuxième couche** : il est possible de choisir "1 joueur" ou "2 joueurs" ou retourner à la première couche du menu. Si on décide de jouer seul, alors on accède à la couche de level/map du menu. Si on veut jouer en coopération alors on accède à la couche multijoueur du menu. 
- **Couche multijoueur** : 2 options de jeu en coopération sont possibles, si l'option "Avec l'ordinateur" est confirmée, on accède ensuite à la couche level. Si cette option est choisie alors, pendant la partie, l'ia simplifiée jouera luigi pendant que le joueur principal jouera mario. Si l'option "En local" est confirmée, on accède également à la couche level et une personne joueura luigi, une autre mario pendant la partie. On peut égalament revenir à la deuxème couche du menu en confirmant Retour
- **Couche Level** : Il est possible de choisir parmis 3 levels classés du plus simple au plus difficile. En confirmant un des levels, la partie démarre. En confirmant Retour, on retourne à la couche du menu sur laquelle on était juste avant celle-ci.

### Jouer en solo ou avec l'IA

Une fois qu'une partie en solo ou avec l'IA est lancée, il y a 4 touches qui permettent à mario de faire des actions:
- **Flèche Gauche** : Déplacement  vers la gauche
- **Flèche Droite** : Déplacement vers la droite
- **Flèche Bas** : permet de se téléporter à travers un tuyau lorsque mario est au dessus de l'entré de celui-ci
- **Flèche Haut** : saut

La partie se termine lorsque mario touche le drapeau ou que mario meurt 3 fois. On accède ainsi à l'écran de fin de jeu.

### Jouer à 2

Les touches qui actionnent mario sont inchangées, celles qui actionnent luigi sont respectivement **Q**, **D**,**S**, **z**.
La partie se termine lorsque mario (ou luigi) touche le drapeau ou que mario (ou luigi) meurt 3 fois. On accède ainsi à l'écran de fin de jeu.

### Écran de fin de jeu

Une fois la fin de jeu activé, on peut voir si on a gagné ou perdu, le temps qui a été nécessaire pour faire le parcours. On peut voir également combien de pièces ont été récoltées et combien de fois luigi ou mario est mort. En appuyant sur la touche 'entrer' du clavier, on retourne à la première couche du menu.

## Structure du projet



## Contributeurs

## Licence 
(si besoin)

