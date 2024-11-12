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
	m_hand = {};
	m_deck = {};
	m_defausse = {};
	m_rebut = {};
}

/*Desctruction*/
Joueur::~Joueur() {}

void Joueur::addBuys(int buys){m_nb_buys += buys;}

void Joueur::addActions(int actions){m_nb_actions += actions;}

void Joueur::addCoins(int coins){m_coins += coins;}

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
	m_hand = {}
}




