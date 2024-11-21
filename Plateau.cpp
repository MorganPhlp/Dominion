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
std::vector<CarteAction> m_listeCarteActionChoisie;
std::vector<std::pair<CarteAction, int>> m_PilesAction;
std::vector<std::pair<CarteTresor, int>> m_PilesTresor;
std::vector<std::pair<CarteVictoire, int>> m_PilesVictoire;
std::vector<std::vector<Carte*>> m_listeCartesDevoilees;
std::vector<Carte*> m_listeCartesEcartees;
const size_t Plateau::m_maxIndex = 17;

Plateau::Plateau() {} // Constructeur
// TODO Peut-être remplacer le init et le mettre dans le constructeur

Plateau::~Plateau() {} // Destructeur

// Getters
std::vector<CarteAction> Plateau::getListeCarteAction() const{ return m_listeCarteActionChoisie; }

std::vector<CarteTresor> Plateau::getListeCarteTresor() const{ return listeCarteTresor; }

std::vector<CarteVictoire> Plateau::getListeCarteVictoire() const{ return listeCarteVictoire; }

std::vector<std::pair<CarteAction, int>> Plateau::getPilesAction() {return m_PilesAction;}

std::vector<std::pair<CarteTresor, int>> Plateau::getPilesTresor() {return m_PilesTresor;}

std::vector<std::pair<CarteVictoire, int>> Plateau::getPilesVictoire() {return m_PilesVictoire;}

size_t Plateau::getMaxIndex () const{return m_maxIndex;}

std::vector<std::vector<Carte*>>& Plateau::getListeCartesDevoilees(){return m_listeCartesDevoilees;}

std::vector<Carte*>& Plateau::getListeCartesEcartees(){return m_listeCartesEcartees;}

// Méthode pour lire un int dans un string stream
int Plateau::lireInt(std::stringstream& ss) {
    std::string col;
    std::getline(ss, col, ',');
    return col.empty() ? 0 : std::stoi(col); // Si la colonne est vide on met 0 sinon la valeur
}

// Méthode pour lire un booléen dans un string stream
bool Plateau::lireBool(std::stringstream& ss) {
    std::string col;
    std::getline(ss, col, ',');
    return col == "1";
}

// Méthode pour lire un string dans un string stream mais qui est entreGuillemets à cause des virgules contenues dans ce string
std::string Plateau::lireGuillemets(std::stringstream& ss) {
    std::string col;
    bool entreGuillemets = false;
    char c;
    while (ss.get(c)) { // Tant qu'il reste des caractères dans ce string stream
        if (c == '"' && !entreGuillemets) { // Cas où on entre dans les guillemets
            entreGuillemets = true;
        } else if (c == '"' && entreGuillemets) { // Cas où on sort des guillemets
            entreGuillemets = false;
        } else if (c == ',' && !entreGuillemets) { // Cas où on a la virgule de fin de champ
            break;
        } else { // Quand on est dans les guillemets on renvoie tous les caractères
            col.push_back(c);
        }
    }
    return col;
}

// Méthode pour charger toutes les cartes avec tous leurs paramètres depuis le fichier csv contenu dans le même dossier
std::vector< std::tuple< std::string, std::string, int, std::string, int, int, int, int, int, bool, bool>> Plateau::loadCard(){
  std::vector< std::tuple< std::string, std::string, int, std::string, int, int, int, int, int, bool, bool>> data;
  std::ifstream file("ExcelActionCards.csv"); // Récupère le fichier csv
  std::string line;
	
  if (!file.is_open()) { // Si le fichier ne peut pas être ouvert alors on s'arrête
    std::cerr << "Impossible d'ouvrir le fichier donné" << std::endl;
    return data;
  }
  
  std::getline(file, line); //Pour ignorer en-tête
  while(std::getline(file, line)){ // Tant qu'il reste des lignes dans le fichier on continue et on la met dans la variable line
    std::stringstream ss(line); // transforme la ligne en stringstream pour pouvoir la segmenter plus facilement
    std::string name, description, cardType;
    int cost = 0, win_points = 0, coins = 0, draws = 0, actions = 0, buys = 0;
    bool attack = false, reaction = false;
		
    try{
      std::getline(ss, cardType, ',');
      std::getline(ss, name, ',');
      cost = lireInt(ss);
      description = lireGuillemets(ss);
      win_points = lireInt(ss);
      coins = lireInt(ss);
      draws = lireInt(ss);
      actions = lireInt(ss);
      buys = lireInt(ss);
      attack = lireBool(ss);
      reaction = lireBool(ss);
				
      data.emplace_back(cardType, name, cost, description, win_points, coins, draws, actions, buys, attack, reaction); // Ajoute un nouvel objet à la fin de data
    } catch (const std::exception& e) { // Vérifie les erreurs comme par exemple des erreurs de conversion avec std::stoi
      std::cerr << "Erreur dans la ligne : " << line << "\nException" << e.what() << std::endl;
    }
  }
  file.close(); // Fermer le fichier
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

void Plateau::choisirCarteActionHasard() {
    m_listeCarteActionChoisie.clear(); // Vide la liste au cas où elle contiendrait déjà des cartes
    std::vector<int> indices(listeCarteAction.size());
    std::iota(indices.begin(), indices.end(), 0); // Génère la liste de 0 à taille-1

    // Mélange aléatoirement les indices
    std::default_random_engine re(std::chrono::system_clock::now().time_since_epoch().count());
    std::shuffle(indices.begin(), indices.end(), re);

    // Sélectionne les 10 premières cartes des indices mélangés
    for (int i = 0; i < 10; i++) {
        m_listeCarteActionChoisie.push_back(listeCarteAction.at(indices[i]));
    }
}

void Plateau::choisirCarteActionSetBase(){
  m_listeCarteActionChoisie = {};
  for(size_t i = 0; i < 10; i++){
    m_listeCarteActionChoisie.push_back(listeCarteAction.at(i));
  }
}

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
    	
    	for(size_t i = 0; i < m_listeCarteActionChoisie.size(); i++){
    		CarteAction c = m_listeCarteActionChoisie.at(i);
    		if(c.getName() == "Jardins") m_PilesAction.push_back(std::make_pair(c, nb_joueurs));
    		else m_PilesAction.push_back(std::make_pair(c, 10));
    	}
}


void Plateau::init(int nb_joueurs){
	remplirListeCarte();
	size_t rep;
	std::cout << "Voulez-vous jouer avec un set de cartes Action au hasard ou avec le set de base ? (Répondez 1 ou 2 suivant la réponse)"<<std::endl;
	while(rep!=1 && rep != 2){
	  std::cin >> rep;
	}
	if(rep == 1) choisirCarteActionHasard();
	else choisirCarteActionSetBase();
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

// TODO Remplacer ces 3 méthodes par une méthode générique
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

// TODO Remplacer ces 2 méthodes par des méthodes génériques
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

std::vector<std::pair<Carte*, int>> Plateau::getMaxTresor(int n){
  int somme = 0;
  std::vector<std::pair<Carte*, int>> max;
  for(size_t i = 0; i < m_PilesTresor.size(); i++){
    somme++;
    if(m_PilesTresor.at(i).first.getCost() <= n && m_PilesTresor.at(i).second !=0){
      std::pair<Carte*, int> elem = std::make_pair(&m_PilesTresor.at(i).first, somme);
      max.push_back(elem);
    }
  }
  return max;
}
