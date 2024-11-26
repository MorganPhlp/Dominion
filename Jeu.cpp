#include "Jeu.h"
#include <random>
#include <chrono>
#include <functional>
#include <iomanip>
#include <ncurses.h>

std::vector<size_t> m_joueursImmunises = {};
std::unordered_map<size_t, std::vector<Carte*>> mapCartesDevoilees;

Jeu::Jeu(Plateau plateau, std::vector<Joueur> listeJoueur) : m_nb_joueurs(listeJoueur.size()), m_plateau(plateau), m_listeJoueur(listeJoueur) {
	for(size_t i = 0; i < m_listeJoueur.size(); i++){
		m_listeJoueur.at(i).initDeck(m_plateau);
		m_listeJoueur.at(i).makeHand();
	}
	initJoueurActif(m_listeJoueur);
	m_nbMarchand = 0;
}

Jeu::Jeu(Plateau plateau, std::vector<Joueur> listeJoueur, bool finPartie) : m_nb_joueurs(listeJoueur.size()), m_plateau(plateau), m_listeJoueur(listeJoueur) {
        (void) finPartie;
	for(size_t i = 0; i < m_listeJoueur.size(); i++){
		m_listeJoueur.at(i).initDeckFin(m_plateau);
		m_listeJoueur.at(i).makeHand();
	}
	initJoueurActif(m_listeJoueur);
	m_nbMarchand = 0;
	m_plateau.modifierPlateauFin();
}

Jeu::~Jeu() {}

void Jeu::initJoueurActif(std::vector<Joueur>& liste){
	std::default_random_engine re(std::chrono::system_clock::now().time_since_epoch().count());
	int taille = liste.size();
	std::uniform_int_distribution<int> distrib{0, taille-1};
	auto rd = bind(distrib, re);
	int alea = rd();
	m_joueurActif = &liste.at(alea);
	std::cout << "Le joueur actif initial est : " << m_joueurActif->getPseudo() << std::endl;
}

Plateau Jeu::getPlateau(){
	return m_plateau;
}


void Jeu::tourJoueur(Joueur* j){
  j->initNouveauTour();
  size_t index;
  j->calculerScore();
  
  //Phase Action
  //std::cout << "======== Phase Action ========" << std::endl;
  /*
  std::string choix;
  
  while(j->getNbActions() > 0 && j->getNbCarteActionHand().first != 0){
    if(j->queJardins()) break;
    std::cout << "Nombre d'actions restantes : " << j->getNbActions() << std::endl;
    j->printHand();
    std::cout << "Entrez le numéro de la carte que vous souhaitez jouer (ou ACHAT pour passer à la phase Achat) : " << std::endl;
    std::cin >> choix;
    if(choix == "ACHAT") break;
    try{
      index = std::stoi(choix);
      if(index < j->getHand().size()){
        Carte* carte = j->getHand().at(index);
        if(carte->getType() != TypeCarte::Action || carte->getName() == "Jardins"){
          std::cout << "Vous ne pouvez pas jouer cette carte" << std::endl;
        }
        else{
          carte->play(*j, m_plateau, index, *this);
          j->addActions(-1);
        }		
      }
      else{
        std::cout << "Index invalide" << std::endl;
      }
    }
    catch (...){
      std::cerr << "Erreur lors de la lecture de l'index" << std::endl;
    }
    int nb = j->getDraws();
    if(nb != 0){
      for(int i = 0; i < nb; i++) j->piocher();
    }
    j->resetDraws();
  }
  */
  
  //Phase Achat
  //std::cout << "======== Phase Achat ========" << std::endl;
  /*
  while(j->getNbCarteTresorHand().first > 0){
    j->printHand();
    std::cout << "Entrez l'index d'une carte Trésor à jouer (ou PASSER pour passer) : " << std::endl;
    std::cin >> choix;
    if(choix == "PASSER") break;
    try{
      index = std::stoi(choix);
      if(index < j->getHand().size()){
        Carte* carte = j->getHand().at(index);
        if(carte->getType() == TypeCarte::Tresor){
          if(carte->getName() == "Argent" && m_nbMarchand != 0){
            carte->play(*j, m_plateau, index, *this);
            j->addCoins(m_nbMarchand);
            m_nbMarchand = 0;
          }
          else{
            carte->play(*j, m_plateau, index, *this);
          }
        }
        else{
          std::cout << "Vous ne pouvez pas jouer cette carte" << std::endl;
        }
      }
      else{
        std::cout << "Index Invalide" << std::endl;
      }
    }
    catch (...){
      std::cerr << "Erreur lors de la lecture de l'index" << std::endl;
    }
  }
  */
  
  /*
  while(j->getNbBuys() > 0){
    int coins = j->getCoins();
    int buys = j->getNbBuys();
    int score = j->calculerPoints(); // Utilisation du score actuel
    std::string pseudo = j->getPseudo();

    m_plateau.print(pseudo, coins, buys, score); // Affichage du plateau avec les infos du joueur
    
    std::cout << "Choisissez l'index de la pile pour acheter une carte ou PASSER pour passer : " << std::endl;
    std::cin >> choix;
    if (choix == "PASSER") break;
    try{
      index = std::stoi(choix);
      if (index < m_plateau.getMaxIndex()){
        if(j->getCoins() >= m_plateau.chercherCoutParIndex(index)){
          try {
            j->buyCard(index, m_plateau);
            std::cout << "Achat réussi !" << std::endl;
          } catch (const std::exception& e) {
            std::cerr << "Erreur : " << e.what() << std::endl;
          }
        }
        else{
          std::cout << "Vous n'avez pas assez de pièces" << std::endl;
        }
      }
      else{
        std::cout << "Index invalide" << std::endl;
      }
    }
    catch (const std::exception& e){
      std::cerr << "Erreur lors de la lecture de l'index \n Erreur " << e.what() << std::endl;
    }
  }
	
  //phase ajustement
  std::cout << "======== Phase Ajustement ========" << std::endl;
  j->defausser(); //Défausser toutes les cartes jouées et les cartes en main

  //Reformer le deck si nécessaire et piocher 5 cartes pour une nouvelle main
  if (j->getDeck().size() < 5) {
    j->assembleDeckDefausse();
  }
  j->makeHand();  // Piocher 5 nouvelles cartes

  std::cout << "Votre nouvelle main pour le prochain tour est prête !" << std::endl;
  j->printHand();
  */
  
  
  while (j->getNbActions() > 0 && j->getNbCarteActionHand().first != 0) {
    if (j->queJardins()) break; // Sortie si toutes les cartes sont des "Jardins"

/*
  while (j->getNbBuys() > 0) {
        int coins = j->getCoins();
        int buys = j->getNbBuys();
        int score = j->getNbWinPoints(); // Utilisation du score actuel
        std::string pseudo = j->getPseudo();
        */

    j->printHand(); // Affiche la main actuelle

    // Création d'une fenêtre popup pour demander l'action
    int popup_height = 5;
    int popup_width = 70;
    int popup_y = LINES / 2; // Sous l'affichage de la main
    int popup_x = (COLS - popup_width) / 2;
    WINDOW* popup = newwin(popup_height, popup_width, popup_y, popup_x);
    box(popup, 0, 0);

    // Affiche une instruction dans la fenêtre
    mvwprintw(popup, 1, 2, "Entrez l'index d'une carte Action à jouer (ou -1 pour passer) :");
    wrefresh(popup);

    // Lecture de l'entrée utilisateur
    std::string choix;
    echo(); // Activer l'affichage des entrées utilisateur
    char input[10];
    wgetnstr(popup, input, 10);
    noecho(); // Désactiver l'affichage des entrées utilisateur
    choix = input;
    delwin(popup); // Supprime la fenêtre popup

    // Gestion de l'entrée utilisateur
    if (choix == "-1") break; // Quitte la boucle si l'utilisateur passe
    try {
        size_t index = std::stoi(choix);
        if (index < j->getHand().size()) {
            Carte* carte = j->getHand().at(index);
            if (carte->getType() == TypeCarte::Action) {
                carte->play(*j, m_plateau, index, *this);
                j->addActions(-1); // Réduit le nombre d'actions
            } else {
                // Affiche une erreur si ce n'est pas une carte Action
                WINDOW* error = newwin(popup_height, popup_width, popup_y, popup_x);
                box(error, 0, 0);
                mvwprintw(error, 2, 2, "Erreur : Ce n'est pas une carte Action !");
                wrefresh(error);
                getch(); // Attendre que l'utilisateur appuie sur une touche
                delwin(error);
            }
        } else {
            // Affiche une erreur si l'index est invalide
            WINDOW* error = newwin(popup_height, popup_width, popup_y, popup_x);
            box(error, 0, 0);
            mvwprintw(error, 2, 2, "Erreur : Index invalide !");
            wrefresh(error);
            getch(); // Attendre que l'utilisateur appuie sur une touche
            delwin(error);
        }
    } catch (...) {
        // Affiche une erreur si l'entrée est incorrecte
        WINDOW* error = newwin(popup_height, popup_width, popup_y, popup_x);
        box(error, 0, 0);
        mvwprintw(error, 2, 2, "Erreur : Entrée non valide !");
        wrefresh(error);
        getch(); // Attendre que l'utilisateur appuie sur une touche
        delwin(error);
    }

    // Pioche des cartes si des cartes donnent cet effet
    int nb = j->getDraws();
    if (nb != 0) {
        for (int i = 0; i < nb; i++) j->piocher();
    }
    j->resetDraws();
}
  
  while (j->getNbCarteTresorHand().first > 0) {
    j->printHand(); // Affiche la main actuelle

    // Création d'une fenêtre popup sous l'affichage de la main
    int popup_height = 5;
    int popup_width = 70;
    int popup_y = LINES / 2; // Sous l'affichage de la main
    int popup_x = (COLS - popup_width) / 2;
    WINDOW* popup = newwin(popup_height, popup_width, popup_y, popup_x);
    box(popup, 0, 0);

    // Affiche une instruction dans la fenêtre
    mvwprintw(popup, 1, 2, "Entrez l'index d'une carte Trésor à jouer (ou -1 pour passer) :");
    wrefresh(popup);

    // Lecture de l'entrée utilisateur
    std::string choix;
    echo(); // Affiche l'entrée de l'utilisateur dans la fenêtre
    char input[10];
    wgetnstr(popup, input, 10);
    noecho(); // Arrête d'afficher les entrées utilisateur
    choix = input;
    delwin(popup); // Supprime la fenêtre popup

    // Gestion de l'entrée utilisateur
    if (choix == "-1") break; // Quitte la boucle si l'utilisateur passe
    try {
        size_t index = std::stoi(choix);
        if (index < j->getHand().size()) {
            Carte* carte = j->getHand().at(index);
            if (carte->getType() == TypeCarte::Tresor) {
                if (carte->getName() == "Argent" && m_nbMarchand != 0) {
                    carte->play(*j, m_plateau, index, *this);
                    j->addCoins(m_nbMarchand);
                    m_nbMarchand = 0;
                } else {
                    carte->play(*j, m_plateau, index, *this);
                }
            } else {
                // Si ce n'est pas une carte Trésor, afficher une erreur dans une fenêtre
                WINDOW* error = newwin(popup_height, popup_width, popup_y, popup_x);
                box(error, 0, 0);
                mvwprintw(error, 2, 2, "Erreur : Ce n'est pas une carte Trésor !");
                wrefresh(error);
                getch(); // Attendre que l'utilisateur appuie sur une touche
                delwin(error);
            }
        } else {
            // Afficher une erreur si l'index est invalide
            WINDOW* error = newwin(popup_height, popup_width, popup_y, popup_x);
            box(error, 0, 0);
            mvwprintw(error, 2, 2, "Erreur : Index invalide !");
            wrefresh(error);
            getch(); // Attendre que l'utilisateur appuie sur une touche
            delwin(error);
        }
    } catch (...) {
        // Afficher une erreur si l'entrée est incorrecte
        WINDOW* error = newwin(popup_height, popup_width, popup_y, popup_x);
        box(error, 0, 0);
        mvwprintw(error, 2, 2, "Erreur : Entrée non valide !");
        wrefresh(error);
        getch(); // Attendre que l'utilisateur appuie sur une touche
        delwin(error);
    }

}
  
  
  while (j->getNbBuys() > 0) {
    int coins = j->getCoins();
    int buys = j->getNbBuys();
    int score = j->calculerPoints();
    std::string pseudo = j->getPseudo();
    if(verifWin()) return;

    m_plateau.print(pseudo, coins, buys, score); // Affichage du plateau avec les infos du joueur

    // Afficher la fenêtre flottante pour demander l'index
    WINDOW* popup = newwin(5, 70, (LINES - 5) / 2, (COLS - 50) / 2); // Fenêtre centrée
    box(popup, 0, 0);
    mvwprintw(popup, 1, 2, "Entrez l'index de la pile pour acheter une carte:");
    mvwprintw(popup, 3, 2, "Tapez 'PASSER' pour passer.");
    wrefresh(popup);

    char choix[10]; // Utiliser un tableau pour gérer "PASSER"
    echo();
    wgetnstr(popup, choix, 10); // Lecture de l'entrée utilisateur
    noecho();
    delwin(popup); // Supprimer la fenêtre flottante

    std::string choix_str(choix); // Convertir en chaîne de caractères
    if (choix_str == "PASSER") break;

    try {
        index = std::stoi(choix_str);
        if (index < m_plateau.getMaxIndex()) {
            if (j->getCoins() >= m_plateau.chercherCoutParIndex(index)) {
                try {
                    j->buyCard(index, m_plateau);

                    // Afficher une fenêtre de succès
                    WINDOW* success = newwin(3, 40, (LINES - 3) / 2, (COLS - 40) / 2);
                    box(success, 0, 0);
                    mvwprintw(success, 1, 2, "Achat réussi !");
                    wrefresh(success);
                    getch();
                    delwin(success);
                } catch (const std::exception& e) {
                    // Afficher une fenêtre d'erreur
                    WINDOW* error = newwin(5, 50, (LINES - 5) / 2, (COLS - 50) / 2);
                    box(error, 0, 0);
                    mvwprintw(error, 1, 2, "Erreur: %s", e.what());
                    mvwprintw(error, 3, 2, "Appuyez sur une touche pour continuer...");
                    wrefresh(error);
                    getch();
                    delwin(error);
                }
            } else {
                // Afficher une fenêtre d'erreur pour pièces insuffisantes
                WINDOW* error = newwin(5, 50, (LINES - 5) / 2, (COLS - 50) / 2);
                box(error, 0, 0);
                mvwprintw(error, 1, 2, "Vous n'avez pas assez de pièces !");
                mvwprintw(error, 3, 2, "Appuyez sur une touche pour continuer...");
                wrefresh(error);
                getch();
                delwin(error);
            }
        } else {
            // Afficher une fenêtre d'erreur pour index invalide
            WINDOW* error = newwin(5, 50, (LINES - 5) / 2, (COLS - 50) / 2);
            box(error, 0, 0);
            mvwprintw(error, 1, 2, "Index invalide !");
            mvwprintw(error, 3, 2, "Appuyez sur une touche pour continuer...");
            wrefresh(error);
            getch();
            delwin(error);
        }
    } catch (...) {
        // Gérer les erreurs de conversion avec une fenêtre d'erreur
        WINDOW* error = newwin(5, 50, (LINES - 5) / 2, (COLS - 50) / 2);
        box(error, 0, 0);
        mvwprintw(error, 1, 2, "Erreur lors de la lecture de l'index !");
        mvwprintw(error, 3, 2, "Appuyez sur une touche pour continuer...");
        wrefresh(error);
        getch();
        delwin(error);
    }
    
    //phase ajustement
    std::cout << "======== Phase Ajustement ========" << std::endl;
    j->defausser(); //Défausser toutes les cartes jouées et les cartes en main

    //Reformer le deck si nécessaire et piocher 5 cartes pour une nouvelle main
    if (j->getDeck().size() < 5) {
      j->assembleDeckDefausse();
    }
    j->makeHand();  // Piocher 5 nouvelles cartes

    std::cout << "Votre nouvelle main pour le prochain tour est prête !" << std::endl;
    j->printHand();
  }
}


void Jeu::tousSaufActifMalediction(){
  for(size_t i = 0; i < m_listeJoueur.size(); i++){
    if(&m_listeJoueur.at(i) != m_joueurActif && std::find(m_joueursImmunises.begin(), m_joueursImmunises.end(), i) == m_joueursImmunises.end()){
      m_listeJoueur.at(i).receiveMalediction(m_plateau);
    }
  }
}

std::vector<size_t> Jeu::revelerCartes(){
        std::vector<size_t> indexJoueur;
	for(size_t i = 0; i < m_listeJoueur.size(); i++){
		if(&m_listeJoueur.at(i) != m_joueurActif && std::find(m_joueursImmunises.begin(), m_joueursImmunises.end(), i) == m_joueursImmunises.end()){
		        indexJoueur.push_back(i);
			m_listeJoueur.at(i).devoiler2Cartes(m_plateau);
			mapCartesDevoilees[i] = m_plateau.getListeCartesDevoilees().back();
		}
	}
	return indexJoueur;
}

void Jeu::banditisme(){
	//m_joueurActif->printDefausse();		//Permet de vérifier qu'on recoit bien la carte Or
	revelerCartes();
	std::vector<std::vector<Carte*>>& listeCartesDevoilees = m_plateau.getListeCartesDevoilees();
	for(size_t i = 0; i < listeCartesDevoilees.size(); i++){
		if(&m_listeJoueur.at(i) != m_joueurActif && std::find(m_joueursImmunises.begin(), m_joueursImmunises.end(), i) == m_joueursImmunises.end()){
			for(size_t j = 0; j < 2; j++){
				Carte* c = listeCartesDevoilees.at(i).at(j);
				c->printCard();
				if(c->getType() == TypeCarte::Tresor && c->getName() != "Cuivre"){
					std::vector<Carte*>& rebut = m_listeJoueur.at(i).getRebut();
					rebut.push_back(c);
					//m_listeJoueur.at(i).printRebut();
				}
				std::vector<Carte*>& defausse = m_listeJoueur.at(i).getDefausse();
				defausse.push_back(c);
				//m_listeJoueur.at(i).printDefausse();
			}
		}
	}	
}

void Jeu::volerCartesAdversaires(){
	std::vector<std::vector<Carte*>>& listeCartesDevoilees = m_plateau.getListeCartesDevoilees();
	std::vector<Carte*>& listeCartesEcartees = m_plateau.getListeCartesEcartees();
	size_t index;
	bool stop = false;
	bool tresor = false;
	std::string res1,res2;
	std::vector<size_t> indexJoueur = revelerCartes();
	for(size_t i = 0; i < listeCartesDevoilees.size(); i++){
		for(size_t j = 0; j < 2; j++){
			Carte* c = listeCartesDevoilees.at(i).at(j);
			if(c->getType() == TypeCarte::Tresor) tresor = true;
		}
		m_joueurActif->printCards(listeCartesDevoilees.at(i), "Cartes Devoilees");
		if(tresor){
		        std::cout << "Voulez-vous écarter une carte trésor ? Répondez par 'Oui' ou 'oui' ou 'O' ou 'o' si vous le souhaitez" << std::endl;
		        std::cin >> res1;
	                if(res1 == "O" or res1 == "o" or res1 == "Oui" or res1 == "oui"){
	                        while(true){
		                        m_joueurActif->printCards(listeCartesDevoilees.at(i), "Cartes Devoilees");
		                        std::cout << "Quelle carte trésor voulez-vous écarter ? (Entrez l'index)" << std::endl;
		                        std::cin >> res1;
		                        try{
		                                size_t j = std::stoi(res1);
		                                if(listeCartesDevoilees.at(i).at(j)->getType() == TypeCarte::Tresor){
	                                                  listeCartesEcartees.push_back(listeCartesDevoilees.at(i).at(j));
	                                                  break;
	                                        }
	                                        else{
	                                                  std::cout << "Ce n'est pas une carte trésor" << std::endl;
	                                        }
	                                }
	                                catch(...){
	                                        std::cerr << "Index Invalide" << std::endl;
	                                }
		                }
	                }
	                tresor = false;
		}
	}
	while(!stop and !listeCartesEcartees.empty()){
		std::cout << "Souhaitez-vous récupérer une des cartes écartées ?" << std::endl;
		std::cin >> res2;
		if(res2 == "O" or res2 == "o" or res2 == "Oui" or res2 == "oui"){
		        m_joueurActif->printCards(listeCartesEcartees, "Cartes Ecartees");
			std::cout << "Entrer l'index de la carte que vous souhaitez récupérer : " << std::endl;
			std::cin >> index;
			if(index < listeCartesEcartees.size()){
				Carte* c = listeCartesEcartees.at(index);
		                m_joueurActif->getDefausse().push_back(c);
		                
		                for (auto& [joueurIndex, cartes] : mapCartesDevoilees) {
                                        auto point = std::find(cartes.begin(), cartes.end(), c);
                                        if (point != cartes.end()) {
                                                  cartes.erase(point);
                                                  m_listeJoueur.at(joueurIndex).getDefausse().erase(std::remove(m_listeJoueur.at(joueurIndex).getDefausse().begin(), m_listeJoueur.at(joueurIndex).getDefausse().end(), c), m_listeJoueur.at(joueurIndex).getDefausse().end());
                                                  break;
                                        }
                                }
		                
			        listeCartesEcartees.erase(listeCartesEcartees.begin() + index);
			}
			else{
			        std::cout << "Index invalide" << std::endl;
			}
		}
		else{
		  stop = true;
		}
	}
	listeCartesDevoilees.clear();
	listeCartesEcartees.clear();
}

void Jeu::tousSaufActifPiochent(){
  for(size_t i = 0; i < m_listeJoueur.size(); i++){
    if(&m_listeJoueur.at(i) != m_joueurActif){
      m_listeJoueur.at(i).piocher();
    }
  }
}

void Jeu::tousSaufActifDefausseJusqua(size_t n){
  for(size_t i = 0; i < m_listeJoueur.size(); i++){
    if(&m_listeJoueur.at(i) != m_joueurActif && std::find(m_joueursImmunises.begin(), m_joueursImmunises.end(), i) == m_joueursImmunises.end()){
      while(m_listeJoueur.at(i).getHand().size() > n){
        m_listeJoueur.at(i).demandeDefausse();
      }
    }
  }
}

void Jeu::tousSaufActifPoseCarteVictoire(){
  for(size_t i = 0; i < m_listeJoueur.size(); i++){
    if(&m_listeJoueur.at(i) != m_joueurActif && std::find(m_joueursImmunises.begin(), m_joueursImmunises.end(), i) == m_joueursImmunises.end()){
      std::vector<size_t> indices = m_listeJoueur.at(i).getNbCarteVictoireHand().second;
      std::vector<Carte*> temp;
      for(const auto index : indices){
      	Carte* c = m_listeJoueur.at(i).getHand().at(index);
      	temp.push_back(c);
      	//m_listeJoueur.at(i).getHand().at(index).printCard();
      }
      m_listeJoueur.at(i).printCards(temp,"Cartes Victoire de la main");
      if(m_listeJoueur.at(i).getNbCarteVictoireHand().first == 0){
        m_listeJoueur.at(i).printHand(); // TODO Dévoiler la main aux autres joueurs (peut-être à changer)
      }
      else {
      	m_listeJoueur.at(i).carteVictoireOnDeck();
      	m_listeJoueur.at(i).printDeck();
      }
    }
  }
}

bool Jeu::verifWin(){
  for(auto& pile : m_plateau.getPilesVictoire()){
    if(pile.second == 0 && pile.first.getName() == "Province") return true;
  }
  
  return m_plateau.getNbPileVide() >= 3;
}

/*
void Jeu::calculerScoreFinal() {
    std::cout << "======== Affichage des scores ========" << std::endl;
    std::vector<std::pair<Joueur, int>> tabScore;
    for (auto& joueur : m_listeJoueur) {
        int score = joueur.calculerPoints();
        tabScore.push_back(std::make_pair(joueur, score));
    }
    
    std::sort(tabScore.begin(), tabScore.end(), [](const auto &p1, const auto &p2){
      return p1.second > p2.second;
    });
    
    std::cout << std::left << std::setw(15) << "Joueur" << "Score" << std::endl;
    std::cout << "-------------------------------" << std::endl;
    for (size_t i = 0; i < tabScore.size(); ++i) {
        if (i == 0 || tabScore.at(i).second == tabScore.at(0).second) {
            std::cout << std::left << std::setw(15) << tabScore.at(i).first.getPseudo() 
                      << tabScore.at(i).second << " (GAGNANT)" << std::endl;
        } else {
            std::cout << std::left << std::setw(15) << tabScore.at(i).first.getPseudo() 
                      << tabScore.at(i).second << std::endl;
        }
    }
    std::cout << "===============================" << std::endl;
}

*/

void Jeu::calculerScoreFinal() {
    initscr();
    start_color();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    // Initialiser une paire de couleurs pour l'affichage des scores
    init_pair(1, COLOR_GREEN, COLOR_BLACK); // Gagnant
    init_pair(2, COLOR_WHITE, COLOR_BLACK); // Autres joueurs

    // Calculer les scores des joueurs
    std::vector<std::pair<Joueur, int>> tabScore;
    for (auto& joueur : m_listeJoueur) {
        int score = joueur.calculerPoints();
        tabScore.push_back(std::make_pair(joueur, score));
    }

    // Trier les joueurs par score décroissant
    std::sort(tabScore.begin(), tabScore.end(), [](const auto& p1, const auto& p2) {
        return p1.second > p2.second;
    });

    // Déterminer la taille de la fenêtre en fonction du nombre de joueurs
    int num_lines = 4 + tabScore.size(); // 4 lignes pour le titre, séparateur, et espace
    int num_cols = 50; // Largeur fixe pour l'affichage
    int start_y = (LINES - num_lines) / 2;
    int start_x = (COLS - num_cols) / 2;

    // Créer une fenêtre pour afficher les scores
    WINDOW* score_popup = newwin(num_lines, num_cols, start_y, start_x);
    box(score_popup, 0, 0);

    // Afficher le titre
    wattron(score_popup, COLOR_PAIR(1));
    mvwprintw(score_popup, 1, (num_cols - 26) / 2, "======== Tableau des scores ========");
    wattroff(score_popup, COLOR_PAIR(1));

    // Afficher les scores des joueurs
    mvwprintw(score_popup, 2, 2, "Joueur           | Score");
    mvwhline(score_popup, 3, 1, '-', num_cols - 2); // Ligne de séparation

    for (size_t i = 0; i < tabScore.size(); ++i) {
        int line = 4 + i; // Ligne de départ pour les scores
        if (i == 0) { // Le gagnant
            wattron(score_popup, COLOR_PAIR(1));
            mvwprintw(score_popup, line, 2, "%-15s | %d (GAGNANT)", tabScore[i].first.getPseudo().c_str(), tabScore[i].second);
            wattroff(score_popup, COLOR_PAIR(1));
        } else { // Autres joueurs
            wattron(score_popup, COLOR_PAIR(2));
            mvwprintw(score_popup, line, 2, "%-15s | %d", tabScore[i].first.getPseudo().c_str(), tabScore[i].second);
            wattroff(score_popup, COLOR_PAIR(2));
        }
    }

    // Rafraîchir et afficher la fenêtre
    wrefresh(score_popup);

    // Attendre une touche pour fermer la fenêtre
    mvwprintw(score_popup, num_lines - 2, 2, "Appuyez sur une touche pour continuer...");
    wrefresh(score_popup);
    getch();

    // Supprimer la fenêtre
    delwin(score_popup);
    endwin();
}


/*
void Jeu::jouerPartie(){
  std::cout << "======== Début de la partie ========" << std::endl;
  int nbTour = 0;
  while (!verifWin()) {
    nbTour++;
    std::cout << "\n======== Tour numéro " << nbTour << " ========" << std::endl;
    for (size_t i = 0; i < m_listeJoueur.size(); i++) {
      m_joueurActif = &m_listeJoueur.at(i);
      std::cout << "\nTour de " << m_joueurActif->getPseudo() << std::endl;
      tourJoueur(m_joueurActif);
      if (verifWin()) {
        std::cout << "Fin de la partie atteinte en " << nbTour << " tours" << std::endl;
        break;
      }
    }
  }
  calculerScoreFinal();
}
*/

void Jeu::jouerPartie() {
    initscr();
    start_color();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    // Initialiser une paire de couleurs pour les messages de popup
    init_pair(1, COLOR_YELLOW, COLOR_BLACK);

    // Rafraîchir l'écran principal avant d'afficher des fenêtres
    clear();
    refresh();
    
    // Affichage du message "Début de la partie"
    WINDOW* popup = newwin(5, 50, (LINES - 5) / 2, (COLS - 50) / 2); // Fenêtre centrée
    box(popup, 0, 0);
    wattron(popup, COLOR_PAIR(1));
    mvwprintw(popup, 2, 15, " Début de la partie ");
    wattroff(popup, COLOR_PAIR(1));
    wrefresh(popup);
    getch();
    delwin(popup);

    // Boucle principale de la partie
    int nbTour = 0;
    while (!verifWin()) {
        nbTour++;

        // Affichage du message "Tour numéro x"
        popup = newwin(5, 50, (LINES - 5) / 2, (COLS - 50) / 2); // Fenêtre centrée
        box(popup, 0, 0);
        wattron(popup, COLOR_PAIR(1));
        mvwprintw(popup, 2, 15, " Tour numéro %d ", nbTour);
        wattroff(popup, COLOR_PAIR(1));
        wrefresh(popup);
        getch();
        delwin(popup);

        // Tour de chaque joueur
        for (size_t i = 0; i < m_listeJoueur.size(); i++) {
            m_joueurActif = &m_listeJoueur.at(i);

            // Affichage du message "Tour de joueur"
            popup = newwin(5, 50, (LINES - 5) / 2, (COLS - 50) / 2); // Fenêtre centrée
            box(popup, 0, 0);
            wattron(popup, COLOR_PAIR(1));
            mvwprintw(popup, 2, 15, "Tour de %s", m_joueurActif->getPseudo().c_str());
            wattroff(popup, COLOR_PAIR(1));
            wrefresh(popup);
            getch();
            delwin(popup);

            // Exécuter le tour du joueur
            tourJoueur(m_joueurActif);

            // Vérifier la condition de fin
            if (verifWin()) {
                break;
            }
        }

        // Sortir de la boucle principale si la partie est gagnée
        if (verifWin()) {
            break;
        }
    }

    // Affichage du message "Fin de partie"
    popup = newwin(5, 50, (LINES - 5) / 2, (COLS - 50) / 2); // Fenêtre centrée
    box(popup, 0, 0);
    wattron(popup, COLOR_PAIR(1));
    mvwprintw(popup, 2, 8, "Fin de la partie atteinte en %d tours", nbTour);
    wattroff(popup, COLOR_PAIR(1));
    wrefresh(popup);
    getch();
    delwin(popup);

    endwin();

    // Calculer et afficher le score final
    calculerScoreFinal();
}



void Jeu::espionnage(){
	std::vector<bool> reponse = m_joueurActif->decideDefausse(m_listeJoueur);
	
	for(size_t i = 0; i < m_listeJoueur.size(); i++){
		if(std::find(m_joueursImmunises.begin(), m_joueursImmunises.end(), i) == m_joueursImmunises.end()){
			std::cout << m_listeJoueur.at(i).getPseudo() << " dévoile la 1ère carte de son deck : " << std::endl;
			m_listeJoueur.at(i).getDeck().at(0)->printCard();
			if(reponse.at(i) == true){
				m_listeJoueur.at(i).defausseCarteDeck(0);
			}		
		}
		
	}
}
	

void Jeu::verifDouve(){
  for(size_t i = 0; i < m_listeJoueur.size(); i++){
    if(m_listeJoueur.at(i).handContainsDouve()){
      std::cout << "Le joueur " << m_listeJoueur.at(i).getPseudo() << " est immunisé grâce à sa carte Douve" << std::endl;
      m_joueursImmunises.push_back(i);
    }
  }
}

void Jeu::viderImmunises(){
  m_joueursImmunises = {};
}

void Jeu::addNbMarchand(){
  m_nbMarchand++;
}
