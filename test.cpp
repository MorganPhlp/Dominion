//#include "Plateau.h"
#include "Jeu.h"
#include <ncurses.h>


int main(){

	/*
	Plateau plat = Plateau(2);
	Joueur j1 = Joueur("Morgan");
	Joueur j2 = Joueur("Fefe");
	std::vector<Joueur> liste = {j1,j2};
	Jeu jeu = Jeu(plat, liste);
	jeu.jouerPartie();
	return 0;
	*/
    	
    // Création et initialisation du plateau
    int nb_joueurs = 2; // Vous pouvez modifier le nombre de joueurs
    Plateau plateau = Plateau(nb_joueurs);

    // Appel de la méthode print() pour afficher le plateau
    plateau.print();

    return 0;

}

