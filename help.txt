=================
= Jump'n'Run ====
=================

== Compilation ==

Prérequis :
OpenGL (-libopengl32)
SFML 2.0 RC http://www.sfml-dev.org/download.php#2.0-rc
ou dernière version de SFML 2 (à mon avis plus simple grâce à CMake) : https://github.com/LaurentGomila/SFML

Récupération de la dernière version :
http://code.google.com/p/jump-n-run/
svn checkout http://jump-n-run.googlecode.com/svn/trunk/ jump-n-run-read-only

Utilisation du Makefile :
-Sous Unix (OS X ou Linux testés)
faire make run

-Sous Windows
Utiliser le projet Codeblocks.
Ou depuis le shell faire:
	make windirs #Que la première fois
	make run
Pour chaque exécution faire un make run

== Configuration ==

Les options sont modifiables dans le fichier Config.cfg à la raçine du dossier du jeu.

== Raccourcis de l'éditeur ==
Echap							Affiche le menu de l'éditeur
Clic Souris Gauche				Attrape le vertex le plus proche
								(Dans la mini-map : Déplacement rapide)
Clic Souris Gauche + 1			Création d'un nouveau Polygone Fixe
Clic Souris Gauche + 2			Création d'un nouveau Polygon Dynamique
Clic Souris Gauche + 3			Création d'un nouvel Elastic
Clic Souris Gauche + 4			Création d'un nouveau Rigid
Clic Souris Gauche + Ctrl		En restant appuyé, création d'un rectangle.
Clic Souris Droit				Affiche un menu d'édition rapide du niveau
								(Dans un Polygone : Menu d'édition du Polygon)
Clic Souris Droit + V			Menu d'édition du Vertex le plus proche
Clic Souris Droit + R			Menu d'édition de la liaison rigide la plus proche
Clic Souris Droit + E			Menu d'édition de la liaison élastique la plus proche
Clic Souric Millieu				Déplacement à la souris
Molette Souris					Zoom/Dézoom
Z/W								Déplacement vers le haut
Q/A								Déplacement vers la gauche
S								Déplacement vers le bas
D								Déplacement vers la droite
T								Test du niveau
J								Place le point d'apparition du joueur
K								Place le but du niveau
Espace							Réinitialise le niveau de Zoom
B								Créé une boîte
G								Attrape le vertex le plus proche
N								Affiche (console) le nombre de Vertices/Polygon
E								Attrape le vertex le plus proche à l'aide d'une liaison élastique
Suppr							Supprime le polygon survolé
Shift + Suppr					Supprime le vertex le plus proche
Ctrl + S						Sauvegarde du niveau
Ctrl + L						Chargement du niveau
L								Activer/Désactiver l'affichage Debug
F1								Activer/Désactiver l'affichage du Fond
F2								Activer/Désactiver l'affichage de la première couche de décors
F3								Activer/Désactiver l'affichage de la deuxième couche de décors
F4								Activer/Désactiver l'affichage du Premier Plan
F								Rend le Polygone le plus proche Fixe/Dynamique
C								Rend le Vertex le plus proche Fixe/Dynamique
Ctrl + C						Copie l'objet survolé
Ctrl + V						Colle l'objet précédement copié
,								Active/Désactive la pause


== Commandes du jeu ==
Echap							Affiche le menu (Pause)
Clic Souris Gauche				S'accrocher de la main gauche
Clic Souris Droit				S'accrocher de la main droite
Z/W								Saut
Q/A								Déplacement vers la gauche
D								Déplacement vers la droite
Shift							Rotation Gauche
Space							Rotation Droite
X								Se relever
