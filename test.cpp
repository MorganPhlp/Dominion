//#include "Plateau.h"
#include "Jeu.h"
#include <ncurses.h>
#include <vector>


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
    
    //TEST PRINT PLATEAU	
    /*
    int nb_joueurs = 2;
    Plateau plateau = Plateau(nb_joueurs);
    plateau.print();
    */
    
    //TEST PRINTCARD()
    /*
    CarteAction carteAction("Atelier", "Permet de gagner une carte coûtant jusqu'à 4 pièces.", 
                            3, 0, 0, 0, 0, false, false);

    CarteAction carteActionAttaque("Milice", "Réduit la main de l'adversaire à 3 cartes.", 
                                   4, 0, 0, 0, 2, true, false);

    CarteAction carteActionReaction("Moat", "Bloque les attaques adverses et permet de piocher 2 cartes.", 
                                    2, 0, 0, 2, 0, false, true);

    // Créer des cartes Trésor
    CarteTresor carteTresorOr("Or", "", 6, 3);
    CarteTresor carteTresorCuivre("Cuivre", "Donne 1 pièce.", 0, 1);
    CarteTresor carteTresorArgent("Argent", "Donne 2 pièces.", 3, 2);

    // Créer des cartes Victoire
    CarteVictoire carteVictoireProvince("Province", "Donne 6 points de victoire.", 8, 6);
    CarteVictoire carteVictoireDuchy("Duché", "Donne 3 points de victoire.", 5, 3);
    CarteVictoire carteVictoireEstate("Domaine", "Donne 1 point de victoire.", 2, 1);

    // Ajouter ces cartes dans un vecteur pour démonstration
    std::vector<Carte*> cartes = {
        &carteAction, &carteActionAttaque, &carteActionReaction,
        &carteTresorOr, &carteTresorCuivre, &carteTresorArgent,
        &carteVictoireProvince, &carteVictoireDuchy, &carteVictoireEstate
    };

    // Afficher chaque carte avec printCard()
    for (const auto& carte : cartes) {
        carte->printCard();
    }
    */
    
    //TEST PRINTCARTD() DE CARTEACTION
    
    CarteAction carte1("Atelier", "Permet de gagner une carte coûtant jusqu'à 4 pièces.", 3, 0, 0, 0, 0, false, false);
    CarteAction carte2("Milice", "Réduit la main de l'adversaire à 3 cartes.", 4, 0, 0, 0, 2, true, false);
    CarteAction carte3("Moat", "Bloque les attaques adverses et permet de piocher 2 cartes.", 2, 0, 0, 2, 0, false, true);

    // Vecteur contenant les cartes
    std::vector<CarteAction> cartes = {carte1, carte2, carte3};

    // Affichage des cartes
    for (const auto& carte : cartes) {
        carte.printCard();
    }
    
    
    //TEST PRINTCARD() DE CARTETRESOR
    /*
    CarteTresor carte1("Cuivre", "Donne 1 pièce.", 0, 1);
    CarteTresor carte2("Argent", "Donne 2 pièces.", 3, 2);
    CarteTresor carte3("Or", "Donne 3 pièces.", 6, 3);

    // Vecteur contenant les cartes
    std::vector<CarteTresor> cartes = {carte1, carte2, carte3};

    // Affichage des cartes
    for (const auto& carte : cartes) {
        carte.printCard();
    }
    */
    
    
    //TEST PRINTCARD() DE CARTEVICTOIRE
    /*
    CarteVictoire carte1("Province", "Donne 6 points de victoire.", 8, 6);
    CarteVictoire carte2("Duché", "Donne 3 points de victoire.", 5, 3);
    CarteVictoire carte3("Domaine", "Donne 1 point de victoire.", 2, 1);

    // Vecteur contenant les cartes
    std::vector<CarteVictoire> cartes = {carte1, carte2, carte3};

    // Affichage des cartes
    for (const auto& carte : cartes) {
        carte.printCard();
    }
    */

    return 0;
}

