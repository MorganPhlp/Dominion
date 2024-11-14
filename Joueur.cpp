#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>
#include "Joueur.h"
#include "Jeu.h"

/*Constructeur*/
Joueur::Joueur(std::string pseudo) : m_pseudo(pseudo) {
	m_nb_win_points = 0;
	m_coins = 0;
	m_nb_actions = 0;
	m_nb_buys = 0;
	m_draws = 0;
	m_hand = {};
	m_deck = {};
	m_defausse = {};
	m_rebut = {};
}

/*Destruction*/
Joueur::~Joueur() {}

void Joueur::addBuys(int buys){m_nb_buys += buys;}

void Joueur::addActions(int actions){m_nb_actions += actions;}

void Joueur::addCoins(int coins){m_coins += coins;}

void Joueur::addDraws(int draws){m_draws += draws;}

void Joueur::addWinPoints(int win_points){m_nb_win_points += win_points;}

void Joueur::initDeck(Plateau p){
	m_deck = {};
	std::vector<CarteTresor> listeTresor = p.getListeCarteTresor();
	int indexCuivre;
	for(size_t i = 0; i < listeTresor.size(); i++){
		if(listeTresor.at(i).getName() == "Cuivre"){
			indexCuivre = i;
			break;
		}
	} 
	Carte* c = &listeTresor.at(indexCuivre);
	for(size_t i = 0; i < 7; i++){
		m_deck.push_back(c);
	}
	
	std::vector<CarteVictoire> listeVictoire = p.getListeCarteVictoire();
	int indexDomaine;
	for(size_t i = 0; i < listeVictoire.size(); i++){
		if(listeVictoire.at(i).getName() == "Domaine"){
			indexDomaine = i;
			break;
		}
	} 
	c = &listeVictoire.at(indexDomaine);
	for(size_t i = 0; i < 3; i++){
		m_deck.push_back(c);
		addWinPoints(1);
	}
	
	shuffleCartes(m_deck);
}

void Joueur::shuffleCartes(std::vector<Carte*> v){
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	shuffle(v.begin(), v.end(), std::default_random_engine(seed));
}

void Joueur::printDeck() const{
	std::cout << "======== DECK " << m_pseudo << " ========\n\n";
	
	for(size_t i = 0; i < m_deck.size(); i++){
        	m_deck.at(i)->printCard();
        	std::cout << "-----------------------------------\n";
	}
}

void Joueur::assembleDeckDefausse(){
	shuffleCartes(m_defausse);
	for(size_t i = 0; i < m_defausse.size(); i++){
		m_deck.push_back(m_defausse.at(i));
	}
	m_defausse = {};
}

void Joueur::makeHand(){
	m_hand = {};
	if(m_deck.size() < 5) assembleDeckDefausse();
	for(size_t i = 0; i < 5; i++){
		m_hand.push_back(m_deck.at(i));
	}
	
	m_deck.erase(m_deck.begin(), m_deck.begin()+5);
}

void Joueur::defausser(){
	for(size_t i = 0; i < m_hand.size(); i++){
		m_defausse.push_back(m_deck.at(i));
	}
	m_hand = {};
}

void Joueur::receiveCard(int n, Plateau &plat){
  std::vector<std::pair<Carte*, int>> liste = plat.getMax(n);
  std::cout << "Quelle carte voulez-vous parmi les suivantes : " << std::endl;
  for(size_t i = 0; i < liste.size(); i++){
    if(i== liste.size()-1) std::cout << liste.at(i).first->getName() << std::endl;
    else std::cout << liste.at(i).first->getName() << "; ";
  }
  size_t index = 10000;
  while(index >= liste.size()){
    std:: cin >> index;
    //TODO Gérer si on donne un string que ça renvoie erreur
  }
  Carte* c = plat.buyCard(liste.at(index).second);
  m_hand.push_back(c);
}

void Joueur::throwMax(int n){ // TODO Gestion erreurs
  //printHand();
  std:: cout << "Vous pouvez jeter jusqu'à " << n << " cartes de votre main" << std::endl;
  std::string rep;
  bool stop = false;
  for(int i = 0; i < n; i++){
    while(!stop){
      std:: cout << "Quelle carte voulez-vous jeter ? (Ecrivez l'index de la carte ou STOP si vous ne voulez plus en jeter)" << std::endl;
      std::cin >> rep;
      if(rep == "STOP") stop = true;
      else if(std::stoul(rep) < m_hand.size()){
        //defausseCarte(std::stoul(rep));
        break;
      }
    }
    if(stop){
      break;
    }
  }
}
