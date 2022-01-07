`Antoine DUMOULIN`

# Usage du programme
![rendu](rendu.png)

## Test de remplissage

1. ./plot 600 600
2. plusieurs cliques gauches pour ajouter des sommets
3. touche `c` pour fermer le polygone
4. touche `f` pour remplir le polygone

## Test mode vertex

1. ./plot 600 600
2. plusieurs cliques gauches pour ajouter des sommets
3. touche `v` pour passer en mode vertex
4. clic gauche pour selectionner un sommet
   ou clavier avec page suivante/précedante
5. fleches directionnelles pour déplacer les points
6. `suppr` pour supprimer le point sélectionné
7. touche `i` pour revenir au mode insert

## Test mode edge

1. ./plot 600 600
2. plusieurs cliques gauches pour ajouter des sommets
3. touche `e` pour passer en mode edge
4. clic gauche pour selectionner une arrête
   ou clavier avec page suivante/précedante
5. clic droit pour diviser l'arrête en deux
6. touche `i` pour revenir au mode insert


# Techniques utilisées

J'ai fait le code avec une liste doublement chainée qui est quand même plus pertinente
qu'une liste simplement chainée... 

La sélection d'une vertex avec la souris se fait avec la plus petite distance euclidienne
entre les sommets du polygone.

Pour la sélection de l'arrête je réutilise la sélection du plus proche vertex, puis 
je détermine, avec un simple calcul d'angle, l'arrête adjacente à ce vertex la plus proche.


# Cas particuliers pour le remplissage

J'ai eu du mal à gérer correctement les cas où plusieurs lignes du polygones se "collent" en formant des composantes 4-connexes horizontales entre elles.
J'arrive à determiner si un point appartenant au polygone est tangent horizontalement,
mais je n'ai pas réussi pour les points des lignes :

![situation initiale](ensemble_4-connexe.png) | ![remplissage](remplissage_errone.png)
