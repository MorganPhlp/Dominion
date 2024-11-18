#include "Plateau.h"
#include <random>
#include <chrono>
#include <functional>
#include <tuple>
#include <fstream>
#include <sstream>

std::vector<CarteTresor> Plateau::listeCarteTresor;
std::vector<CarteAction> Plateau::listeCarteAction;
std::vector<CarteVictoire> Plateau::listeCarteVictoire;
std::vector<CarteAction> m_listeCarteActionHasard;
std::vector<std::pair<CarteAction, int>> m_PilesAction;
std::vector<std::pair<CarteTresor, int>> m_PilesTresor;
std::vector<std::pair<CarteVictoire, int>> m_PilesVictoire;
std::vector<std::vector<Carte*>> m_listeCartesDevoilees;
std::vector<Carte*> m_listeCartesEcartees;
const size_t Plateau::m_maxIndex = 17;

Plateau::Plateau() {}

Plateau::~Plateau() {}

std::vector< std::tuple< std::string, std::string, int, std::string, int, int, int, int, int, bool, bool>> Plateau::loadCard(){
	std::vector< std::tuple< std::string, std::string, int, std::string, int, int, int, int, int, bool, bool>> data;
	std::ifstream file("ExcelActionCards.csv");
	std::string line;
	
	if(file.is_open()){
		std::getline(file, line); //Pour ignorer en-tête
		
		while(std::getline(file, line)){
			std::stringstream ss(line);
			std::string name, description, cardType, col;
			int cost, win_points = 0, coins = 0, draws = 0, actions = 0, buys = 0;
			bool attack = false, reaction = false;
			
			try{
				std::getline(ss, cardType, ',');
				std::getline(ss, name, ',');
				
				std::getline(ss, col, ',');
				cost = col.empty() ? 0 : std::stoi(col);
				
				bool entreGuillemets = false;
				char c;
				while(ss.get(c)){
					if(c == '"' && !entreGuillemets){
						entreGuillemets = true;
					}
					else if(c == '"' && entreGuillemets){
						entreGuillemets = false;
					}
					else if(c == ',' && !entreGuillemets){
						break;
					}
					else description.push_back(c);
				}
				
				
				std::getline(ss, col, ',');
				win_points = col.empty() ? 0 : std::stoi(col);
				std::getline(ss, col, ',');
				coins = col.empty() ? 0 : std::stoi(col);
				std::getline(ss, col, ',');
				draws = col.empty() ? 0 : std::stoi(col);
				std::getline(ss, col, ',');
				actions = col.empty() ? 0 : std::stoi(col);
				std::getline(ss, col, ',');
				buys = col.empty() ? 0 : std::stoi(col);
				
				std::getline(ss, col, ',');
				attack = (col == "1");
				std::getline(ss, col, ',');
				reaction = (col == "1");
				
				std::tuple< std::string, std::string, int, std::string, int, int, int, int, int, bool, bool> t {cardType, name, cost, description, win_points, coins, draws, actions, buys, attack, reaction};
				data.push_back(t);
			} catch (const std::invalid_argument& e) {
                		std::cerr << "Erreur de conversion dans la ligne : " << line << "\n";
                		std::cerr << "Exception: " << e.what() << std::endl;
			} catch (const std::out_of_range& e) {
				std::cerr << "Valeur hors limite dans la ligne : " << line << "\n";
				std::cerr << "Exception: " << e.what() << std::endl;
			}
			
		}
		file.close();
	} else{
		std::cerr << "Impossible d'ouvrir le fichier donné" << std::endl;
	}
	return data;
}

void Plateau::remplirListeCarte(){
	std::vector< std::tuple< std::string, std::string, int, std::string, int, int, int, int, int, bool, bool>> data = loadCard();
	for(const auto& t : data){
		if(std::get<0>(t) == "Trésor"){
			CarteTresor c = CarteTresor(std::get<1>(t), std::get<3>(t), std::get<2>(t), std::get<5>(t));
			listeCarteTresor.push_back(c);
		}
		else if(std::get<0>(t) == "Victoire"){
			CarteVictoire c = CarteVictoire(std::get<1>(t), std::get<3>(t), std::get<2>(t), std::get<4>(t));
			listeCarteVictoire.push_back(c);
		}
		else{
			CarteAction c = CarteAction(std::get<1>(t), std::get<3>(t), std::get<2>(t), std::get<7>(t), std::get<8>(t), std::get<6>(t), std::get<5>(t), std::get<9>(t), std::get<10>(t));
			listeCarteAction.push_back(c);
		}
	}
}


void Plateau::printTotalCard(){
    std::cout << "========== CARTES TRESOR ==========" << std::endl;
    for (const CarteTresor& carte : listeCarteTresor) {
        carte.printCard();
        std::cout << "-----------------------------------" << std::endl;
    }

    std::cout << "\n========== CARTES VICTOIRE ==========" << std::endl;
    for (const CarteVictoire& carte : listeCarteVictoire) {
        carte.printCard();
        std::cout << "-----------------------------------" << std::endl;
    }

    std::cout << "\n========== CARTES ACTION ==========" << std::endl;
    for (const CarteAction& carte : listeCarteAction) {
        carte.printCard();
        std::cout << "-----------------------------------" << std::endl;
    }
}


void Plateau::choisirCarteActionHasard(){
	m_listeCarteActionHasard = {};
	std::vector<int> nb;
	
	std::default_random_engine re(std::chrono::system_clock::now().time_since_epoch().count());
	int taille = listeCarteAction.size();
	std::uniform_int_distribution<int> distrib{0, taille-1};
	auto rd = bind(distrib, re);
	
	for(int i = 0; i < 10; i++){
		int alea = rd();
		bool present = false;
		for(size_t j = 0; j < nb.size(); j++){
			if(nb.at(j) == alea){
				present = true;
				i-=1;
				break;
			}
		}
		if(!present){
			m_listeCarteActionHasard.push_back(listeCarteAction.at(alea));
		}
	}
}

std::vector<CarteAction> Plateau::getListeCarteAction() const{ return m_listeCarteActionHasard; }

std::vector<CarteTresor> Plateau::getListeCarteTresor() const{ return listeCarteTresor; }

std::vector<CarteVictoire> Plateau::getListeCarteVictoire() const{ return listeCarteVictoire; }


void Plateau::remplirPiles(int nb_joueurs){
	m_PilesAction = {};
	m_PilesTresor = {};
	m_PilesVictoire = {};
	
	for(size_t i = 0; i < listeCarteTresor.size(); i++){
		CarteTresor c = listeCarteTresor.at(i);
		if(c.getName() == "Cuivre") m_PilesTresor.push_back(std::make_pair(c, 60));
		else if(c.getName() == "Argent") m_PilesTresor.push_back(std::make_pair(c, 40));
		else if(c.getName() == "Or") m_PilesTresor.push_back(std::make_pair(c, 30));
	}
	
	int nbVictoire = 8;
	if(nb_joueurs != 2) nbVictoire = 12;
	
	for (size_t i = 0; i < listeCarteVictoire.size(); i++) {
        	CarteVictoire c = listeCarteVictoire.at(i);
        	if(c.getName() == "Domaine") m_PilesVictoire.push_back(std::make_pair(c, nbVictoire));
        	else if(c.getName() == "Duché") m_PilesVictoire.push_back(std::make_pair(c, nbVictoire));
    		else if(c.getName() == "Province") m_PilesVictoire.push_back(std::make_pair(c, nbVictoire));
    		else if(c.getName() == "Malédiction") m_PilesVictoire.push_back(std::make_pair(c, (10*nb_joueurs) - 10));
    	}
    	
    	for(size_t i = 0; i < m_listeCarteActionHasard.size(); i++){
    		CarteAction c = m_listeCarteActionHasard.at(i);
    		if(c.getName() == "Jardins") m_PilesAction.push_back(std::make_pair(c, nb_joueurs));
    		else m_PilesAction.push_back(std::make_pair(c, 10));
    	}
}


void Plateau::init(int nb_joueurs){
	remplirListeCarte();
	choisirCarteActionHasard();
	remplirPiles(nb_joueurs);
}


void Plateau::print() const{
    std::cout << "======== PLATEAU DOMINION ========\n\n";
    
    std::cout << "========== PILES TRESOR ==========\n";
    for (size_t i = 0; i < m_PilesTresor.size(); i++) {
        std::cout << "Quantité: " << m_PilesTresor.at(i).second << " | ";
        m_PilesTresor.at(i).first.printCard();
        std::cout << "-----------------------------------\n";
    }

    std::cout << "\n========== PILES VICTOIRE ==========\n";
    for (size_t i = 0; i < m_PilesVictoire.size(); i++) {
        std::cout << "Quantité: " << m_PilesVictoire.at(i).second << " | ";
        m_PilesVictoire.at(i).first.printCard();
        std::cout << "-----------------------------------\n";
    }

    std::cout << "\n========== PILES ACTION ==========\n";
    for (size_t i = 0; i < m_PilesAction.size(); i++) {
        std::cout << "Quantité: " << m_PilesAction.at(i).second << " | ";
        m_PilesAction.at(i).first.printCard();
        std::cout << "-----------------------------------\n";
    }

    std::cout << "\n===================================\n";
}


int Plateau::chercherCarteAction(std::string name){
  for(size_t i = 0; i < m_PilesAction.size(); i++){
    if(m_PilesAction.at(i).first.getName() == name) return i;
  }
  throw std::runtime_error("Carte introuvable : " + name);
}

int Plateau::chercherCarteTresor(std::string name){
  for(size_t i = 0; i < m_PilesTresor.size(); i++){
    if(m_PilesTresor.at(i).first.getName() == name) return i;
  }
  throw std::runtime_error("Carte introuvable : " + name);
}

int Plateau::chercherCarteVictoire(std::string name){
  for(size_t i = 0; i < m_PilesVictoire.size(); i++){
    if(m_PilesVictoire.at(i).first.getName() == name) return i;
  }
  throw std::runtime_error("Carte introuvable : " + name);
}


Carte* Plateau::buyCard(int index){
  size_t i = static_cast<size_t>(index);
  if(i > m_PilesTresor.size() - 1){
    i -= m_PilesTresor.size() - 1;
    if(i > m_PilesVictoire.size() - 1){
      i -= m_PilesVictoire.size() - 1;
      m_PilesAction.at(i).second -= 1;
      return &m_PilesAction.at(i).first;
    }
    else{
      m_PilesVictoire.at(i).second -= 1;
      return &m_PilesVictoire.at(i).first;
    }
  }
  else{
    m_PilesTresor.at(i).second -= 1;
    return &m_PilesTresor.at(i).first;
  }
}


std::vector<std::pair<Carte*, int>> Plateau::getMax(int n){
  int somme = 0;
  std::vector<std::pair<Carte*, int>> max;
  for(size_t i = 0; i < m_PilesTresor.size(); i++){
    somme++;
    if(m_PilesTresor.at(i).first.getCost() <= n && m_PilesTresor.at(i).second !=0){
      std::pair<Carte*, int> elem = std::make_pair(&m_PilesTresor.at(i).first, somme);
      max.push_back(elem);
    }
  }
  
  for(size_t i = 0; i < m_PilesVictoire.size(); i++){
    somme++;
    if(m_PilesVictoire.at(i).first.getCost() <= n && m_PilesVictoire.at(i).second !=0){
      std::pair<Carte*, int> elem = std::make_pair(&m_PilesVictoire.at(i).first, somme);
      max.push_back(elem);
    }
  }
  
  for(size_t i = 0; i < m_PilesAction.size(); i++){
    somme++;
    if(m_PilesAction.at(i).first.getCost() <= n && m_PilesAction.at(i).second !=0){
      std::pair<Carte*, int> elem = std::make_pair(&m_PilesAction.at(i).first, somme);
      max.push_back(elem);
    }
  }
  return max;
}

size_t Plateau::getMaxIndex () const{return m_maxIndex;}

//std::vector<std::vector<Carte*>> Plateau::getListeCartesDevoilees(){return m_listeCartesDevoilees};

//std::vector<Carte*> Plateau::getListeCartesEcartees(){return m_listeCartesEcartees};


