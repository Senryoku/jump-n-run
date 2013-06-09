# Jump'n'Run
____________

## Présentation
C'est un jeu de courses avec un bonhomme. Un éditeur de niveau permet de créer de très larges mondes avec plein d'obstacles. 
La physique avancée permet de gérer des polygones fixes ou pas, des  points et des contraintes rigides et élastiques (ressorts). On peu aussi importer des images personnalisées pour créer n'importe quel objet et le mettre dans le niveau. 

## Captures d'écran
![img1](http://i.imgur.com/yZ9MJ.png)

Une chaîne de boîtes liées par de ressorts

![img2](http://i.imgur.com/wtdkd.png)

Quelques objets avec des images personalisées

![img3](http://i.imgur.com/AmhKz.png)

Le joueur en affichage Debug et un Menu de test

![img4](http://i.imgur.com/jxxQE.png)
![img5](http://i.imgur.com/8tuFG.png)

Le joueur en affichage normal

## Vidéo d'un niveau
[Regarder sur Youtube](http://www.youtube.com/watch?v=qlRJhccilRk)


## Compilation
Pour pouvoir compiler il est nécessaire d'avoir SFML 2 et OpenGL
Vous pouvez utiliser soit la version RC de SFML 2.0 soit la version GitHub. On encourage d'utiliser cette dernière qui est d'ailleurs très facile à installer grâce à CMake.

https://github.com/LaurentGomila/SFML

Pour compiler SFML 2 voici une aide avec les dépendences : http://sfmlcoder.wordpress.com/2011/08/16/building-sfml-2-0-with-make-for-gcc/

http://www.sfml-dev.org/download.php#2.0-rc

Le binaire est créé dans bin/ et doit être exécuté depuis .

* Unix

_Uniquement testé sous OS X 10.6, Linux Ubuntu 11 et Kubuntu 12_
Récupérer le code et se placer dans le répertoire de travail

    svn checkout http://jump-n-run.googlecode.com/svn/trunk/ jump-n-run-read-only
    cd jump-n-run-read-only

Lancer le jeu

    make runGame
Lancer l'éditeur

    make runEditor

* Windows

Utiliser de préférence le projet CodeBlocks JumpnRun.cbp
Sinon faire juste la première fois:

    make windirs
Et pour lancer utiliser les mêmes commandes que pour Unix

## Contrôles
Regarder dans help.txt

## Crédits
Développé par *Yoann Maret-Verdant* et *Eduardo San Martin Morote*


