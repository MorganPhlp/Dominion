//#include "Plateau.h"
#include "Jeu.h"
#include <ncurses.h>
#include <vector>
#include <cstdlib>
#include <ctime>


int main(){


  	
  Plateau plat = Plateau(2);
  Joueur j1 = Joueur("Morgan");
  Joueur j2 = Joueur("Fefe");
  std::vector<Joueur> liste = {j1,j2};
  Jeu jeu = Jeu(plat, liste);
  jeu.jouerPartie();
  	
    
    //TEST PRINT PLATEAU	
    /*
    int nb_joueurs = 2;
    Plateau plateau = Plateau(nb_joueurs);
    Joueur j1 = Joueur("Caca");
    plateau.print(j1.getPseudo(), j1.getCoins(), j1.getNbBuys(), j1.getNbWinPoints());
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
    /*
    CarteAction carte1("Atelier", "Permet de gagner une carte coûtant jusqu'à 4 pièces.", 3, 0, 0, 0, 0, false, false);
    CarteAction carte2("Milice", "Réduit la main de l'adversaire à 3 cartes.", 4, 0, 0, 0, 2, true, false);
    CarteAction carte3("Moat", "Bloque les attaques adverses et permet de piocher 2 cartes.", 2, 0, 0, 2, 0, false, true);

    // Vecteur contenant les cartes
    std::vector<CarteAction> cartes = {carte1, carte2, carte3};

    // Affichage des cartes
    for (const auto& carte : cartes) {
        carte.printCard();
    }
    */
    
    
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
    
    
    /*
    //TEST PRINTCARD() DE CARTEVICTOIRE
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
    
    //TEST PRINTCARDS()
    /*
    // Initialisation du générateur aléatoire
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // Création de cartes types
    CarteAction* carteAction1 = new CarteAction("Atelier", "Permet de gagner une carte coutant jusqu'à 4 pieces.", 3, 0, 0, 0, 0, false, false);
    CarteAction* carteAction2 = new CarteAction("Festin", "Gagnez une carte coûtant jusqu'à 5 pièces.", 4, 0, 0, 0, 0, false, false);
    CarteVictoire* carteVictoire1 = new CarteVictoire("Province", "Donne 6 points de victoire.", 8, 6);
    CarteVictoire* carteVictoire2 = new CarteVictoire("Domaine", "Donne 1 point de victoire.", 2, 1);
    CarteTresor* carteTresor1 = new CarteTresor("Or", "Une ressource précieuse.", 6, 3);
    CarteTresor* carteTresor2 = new CarteTresor("Cuivre", "Une ressource commune.", 0, 1);

    // Création d'un joueur
    Joueur joueur("Alice");

    // Ajout de 5 cartes fixes à la main
    joueur.getHand().push_back(carteAction1);
    joueur.getHand().push_back(carteAction2);
    joueur.getHand().push_back(carteVictoire1);
    joueur.getHand().push_back(carteTresor1);
    joueur.getHand().push_back(carteTresor2);

    // Ajout de cartes aléatoires dans le deck
    int nbCartesDeck = std::rand() % 10; // Nombre de cartes aléatoire (0 à 9)
    for (int i = 0; i < nbCartesDeck; ++i) {
        int type = std::rand() % 3; // Type de carte : 0 = Action, 1 = Victoire, 2 = Trésor
        switch (type) {
            case 0: joueur.getDeck().push_back(carteAction1); break;
            case 1: joueur.getDeck().push_back(carteVictoire2); break;
            case 2: joueur.getDeck().push_back(carteTresor2); break;
        }
    }

    // Ajout de cartes aléatoires dans la défausse
    int nbCartesDefausse = std::rand() % 10; // Nombre de cartes aléatoire (0 à 9)
    for (int i = 0; i < nbCartesDefausse; ++i) {
        int type = std::rand() % 3; // Type de carte : 0 = Action, 1 = Victoire, 2 = Trésor
        switch (type) {
            case 0: joueur.getDefausse().push_back(carteAction2); break;
            case 1: joueur.getDefausse().push_back(carteVictoire1); break;
            case 2: joueur.getDefausse().push_back(carteTresor1); break;
        }
    }

    // Affichage de la main, du deck et de la défausse
    joueur.printHand();   // Affiche la main
    joueur.printDeck();   // Affiche le deck
    joueur.printDefausse(); // Affiche la défausse

    // Libération des cartes
    delete carteAction1;
    delete carteAction2;
    delete carteVictoire1;
    delete carteVictoire2;
    delete carteTresor1;
    delete carteTresor2;
    
    */
    

    return 0;
}

