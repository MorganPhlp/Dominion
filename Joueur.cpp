#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>
#include <algorithm>
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

std::string Joueur::getPseudo(){ return m_pseudo;}

std::vector<Carte*> Joueur::getHand(){ return m_hand;}

std::vector<Carte*> Joueur::getDeck(){ return m_deck;}

std::vector<Carte*>& Joueur::getDefausse(){ return m_defausse;}

std::vector<Carte*>& Joueur::getRebut(){ return m_rebut;}

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
	size_t taille = m_deck.size();
	for(size_t i = 0; i < 2; i++){
		switch (taille) {
			case 0:
				assembleDeckDefausse();
				for(size_t j = 0; j < 2; j++){
					m_deck.at(j)->printCard();
					temp.push_back(m_deck.at(j));	
				}
				break;
			case 1:
				m_deck.at(0)->printCard();
				temp.push_back(m_deck.at(0));
				assembleDeckDefausse();
				temp.push_back(m_deck.at(0));
				break;
		}
		m_deck.at(i)->printCard();
		temp.push_back(m_deck.at(i));
	}
	std::vector<std::vector<Carte*>>& listeCartesDevoilees = plat.getListeCartesDevoilees();
	listeCartesDevoilees.push_back(temp);
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
  m_deck.erase(m_deck.begin());
}

void Joueur::printDefausse(){
  std::cout << "======== Defausse " << m_pseudo << " ========\n\n";

  for(size_t i = 0; i < m_hand.size(); i++){
    m_defausse.at(i)->printCard();
    std::cout << "-----------------------------------\n";
  }
}

void Joueur::regarderDefausseEtPrendre(){
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

std::pair<size_t,std::vector<size_t>> Joueur::getNbCarteVictoireHand(){
  size_t nb = 0; 
  std::vector<size_t> indexs;
  for(size_t i = 0; i < m_hand.size(); i++){
    if(m_hand.at(i)->getType() == TypeCarte::Victoire){
      nb++;
      indexs.push_back(i);
    }
  }
  return std::make_pair(nb,indexs);
}

void Joueur::carteVictoireOnDeck(){
  size_t index;
  if(getNbCarteVictoireHand().first == 1){
    for(size_t i = 0; i < m_hand.size(); i++){
      if(m_hand.at(i)->getType() == TypeCarte::Victoire){
        index = i;
        m_deck.insert(m_deck.begin(), m_hand.at(index));
        m_hand.erase(m_defausse.begin()+index);
      }
    }
    std::cout << "Votre seule carte victoire de la main qui était index " << index << " a été mis sur le haut de votre deck" << std::endl;
  }
  else{
    std::pair<size_t,std::vector<size_t>> nbVictoire = getNbCarteVictoireHand();
    bool valide = false;
    std::string rep;
    while(!valide){
      std::cout << "Quelle carte victoire voulez vous prendre et mettre sur le dessus de votre Deck ? (Ecrivez l'index de la carte)" << std::endl;
      std::cin >> rep;
      try{
        index = std::stoul(rep);
        if(index < m_hand.size() && std::find(nbVictoire.second.begin(), nbVictoire.second.end(), index) != nbVictoire.second.end()){
          m_deck.insert(m_deck.begin(), m_hand.at(index));
          m_hand.erase(m_hand.begin()+index);
          valide = true;
        }
      }
      catch(...) {}
    }
  }
}

void Joueur::piocherJusquaEtDefausseAction(size_t n){
  if(m_hand.size() >= n) std::cout << "Vous avez déjà plus de " << n << " cartes en main" << std::endl;
  else{
    std::vector<Carte*> temp;
    while(m_hand.size() < n){
      Carte* c = m_deck.at(0);
      m_deck.erase(m_deck.begin());
      if(c->getType() == TypeCarte::Action){
        bool valide = false;
        std::string rep;
        while(!valide){
          std::cout << "Vous venez de piocher la carte " << c->getName() << " \nVoulez vous la garder dans votre main (OUI pour la garder et NON pour la défausser)" << std::endl;
          std::cin >> rep;
          if(rep == "oui" || rep == "OUI" || rep == "Oui"){
            m_hand.push_back(c);
            valide = true;
          }
          else if(rep == "non" || rep == "NON" || rep == "Non"){
            temp.push_back(c);
            valide = true;
          }
        }
      }
      else m_hand.push_back(c);
    }
    for(size_t i = 0; i < temp.size(); i++){
      m_defausse.push_back(temp.at(i));
    }
  }
}

std::pair<size_t,std::vector<size_t>> Joueur::getNbCarteTresorHand(){
  size_t nb = 0; 
  std::vector<size_t> indexs;
  for(size_t i = 0; i < m_hand.size(); i++){
    if(m_hand.at(i)->getType() == TypeCarte::Tresor){
      nb++;
      indexs.push_back(i);
    }
  }
  return std::make_pair(nb,indexs);
}

void Joueur::receiveTresor(int n, Plateau &plat){
  std::vector<std::pair<Carte*, int>> liste = plat.getMaxTresor(n);
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

void Joueur::jeterTresorPourRecuperPlus(size_t n, Plateau &plat){
  std::pair<size_t,std::vector<size_t>> nbTresor = getNbCarteTresorHand();
  if(nbTresor.first == 0){
    std::cout << "Vous n'avez pas de cartes Tresor à jeter" << std::endl;
  }
  else{
    size_t index;
    bool valide = false;
    std::string rep;
    while(!valide){
      std::cout << "Quelle carte trésor voulez vous jeter pour recevoir une carte Tresor coûtant jusqu'à 3 pièces de plus ? (Ecrivez l'index de la carte ou STOP si vous ne voulez pas en jeter)" << std::endl;
      std::cin >> rep;
      if(rep == "STOP" || rep == "stop" || rep == "Stop") break;
      else{
        try{
          index = std::stoul(rep);
          if(index < m_hand.size() && std::find(nbTresor.second.begin(), nbTresor.second.end(), index) != nbTresor.second.end()){
            size_t cout = m_hand.at(index)->getCost();
            m_rebut.push_back(m_hand.at(index));
            m_hand.erase(m_hand.begin()+index);
            valide = true;
            receiveTresor(cout+n, plat);
          }
        }
        catch(...) {}
      }
    }
  }
}

void Joueur::trocCuivrePieces(){
	std::string res;
	for(size_t i = 0; i < m_hand.size(); i++){
		if(m_hand.at(i)->getName() == "Cuivre"){
			m_hand.at(i)->printCard();
			std::cout << "Voulez-vous jeter cette carte Cuivre pour 3 pièces ?  Répondez par 'Oui' ou 'oui' ou 'O' ou 'o' si vous le souhaitez." << std::endl;
			std::cin >> res;
			if(res == "O" or res == "o" or res == "Oui" or res == "oui"){
				addCoins(3);
				m_rebut.push_back(m_hand.at(i));
				m_hand.erase(m_hand.begin()+i);	
			}
		}
	}
}

int Joueur::renovation(){
	size_t index;
	int cost;
	std::cout << "Vous devez jeter une carte. Entrez l'index de la carte que vous souhaitez jeter : " << std::endl;
	std::cin >> index;		
	while(index >= m_hand.size()){
		std::cout << "Index invalide. Veuillez entrer un index valide." << std::endl;
		std::cin >> index;
	}
	cost = m_hand.at(index)->getCost();
	jeterCarte(index);
	return cost;
}

void Joueur::defausseCarteDeck(size_t index){
	if (index < m_deck.size()) {
        	m_defausse.push_back(m_deck.at(index));
        	m_deck.erase(m_deck.begin() + index);
    	} else {
        	std::cerr << "Index invalide pour defausseCarteDeck !" << std::endl;
    	}
}

void Joueur::vassal(Plateau &plat, Jeu &j){
	if (m_deck.empty()) {
        	std::cout << "Votre deck est vide, rien à défausser !" << std::endl;
        	return;
    	}
    	
	std::string res;
	Carte* c = m_deck.front();
    	c->printCard();
    	
	if (c->getType() == TypeCarte::Action){
		std::cout << "Voulez-vous jouer cette carte Action ? Répondez par 'Oui' ou 'oui' ou 'O' ou 'o' si vous le souhaitez." << std::endl;
			std::cin >> res;
			if(res == "O" or res == "o" or res == "Oui" or res == "oui"){
				dynamic_cast<CarteAction*>(c)->play(*this, plat, 0, j);
            			return;	
			}
	} 
	defausseCarteDeck(0);
}

void Joueur::putDeckInDefausse(){
  std::string rep;
  bool fin = false;
  std::cout << "Voulez-vous défausser tout votre deck ? (Répondez par OUI ou NON)" << std::endl;
  while(!fin){
    std::cin >> rep;
    if(rep == "OUI" || rep == "Oui" || rep == "oui"){
      for(size_t i = 0; i < m_deck.size(); i++){
        m_defausse.push_back(m_deck.at(i));
        m_deck.erase(m_deck.begin());
        assembleDeckDefausse();
      }
      fin = true;
    }
    else if(rep == "NON" || rep == "Non" || rep == "non") fin = true;
  }
}

int Joueur::calculerPoints(){
  assembleDeckDefausse();
  int score = 0;
  for(size_t i = 0; i < m_deck.size(); i++){
    if(m_deck.at(i)->getType() == TypeCarte::Victoire){
      CarteVictoire* c = dynamic_cast<CarteVictoire*>(m_deck.at(i));
      if(c) score += c->getWinPoints();
    }
    else if(m_deck.at(i)->getName() == "Jardins")score += m_deck.size()/10;
  }
  return score;
}
