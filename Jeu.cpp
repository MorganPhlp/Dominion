#include "Jeu.h"
#include <random>
#include <chrono>
#include <functional>

Jeu::Jeu(int nb_joueurs, Plateau plateau, std::vector<Joueur> listeJoueur) : m_nb_joueurs(nb_joueurs), m_plateau(plateau), m_listeJoueur(listeJoueur) {
	for(size_t i = 0; i < m_listeJoueur.size(); i++){
		m_listeJoueur.at(i).initDeck(m_plateau);
	}
	initJoueurActif(m_listeJoueur);
}

Jeu::~Jeu() {}

void Jeu::initJoueurActif(std::vector<Joueur>& liste){
	std::default_random_engine re(std::chrono::system_clock::now().time_since_epoch().count());
	int taille = liste.size();
	std::uniform_int_distribution<int> distrib{0, taille-1};
	auto rd = bind(distrib, re);
	int alea = rd();
	m_joueurActif = &liste.at(alea);
}

Plateau Jeu::getPlateau(){
	return m_plateau;
}

void Jeu::tourJoueur(Joueur* j){
	j->initNouveauTour();
	j->printHand();
	std::string choix;
	size_t index;
	
	//phase action
	std::cout << "======== Phase Action ========" << std::endl;
	while(!j->getHand().empty() or choix != "ACHAT" or j->getNbActions() > 0){
		std::cout << "Entrez le numéro de la carte que vous souhaitez jouer : " << std::endl;
		std::cin >> index;
		if(index > 0 and index < j->getHand().size()){
			try{
				j->getHand().at(index)->play(*j, m_plateau, index, *this);		//methode a faire dans la classe Carte
				j->addActions(-1);
			}
			catch (const std::exception& e){
				std::cerr << "Erreur : " << e.what() << std::endl;
			}
			
		}
		std::cout << "Ecrivez ACHAT si vous voulez passer à la phase Achat" << std::endl;
		std::cin >> choix;
		if(choix == "ACHAT"){
			break;
		}
	}
	
	//phase achat
	std::cout << "======== Phase Achat ========" << std::endl;
	while(j->getNbBuys() > 0){
		std::cout << "Choisissez l'index de la pile de carte dont vous souhaitez acheter une carte : " << std::endl;
		std::cin >> index;
		try{
			if(index < m_plateau.getMaxIndex()){
				j->buyCard(index,m_plateau);
			} else {
				std::cerr << "Index invalide. Réessayez. " << std::endl;
			}
		}
		catch (const std::exception& e){
			std::cerr << "Erreur : " << e.what() << std::endl;
		}
	}
	
	//phase ajustement
	std::cout << "======== Phase Ajustement ========" << std::endl;
	
    	j->defausser(); //Défausser toutes les cartes jouées et les cartes en main

    	//Reformer le deck si nécessaire et piocher 5 cartes pour une nouvelle main
    	if (j->getDeck().size() < 5) {
        	j->assembleDeckDefausse();  // Mélanger la défausse pour reformer le deck
    	}
    	j->makeHand();  // Piocher 5 nouvelles cartes

    	std::cout << "Votre nouvelle main pour le prochain tour est prête !" << std::endl;
    	j->printHand();  // Afficher la nouvelle main du joueur
	
}

void Jeu::tousSaufActifMalediction(){
  for(size_t i = 0; i < m_listeJoueur.size(); i++){
    if(&m_listeJoueur.at(i) != m_joueurActif){
      m_listeJoueur.at(i).receiveMalediction(m_plateau);
    }
  }
}

void Jeu::afficheCartesAdversaires(){
	size_t index;
	bool stop = false;
	for(size_t i = 0; i < m_listeJoueur.size(); i++){
		if(&m_listeJoueur.at(i) != m_joueurActif){
			m_listeJoueur.at(i).devoiler2Cartes(m_plateau);
		}
	}
	for(size_t i = 0; i < m_plateau.m_listeCartesDevoilees.size(); i++){
		for(size_t j = 0; j < 2; j++){
			Carte* c = m_plateau.m_listeCartesDevoilees.at(i).at(j);
			c->printCard();
			if(c->getType() == TypeCarte::Tresor){
				m_plateau.m_listeCartesEcartees.push_back(c);
				break;
			}
		}
	}
	while(!stop){
		std::cout << "Entrer l'index de la carte que vous souhaitez récupérer : " << std::endl;
		std::cin >> index;
		if(index >= m_plateau.m_listeCartesEcartees.size(){
			stop = true;
		}
		Carte* c = m_plateau.m_listeCartesEcartees.at(index);
		m_joueurActif.m_defausse.push_back(c);
		m_plateau.m_listeCartesEcartees.erase(m_plateau.m_listeCartesEcartees.begin() + index);
		
	}
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
    if(&m_listeJoueur.at(i) != m_joueurActif){
      m_listeJoueur.at(i).printHand();
      while(m_listeJoueur.at(i).getHand().size() > n){
        m_listeJoueur.at(i).demandeDefausse();
      }
    }
  }
}
