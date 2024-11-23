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

Plateau::Plateau(int nb_joueurs) { // Constructeur
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

Plateau::~Plateau() {} // Destructeur

// Getters
std::vector<CarteAction>& Plateau::getListeCarteAction(){ return m_listeCarteActionChoisie; }

std::vector<CarteTresor>& Plateau::getListeCarteTresor(){ return listeCarteTresor; }

std::vector<CarteVictoire>& Plateau::getListeCarteVictoire(){ return listeCarteVictoire; }

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

// Méthode pour remplire les listes de cartes
void Plateau::remplirListeCarte(){
	std::vector< std::tuple< std::string, std::string, int, std::string, int, int, int, int, int, bool, bool>> data = loadCard(); // On appelle la fonction pour charger toutes les cartes dans un vector
	for(const auto& t : data){
		if(std::get<0>(t) == "Trésor"){ // Pour les cartes trésor
			CarteTresor c = CarteTresor(std::get<1>(t), std::get<3>(t), std::get<2>(t), std::get<5>(t));
			listeCarteTresor.push_back(c);
		}
		else if(std::get<0>(t) == "Victoire"){ // Pour les cartes victoire
			CarteVictoire c = CarteVictoire(std::get<1>(t), std::get<3>(t), std::get<2>(t), std::get<4>(t));
			listeCarteVictoire.push_back(c);
		}
		else{ // Pour les cartes action
			CarteAction c = CarteAction(std::get<1>(t), std::get<3>(t), std::get<2>(t), std::get<7>(t), std::get<8>(t), std::get<6>(t), std::get<5>(t), std::get<9>(t), std::get<10>(t));
			listeCarteAction.push_back(c);
		}
	}
}

// Méthode pour afficher toutes les cartes (seulement pour vérifier qu'on les avait toutes)
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

// Méthode pour choisir 10 cartes action au hasard parmi la liste de cartes action
void Plateau::choisirCarteActionHasard() {
    m_listeCarteActionChoisie.clear();
    std::vector<int> indices(listeCarteAction.size());
    std::iota(indices.begin(), indices.end(), 0); // Génère la liste de 0 à taille-1

    std::default_random_engine re(std::chrono::system_clock::now().time_since_epoch().count());
    std::shuffle(indices.begin(), indices.end(), re); // Mélange aléatoirement les indices

    for (int i = 0; i < 10; i++) { // Sélectionne les 10 premières cartes des indices mélangés
        m_listeCarteActionChoisie.push_back(listeCarteAction.at(indices[i]));
    }
}

// Méthode pour choisir les 10 cartes action du set de base
void Plateau::choisirCarteActionSetBase(){
  m_listeCarteActionChoisie.clear();
  for(size_t i = 0; i < 10; i++){ // On prend les 10 premières cartes car ce sont les 10 premières qu'on a ajouté dans la liste
    m_listeCarteActionChoisie.push_back(listeCarteAction.at(i));
  }
}

// Méthode pour remplir les piles du plateau suivant le nombre de joueurs
void Plateau::remplirPiles(int nb_joueurs) {
    m_PilesTresor.clear(); 
    m_PilesVictoire.clear();
    m_PilesAction.clear(); // On vide les piles au cas où

    for (auto& carte : listeCarteTresor) { // On parcourt les cartes trésor
        int quantite = (carte.getName() == "Cuivre") ? 60 : // 60 cuivres
                       (carte.getName() == "Argent") ? 40 : 30; // 40 argent et 30 or
        m_PilesTresor.push_back(std::make_pair(carte, quantite)); // On ajoute dans la pile
    }

    int nbVictoire = (nb_joueurs <= 2) ? 8 : 12; // Nb de cartes victoires dans les piles selon le nombre de joueurs
    for (auto& carte : listeCarteVictoire) { // On parcourt les cartes victoire
        int quantite = (carte.getName() == "Malédiction") ? (10 * nb_joueurs - 10) : nbVictoire; // Cas spécial pour les malédictions
        m_PilesVictoire.push_back(std::make_pair(carte, quantite));
    }

    for (auto& carte : m_listeCarteActionChoisie) { // On parcourt les cartes action choisies précédemment
        int quantite = (carte.getName() == "Jardins") ? nbVictoire : 10; // Jardins fonctionne comme les cartes victoires sur le nombre
        m_PilesAction.push_back(std::make_pair(carte, quantite));
    }
}

// Méthode pour afficher le plateau
// TODO A changer avec index et en plus beau
void Plateau::print() const{
  size_t somme = 0;
  std::cout << "======== PLATEAU DOMINION ========\n\n";
    
  std::cout << "========== PILES TRESOR ==========\n";
  for (size_t i = 0; i < m_PilesTresor.size(); i++) {
    std::cout << '[' << somme << ']';
    std::cout << "Quantité: " << m_PilesTresor.at(i).second << " | ";
    m_PilesTresor.at(i).first.printCard();
    std::cout << "-----------------------------------\n";
    somme++;
  }

  std::cout << "\n========== PILES VICTOIRE ==========\n";
  for (size_t i = 0; i < m_PilesVictoire.size(); i++) {
    std::cout << '[' << somme << ']';
    std::cout << "Quantité: " << m_PilesVictoire.at(i).second << " | ";
    m_PilesVictoire.at(i).first.printCard();
    std::cout << "-----------------------------------\n";
    somme++;
  }

  std::cout << "\n========== PILES ACTION ==========\n";
  for (size_t i = 0; i < m_PilesAction.size(); i++) {
    std::cout << '[' << somme << ']';
    std::cout << "Quantité: " << m_PilesAction.at(i).second << " | ";
    m_PilesAction.at(i).first.printCard();
    std::cout << "-----------------------------------\n";
    somme++;
  }

  std::cout << "\n===================================\n";
}

// Méthode pour chercher une carte dans les piles et donner l'index de cette carte dans la pile
template <typename T> // Template pour pouvoir mettre plusieurs types suivant le type de la carte
size_t Plateau::chercherCarte(const std::vector<std::pair<T, int>>& pile, const std::string& name){
  for(size_t i = 0; i < pile.size(); i++){
    if(pile.at(i).first.getName() == name) return i; // On renvoie l'index de la carte dans la pile
  }
  throw std::runtime_error("Carte introuvable : " + name); // Renvoie une erreur si on ne trouve pas la carte cherchée
}

// Méthode pour chercher l'index d'une carte action dans sa pile
size_t Plateau::chercherCarteAction(std::string name){
  return chercherCarte(m_PilesAction, name);
}

// Méthode pour chercher l'index d'une carte trésor dans sa pile
size_t Plateau::chercherCarteTresor(std::string name){
  return chercherCarte(m_PilesTresor, name);
}

// Méthode pour chercher l'index d'une carte victoire dans sa pile
size_t Plateau::chercherCarteVictoire(std::string name){
  return chercherCarte(m_PilesVictoire, name);
}

// Méthode pour retirer un exemplaire d'une carte du plateau
Carte* Plateau::buyCard(size_t index){
  if(index > m_PilesTresor.size() - 1){ // Vérifie si l'index est plus loin que les cartes trésor
    index -= m_PilesTresor.size();
    if(index > m_PilesVictoire.size() - 1){ // Vérifie si l'index est plus loin que les cartes victoire
      index -= m_PilesVictoire.size();
      m_PilesAction.at(index).second -= 1; // On est dans les cartes action
      return &m_PilesAction.at(index).first;
    }
    else{ // On est dans les cartes victoire
      m_PilesVictoire.at(index).second -= 1;
      return &m_PilesVictoire.at(index).first;
    }
  }
  else{ // On est dans les cartes trésor
    m_PilesTresor.at(index).second -= 1;
    return &m_PilesTresor.at(index).first;
  }
}

// Méthode pour récupérer la liste des cartes avec leur index (pour pouvoir les récupérer) qui ont un coût maximal de n
std::vector<std::pair<Carte*, int>> Plateau::getMax(int n){
  int somme = 0;
  std::vector<std::pair<Carte*, int>> max;
  for(size_t i = 0; i < m_PilesTresor.size(); i++){ // Vérifie dans les cartes trésor celles qui valident la condition d'avoir un coût inférieur et où il reste des exemplaires de cette carte
    if(m_PilesTresor.at(i).first.getCost() <= n && m_PilesTresor.at(i).second !=0){
      std::pair<Carte*, int> elem = std::make_pair(&m_PilesTresor.at(i).first, somme);
      max.push_back(elem);
    }
    somme++;
  }
  
  for(size_t i = 0; i < m_PilesVictoire.size(); i++){ // Vérifie dans les cartes victoire
    if(m_PilesVictoire.at(i).first.getCost() <= n && m_PilesVictoire.at(i).second !=0){
      std::pair<Carte*, int> elem = std::make_pair(&m_PilesVictoire.at(i).first, somme);
      max.push_back(elem);
    }
    somme++;
  }
  
  for(size_t i = 0; i < m_PilesAction.size(); i++){ // Vérifie dans les cartes actions
    if(m_PilesAction.at(i).first.getCost() <= n && m_PilesAction.at(i).second !=0){
      std::pair<Carte*, int> elem = std::make_pair(&m_PilesAction.at(i).first, somme);
      max.push_back(elem);
    }
    somme++;
  }
  return max;
}

// Méthode pour récupérer la liste des cartes trésor avec leur index qui ont un coût inférieur à n
std::vector<std::pair<Carte*, int>> Plateau::getMaxTresor(int n){
  int somme = 0;
  std::vector<std::pair<Carte*, int>> max;
  for(size_t i = 0; i < m_PilesTresor.size(); i++){ // Regarde dans les cartes trésor celles qui ont un coût inférieur et dont il reste des exemplaires
    somme++;
    if(m_PilesTresor.at(i).first.getCost() <= n && m_PilesTresor.at(i).second !=0){
      std::pair<Carte*, int> elem = std::make_pair(&m_PilesTresor.at(i).first, somme);
      max.push_back(elem);
    }
  }
  return max;
}

int Plateau::chercherCoutParIndex(size_t index){
  if(index > m_PilesTresor.size() - 1){ // Vérifie si l'index est plus loin que les cartes trésor
    index -= m_PilesTresor.size();
    if(index > m_PilesVictoire.size() - 1){ // Vérifie si l'index est plus loin que les cartes victoire
      index -= m_PilesVictoire.size(); 
      return m_PilesAction.at(index).first.getCost(); // On est dans les cartes action
    }
    else{ // On est dans les cartes victoire
      return m_PilesVictoire.at(index).first.getCost();
    }
  }
  else{ // On est dans les cartes trésor
    return m_PilesTresor.at(index).first.getCost();
  }
}

