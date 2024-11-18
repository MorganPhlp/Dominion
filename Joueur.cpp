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
	m_draws = 0;
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

int Joueur::getCoins(){	return m_coins;}

int Joueur::getNbActions(){ return m_nb_actions;}

int Joueur::getNbBuys(){ return m_nb_buys;}

int Joueur::getNbWinPoints(){ return m_nb_win_points;}

int Joueur::getDraws(){ return m_draws;}

std::vector<Carte*> Joueur::getHand(){ return m_hand;}

std::vector<Carte*> Joueur::getDeck(){ return m_deck;}

std::vector<Carte*> Joueur::getDefausse(){ return m_defausse;}

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

void Joueur::printHand() const{
	std::cout << "======== Main " << m_pseudo << " ========\n\n";
	
	for(size_t i = 0; i < m_hand.size(); i++){
        	m_hand.at(i)->printCard();
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
		m_defausse.push_back(m_hand.at(i));
	}
	m_hand = {};
}

void Joueur::defausseCarte(int index){
	m_defausse.push_back(m_hand.at(index));
	m_hand.erase(m_hand.begin()+index);
}

void Joueur::initNouveauTour(){
	m_coins = 0;
	m_nb_actions = 1;
	m_nb_buys = 1;
}

void Joueur::buyCard(int index, Plateau plat){
	Carte* c = plat.buyCard(index);
	if(c->getCost() <= m_coins){
		m_coins -= c->getCost();
		m_nb_buys -= 1;
		m_defausse.push_back(c);
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
  }
  Carte* c = plat.buyCard(liste.at(index).second);
  m_hand.push_back(c);
}

void Joueur::throwMax(int n){ // TODO Gestion erreurs
  printHand();
  std:: cout << "Vous pouvez jeter jusqu'à " << n << " cartes de votre main" << std::endl;
  std::string rep;
  bool stop = false;
  for(int i = 0; i < n; i++){
    while(!stop){
      std:: cout << "Quelle carte voulez-vous jeter ? (Ecrivez l'index de la carte ou STOP si vous ne voulez plus en jeter)" << std::endl;
      std::cin >> rep;
      if(rep == "STOP") stop = true;
      else if(std::stoul(rep) < m_hand.size()){
        jeterCarte(std::stoul(rep));
        break;
      }
    }
    if(stop){
      break;
    }
  }
}

void Joueur::jeterCarte(int index){
  m_hand.erase(m_hand.begin()+index);
}

void Joueur::receiveMalediction(Plateau &plat){
  int index = plat.chercherCarteVictoire("Malédiction");
  Carte* c = plat.buyCard(index);
  m_defausse.push_back(c);
  addWinPoints(-1);
}

void Joueur::putCardFromHandToDeck(){
  size_t rep;
  bool stop = false;
  while(!stop){
      std:: cout << "Quelle carte voulez-vous mettre sur le haut de votre deck ? (Ecrivez l'index de la carte)" << std::endl;
      std::cin >> rep;
      if(rep < m_hand.size()){
        stop = true;
      }
  }
  m_deck.insert(m_deck.begin(), m_hand.at(rep));
  m_hand.erase(m_hand.begin() + rep);
}

void Joueur::devoiler2Cartes(Plateau &plat){
	std::vector<Carte*> temp;
	for(size_t i = 0; i < 2; i++){
		//verifier la taille du deck cf. dominions regles 1ere edition
		m_deck.at(i)->printCard();
		temp.push_back(m_deck.at(i));
	}
	(void) plat; //Juste pour enlever les bugs de compilation
	//plat.m_listeCartesDevoilees.push_back(temp);
}

void Joueur::receiveOr(Plateau &plat){ // Met dans la defausse
  Carte* c = plat.buyCard(plat.chercherCarteTresor("Or"));
  m_defausse.push_back(c);
}

void Joueur::receiveArgent(Plateau &plat){ //Met sur le haut du deck
  Carte* c = plat.buyCard(plat.chercherCarteTresor("Argent"));
  m_deck.insert(m_deck.begin(), c);
}

void Joueur::defaussePuisPioche(){ //TODO Gerer Gestion Erreurs
  printHand();
  size_t nb = 0;
  std::string rep;
  bool stop = false;
  while(!stop && m_hand.size() != 0){
      std:: cout << "Quelle carte voulez-vous défausser ? (Ecrivez l'index de la carte ou STOP si vous ne voulez plus en jeter)" << std::endl;
      std::cin >> rep;
      if(rep == "STOP") stop = true;
      else if(std::stoul(rep) < m_hand.size()){
        defausseCarte(std::stoul(rep));
        nb++;
      }
  }
  if(m_deck.size() < nb) assembleDeckDefausse();
  for(size_t i = 0; i < nb; i++){
    m_hand.push_back(m_deck.at(i));
  }
  m_deck.erase(m_deck.begin(), m_deck.begin()+nb);
}

void Joueur::piocher(){
  m_hand.push_back(m_deck.at(0));
}

void Joueur::printDefausse(){
  std::cout << "======== Defausse " << m_pseudo << " ========\n\n";

  for(size_t i = 0; i < m_hand.size(); i++){
    m_defausse.at(i)->printCard();
    std::cout << "-----------------------------------\n";
  }
}

void Joueur::regarderDefausseEtPrendre(){ //TODO Gerer Gestion Erreurs
  printDefausse(); // Rajouter les index
  std::string rep;
  size_t index;
  std::cout << "Quelle carte voulez vous prendre et mettre sur le dessus de votre Deck ? (Ecrivez l'index de la carte ou autre chose si vous ne voulez-pas en prendre)" << std::endl;
  std::cin >> rep;
  try{
    index = std::stoul(rep);
    if(index < m_hand.size()){
      m_deck.insert(m_deck.begin(), m_defausse.at(index));
      m_defausse.erase(m_defausse.begin()+index);
    }
  } catch (...) {}
}

void Joueur::demandeDefausse(){
  std::string rep;
  size_t index;
  std::cout << "Quelle carte voulez vous defausser ? (Ecrivez l'index de la carte)" << std::endl;
  std::cin >> rep;
  try{
    index = std::stoul(rep);
    if(index < m_hand.size()){
      defausseCarte(index);
    }
  } catch (...) {}
}
