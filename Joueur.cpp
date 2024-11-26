#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>
#include <sstream>
#include "Joueur.h"
#include "Jeu.h"
#include <ncurses.h>
#include <string>
#include <vector>

//Constructeur
Joueur::Joueur(std::string pseudo) : m_pseudo(pseudo) {
  m_nb_win_points = 0;
  m_coins = 0;
  m_draws = 0;
  m_nb_actions = 0;
  m_nb_buys = 0;
  m_hand = {};
  m_deck = {};
  m_defausse = {};
  m_rebut = {};
}

//Destructeur
Joueur::~Joueur() {}

// Ajouts de ressources au joueur
void Joueur::addBuys(int buys){m_nb_buys += buys;}
void Joueur::addActions(int actions){m_nb_actions += actions;}
void Joueur::addCoins(int coins){m_coins += coins;}
void Joueur::addDraws(int draws){m_draws += draws;}
void Joueur::addWinPoints(int win_points){m_nb_win_points += win_points;}

// Getters d'attributs simples
int Joueur::getCoins(){	return m_coins;}
int Joueur::getNbActions(){ return m_nb_actions;}
int Joueur::getNbBuys(){ return m_nb_buys;}
int Joueur::getNbWinPoints(){ return m_nb_win_points;}
int Joueur::getDraws(){ return m_draws;}
std::string Joueur::getPseudo(){ return m_pseudo;}

// Getters de vector
std::vector<Carte*>& Joueur::getHand(){ return m_hand;}
std::vector<Carte*>& Joueur::getDeck(){ return m_deck;}
std::vector<Carte*>& Joueur::getDefausse(){ return m_defausse;}
std::vector<Carte*>& Joueur::getRebut(){ return m_rebut;}

template <typename T>
void Joueur::addCardsByNameInDeck(const std::string& name, int nb,std::vector<T>& cardList, int winPoints) {
    auto point = std::find_if(cardList.begin(), cardList.end(), [&](T& c) {
        return c.getName() == name;
    });
    if (point != cardList.end()) {
        Carte* c = &(*point); // Récupère un pointeur vers la carte
        for (int i = 0; i < nb; ++i) {
            m_deck.push_back(c);
            addWinPoints(winPoints);
        }
    } else {
        std::cerr << "Carte " << name << " non trouvée dans la liste.\n";
    }
}


// Méthode pour initialiser le deck du joueur
void Joueur::initDeck(Plateau& p) {
    m_deck.clear();
    addCardsByNameInDeck("Cuivre", 7, p.getListeCarteTresor());
    addCardsByNameInDeck("Domaine", 3, p.getListeCarteVictoire(), 1);
    shuffleCartes(m_deck);
}

void Joueur::shuffleCartes(std::vector<Carte*>& v){
	unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	shuffle(v.begin(), v.end(), std::default_random_engine(seed));
}

// TODO Méthode à changer pour plus joli avec index
/*
void Joueur::printCards(const std::vector<Carte*>& cards, const std::string& name) const{
  std::cout << "======== " << name << " " << m_pseudo << " ========\n\n";
  for(size_t i = 0; i < cards.size(); i++){
    std::cout << '[' << i << ']';
    cards.at(i)->printCard();
    std::cout << "-----------------------------------\n";
  }
}
*/


// Méthode pour afficher les cartes d'un joueur
/*
void Joueur::printCards(const std::vector<Carte*>& cards, const std::string& name) const {
    initscr();
    start_color();
    cbreak();
    noecho();

    // Initialisation des couleurs pour les cartes
    init_pair(1, COLOR_YELLOW, COLOR_BLACK); // Trésor
    init_pair(2, COLOR_GREEN, COLOR_BLACK);  // Victoire
    init_pair(3, COLOR_RED, COLOR_BLACK);    // Action

    clear();

    // Dimensions des cartes
    const int card_width = 20;  // Largeur de la carte
    const int card_height = 7;  // Hauteur de la carte
    const int spacing = 4;      // Espacement entre les cartes
    int cards_per_line = std::min(5, COLS / (card_width + spacing)); // Cartes par ligne
    int total_lines = (cards.size() + cards_per_line - 1) / cards_per_line; // Nombre total de lignes nécessaires

    // Calcul de la position verticale pour centrer les cartes
    int total_height = total_lines * (card_height + spacing) + 2; // Hauteur totale (lignes de cartes + espacement + titre)
    int start_y = (LINES - total_height) / 2; // Centrer verticalement

    // Calcul de la position horizontale pour centrer le titre
    std::string title = "======== " + name + " : " + m_pseudo + " ========";
    int title_x = (COLS - title.size()) / 2;
    mvprintw(start_y, title_x, "%s", title.c_str());

    // Calcul de la position horizontale pour centrer les cartes
    int start_x = (COLS - (cards_per_line * card_width + (cards_per_line - 1) * spacing)) / 2;

    // Dessiner les cartes
    auto draw_card = [&](int y, int x, Carte* card, int index) {
        // Déterminer la couleur de la carte en fonction de son type
        int color_pair = 1;
        switch (card->getType()) {
            case TypeCarte::Victoire: color_pair = 2; break;
            case TypeCarte::Action: color_pair = 3; break;
            default: break; // Trésor reste 1
        }

        attron(COLOR_PAIR(color_pair));

        // Dessiner la bordure
        mvhline(y, x, '-', card_width);
        mvhline(y + card_height - 1, x, '-', card_width);
        mvvline(y, x, '|', card_height);
        mvvline(y, x + card_width - 1, '|', card_height);
        mvaddch(y, x, '+');
        mvaddch(y, x + card_width - 1, '+');
        mvaddch(y + card_height - 1, x, '+');
        mvaddch(y + card_height - 1, x + card_width - 1, '+');

        // Ajouter les informations de la carte
        mvprintw(y + 1, x + 2, "Nom: %s", card->getName().c_str());
        mvprintw(y + 2, x + 2, "Coût: %d", card->getCost());
        mvprintw(y + 3, x + 2, "Type: %s", 
            card->getType() == TypeCarte::Victoire ? "Victoire" : 
            card->getType() == TypeCarte::Action ? "Action" : "Tresor");

        // Centrer l'index sur la dernière ligne
        std::string index_str = std::to_string(index + 1); // Passer à une numérotation de 1
        int index_x = x + (card_width - index_str.size()) / 2; // Centrer l'index
        mvprintw(y + card_height - 2, index_x, "%s", index_str.c_str());

        attroff(COLOR_PAIR(color_pair));
    };

    // Variables pour le positionnement
    int current_x = start_x;
    int current_y = start_y + 2; // Position après le titre

    for (size_t i = 0; i < cards.size(); i++) {
        draw_card(current_y, current_x, cards[i], i);

        // Passer à la carte suivante
        current_x += card_width + spacing;

        // Retour à la ligne si nécessaire
        if ((i + 1) % cards_per_line == 0) {
            current_x = start_x;
            current_y += card_height + spacing;
        }
    }

    // Pause pour visualisation
    mvprintw(LINES - 1, 0, "Appuyez sur une touche pour continuer...");
    refresh();
    getch();

    endwin();
}
*/

void Joueur::printCards(const std::vector<Carte*>& cards, const std::string& name) const {
    initscr();
    start_color();
    cbreak();
    noecho();
    keypad(stdscr, TRUE); // Activer la prise en charge des touches spéciales comme TAB

    // Initialisation des couleurs pour les cartes
    init_pair(1, COLOR_YELLOW, COLOR_BLACK); // Trésor
    init_pair(2, COLOR_GREEN, COLOR_BLACK);  // Victoire
    init_pair(3, COLOR_RED, COLOR_BLACK);    // Action

    while (true) {
        clear();

        // Dimensions des cartes
        const int card_width = 28;  // Largeur de la carte
        const int card_height = 8;  // Hauteur de la carte
        const int spacing = 4;      // Espacement entre les cartes
        int cards_per_line = std::min(5, COLS / (card_width + spacing)); // Cartes par ligne
        int total_lines = (cards.size() + cards_per_line - 1) / cards_per_line; // Nombre total de lignes nécessaires

        // Calcul de la position verticale pour centrer les cartes
        int total_height = total_lines * (card_height + spacing) + 2; // Hauteur totale (lignes de cartes + espacement + titre)
        int start_y = (LINES - total_height) / 2; // Centrer verticalement

        // Calcul de la position horizontale pour centrer le titre
        std::string title = "======== " + name + " : " + m_pseudo + " ========";
        int title_x = (COLS - title.size()) / 2;
        mvprintw(start_y, title_x, "%s", title.c_str());

        // Calcul de la position horizontale pour centrer les cartes
        int start_x = (COLS - (cards_per_line * card_width + (cards_per_line - 1) * spacing)) / 2;

        // Dessiner les cartes
        auto draw_card = [&](int y, int x, Carte* card, int index) {
            // Déterminer la couleur de la carte en fonction de son type
            int color_pair = 1;
            switch (card->getType()) {
                case TypeCarte::Victoire: color_pair = 2; break;
                case TypeCarte::Action: color_pair = 3; break;
                default: break; // Trésor reste 1
            }

            attron(COLOR_PAIR(color_pair));

            // Dessiner la bordure
            mvhline(y, x, '-', card_width);
            mvhline(y + card_height - 1, x, '-', card_width);
            mvvline(y, x, '|', card_height);
            mvvline(y, x + card_width - 1, '|', card_height);
            mvaddch(y, x, '+');
            mvaddch(y, x + card_width - 1, '+');
            mvaddch(y + card_height - 1, x, '+');
            mvaddch(y + card_height - 1, x + card_width - 1, '+');

            // Ajouter les informations de la carte
            mvprintw(y + 1, x + 2, "Nom: %s", card->getName().c_str());
            mvprintw(y + 2, x + 2, "Coût: %d", card->getCost());
            mvprintw(y + 3, x + 2, "Type: %s", 
                card->getType() == TypeCarte::Victoire ? "Victoire" : 
                card->getType() == TypeCarte::Action ? "Action" : "Tresor");

            // Centrer l'index sur la dernière ligne
            std::string index_str = std::to_string(index); // Passer à une numérotation de 1
            int index_x = x + (card_width - index_str.size()) / 2; // Centrer l'index
            mvprintw(y + card_height - 2, index_x, "%s", index_str.c_str());

            attroff(COLOR_PAIR(color_pair));
        };

        // Variables pour le positionnement
        int current_x = start_x;
        int current_y = start_y + 2; // Position après le titre

        for (size_t i = 0; i < cards.size(); i++) {
            draw_card(current_y, current_x, cards[i], i);

            // Passer à la carte suivante
            current_x += card_width + spacing;

            // Retour à la ligne si nécessaire
            if ((i + 1) % cards_per_line == 0) {
                current_x = start_x;
                current_y += card_height + spacing;
            }
        }

        // Indications pour l'utilisateur
        mvprintw(LINES - 2, 0, "Appuyez sur TAB pour voir une carte en détail, ou sur 'q' pour quitter.");

        // Rafraîchir l'écran
        refresh();

        // Attendre l'entrée utilisateur
        int ch = getch();

        if (ch == 'q') {
            break; // Quitter
        } else if (ch == '\t') { // Touche TAB
            // Demander l'index de la carte à afficher
            clear();
            mvprintw(LINES / 2 - 1, (COLS - 40) / 2, "Entrez l'index de la carte (0-%zu): ", cards.size()-1);
            refresh();

            int index = -1;
            echo(); // Activer l'affichage de l'entrée utilisateur
            scanw("%d", &index);
            noecho(); // Désactiver l'affichage de l'entrée utilisateur

            // Valider l'index
            if (index >= 0 && index < static_cast<int>(cards.size())) {
                // Afficher la carte choisie
                clear();
                cards[index]->printCard(); // Appelle printCard propre au type
                mvprintw(LINES - 2, 0, "Appuyez sur une touche pour revenir...");
                refresh();
                getch(); // Attendre une touche pour revenir
            } else {
                mvprintw(LINES - 2, 0, "Index invalide. Appuyez sur une touche pour continuer...");
                refresh();
                getch();
            }
        }
    }

    endwin();
}



void Joueur::printHand() const{ printCards(m_hand, "Main");}
void Joueur::printDefausse() const{ printCards(m_defausse, "Défausse");}
void Joueur::printDeck() const{ printCards(m_deck, "Deck");}
void Joueur::printRebut() const{ printCards(m_rebut, "Rébut");}

void Joueur::transferCards(std::vector<Carte*>& source, std::vector<Carte*>& destination, size_t nb) {
  nb = std::min(nb, source.size());
  destination.insert(destination.end(), source.begin(), source.begin() + nb);
  source.erase(source.begin(), source.begin() + nb);
}

void Joueur::assembleDeckDefausse(){
  shuffleCartes(m_defausse);
  transferCards(m_defausse, m_deck, m_defausse.size());
}

void Joueur::makeHand(){
  if(m_deck.size() < 5) assembleDeckDefausse();
  transferCards(m_deck, m_hand, 5);
}

void Joueur::defausser(){ transferCards(m_hand, m_defausse, m_hand.size()); }

void Joueur::defausseCarte(int index){
  m_defausse.push_back(m_hand.at(index));
  m_hand.erase(m_hand.begin()+index);
}

void Joueur::initNouveauTour(){
  m_coins = 0;
  m_nb_actions = 1;
  m_nb_buys = 1;
}

void Joueur::buyCard(int index, Plateau &plat){
  if(plat.getVide(index)) throw std::runtime_error("Pile vide, impossible d'acheter une carte.");
  else{
        Carte* c = plat.buyCard(index);
        m_coins -= c->getCost();
        m_nb_buys -= 1;
        m_defausse.push_back(c);
  }
}

void Joueur::receiveCard(int n, Plateau &plat){
  std::vector<std::pair<Carte*, int>> liste = plat.getMax(n);
  std::vector<Carte*> listeAffichee;
  for(size_t i = 0; i < liste.size(); i++){
    listeAffichee.push_back(liste.at(i).first);
  }
  std::cout << "Quelle carte voulez-vous parmi les suivantes : " << std::endl;
  size_t index = liste.size();
  while(index >= liste.size()){
    printCards(listeAffichee, "Cartes disponibles");
    try{
      std:: cin >> index;
    }
    catch(...){
      std::cerr << "Vous n'avez pas rentré un chiffre" << std::endl;
    }
  }
  Carte* c = plat.buyCard(liste.at(index).second);
  m_defausse.push_back(c);
}

void Joueur::throwMax(int n, int& index){
  std:: cout << "Vous pouvez jeter jusqu'à " << n << " cartes de votre main" << std::endl;
  std::string rep;
  bool stop = false;
  for(int i = 0; i < n; i++){
    printHand();
    while(!stop){
      std:: cout << "Quelle carte voulez-vous jeter ? (Ecrivez l'index de la carte ou STOP si vous ne voulez plus en jeter)" << std::endl;
      std::cin >> rep;
      if(rep == "STOP"){
        stop = true;
        break;
      }
      try{
        std::stoul(rep);
        if(std::stoul(rep) < m_hand.size()){
          jeterCarte(std::stoul(rep));
          if(std::stoi(rep) < index && index != 20) index-=1;
          else if(std::stoi(rep) == index) index = 20;
          break;
        }
      }
      catch(...){
        std::cerr << "Index Invalide" << std::endl;
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
      printHand();
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
	if(taille <= 1){
		assembleDeckDefausse();
		for(size_t j = 0; j < 2; j++){
			temp.push_back(m_deck.at(j));	
		}
	}
	else{
	        for(size_t i = 0; i < 2; i++){
		        temp.push_back(m_deck.at(i));
	        }
	}
        for(size_t i = 0; i < 2; i++){
                m_defausse.push_back(m_deck.at(i));
                m_deck.erase(m_deck.begin()+i);
                size_t taille = m_defausse.size();
	        temp.push_back(m_defausse.at(taille-1));
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
  size_t nb = 0;
  std::string rep;
  bool stop = false;
  while(!stop && m_hand.size() != 0){
      printHand();
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
  if(m_deck.empty()){
    assembleDeckDefausse();
  }
  m_hand.push_back(m_deck.at(0));
  m_deck.erase(m_deck.begin());
}

void Joueur::regarderDefausseEtPrendre(){
  printDefausse(); // Rajouter les index
  std::string rep;
  size_t index;
  std::cout << "Quelle carte voulez vous prendre et mettre sur le dessus de votre Deck ? (Ecrivez l'index de la carte ou sinon autre chose si vous ne voulez pas en prendre)" << std::endl;
  std::cin >> rep;
  try{
    index = std::stoul(rep);
    if(index < m_defausse.size()){
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

std::pair<size_t, std::vector<size_t>> Joueur::getNbCarteHandByType(TypeCarte type) {
    size_t nb = 0;
    std::vector<size_t> indices;
    for (size_t i = 0; i < m_hand.size(); i++) {
        if (m_hand.at(i)->getType() == type) {
            nb++;
            indices.push_back(i);
        }
    }
    return std::make_pair(nb, indices);
}

std::pair<size_t,std::vector<size_t>> Joueur::getNbCarteVictoireHand(){ return getNbCarteHandByType(TypeCarte::Victoire);}
std::pair<size_t,std::vector<size_t>> Joueur::getNbCarteTresorHand(){ return getNbCarteHandByType(TypeCarte::Tresor);}
std::pair<size_t,std::vector<size_t>> Joueur::getNbCarteActionHand(){ return getNbCarteHandByType(TypeCarte::Action);}

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
      printHand();
      Carte* c = m_deck.at(0);
      m_deck.erase(m_deck.begin());
      if(c->getType() == TypeCarte::Action){
        printHand();
        bool valide = false;
        std::string rep;
        while(!valide){
          std::cout << "Vous venez de piocher la carte " << c->getName() << " \nVoulez-vous la garder dans votre main (OUI pour la garder et NON pour la défausser)" << std::endl;
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

void Joueur::receiveTresor(int n, Plateau &plat){
  std::vector<std::pair<Carte*, int>> liste = plat.getMaxTresor(n);
  std::vector<Carte*> listeAffichee;
  for(size_t i = 0; i < liste.size(); i++){
    listeAffichee.push_back(liste.at(i).first);
  }
  std::cout << "Quelle carte voulez-vous parmi les suivantes : " << std::endl;
  size_t index = liste.size();
  while(index >= liste.size()){
    printCards(listeAffichee, "Cartes Disponibles");
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
        	std::cerr << "Index invalide !" << std::endl;
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


void Joueur::sentinelle(){
	std::string rep;
	std::vector<Carte*> temp;
	if(m_deck.size() <= 1) assembleDeckDefausse();
	for(size_t i = 0; i < 2; i++){
		temp.push_back(m_deck.at(i));
		m_deck.at(i)->printCard();
		bool actionChoisie = false;
        	while (!actionChoisie) {
            		std::cout << "Souhaitez-vous jeter et/ou défausser cette carte ?\n"
                      << "Répondez 'JETER' pour jeter, 'DEFAUSSER' pour défausser, ou 'STOP' pour ne rien faire." << std::endl;
            		std::cin >> rep;

            		if (rep == "JETER" || rep == "jeter") {
                		m_rebut.push_back(m_deck.at(i));
                		actionChoisie = true;
            		} else if (rep == "DEFAUSSER" || rep == "defausser") {
                		m_defausse.push_back(m_deck.at(i));
                		actionChoisie = true;
            		} else if (rep == "STOP" || rep == "stop") {
                		actionChoisie = true;
            		} else {
                		std::cout << "Entrée invalide. Veuillez réessayer." << std::endl;
            		}
            	}
	}
	
	for (Carte* c : temp) {
        	auto it = std::find(m_deck.begin(), m_deck.end(), c);
        	if (it != m_deck.end()) {
            		m_deck.erase(it);
        	}
    	}
	
	if (!temp.empty()) {
	        std::cout << "Vous avez décidé de garder certaines cartes. Dans quel ordre voulez-vous les replacer sur le haut de votre deck ?" << std::endl;
        	std::cout << "Indiquez l'ordre des indices des cartes à replacer (0 pour la première carte révélée, 1 pour la deuxième) séparés par un espace." << std::endl;

        	for (size_t i = 0; i < temp.size(); i++) {
            		std::cout << i << ": ";
            		temp.at(i)->printCard();
        	}

        	std::vector<size_t> ordre;
        	while (ordre.size() != temp.size()) {
            		std::cout << "Entrez l'ordre exact (par exemple, '1 0' si vous voulez mettre la deuxième carte en haut) :" << std::endl;
            		ordre.clear();

            		std::string input;
            		std::getline(std::cin >> std::ws, input);
            		std::istringstream ss(input);
            		size_t index;
            		while (ss >> index) {
                		if (index < temp.size() && std::find(ordre.begin(), ordre.end(), index) == ordre.end()) {
                    			ordre.push_back(index);
                		} else {
                    			std::cout << "Indice invalide ou déjà utilisé. Veuillez réessayer." << std::endl;
                    			ordre.clear();
                    			break;
                		}
            		}
        	}
        	for (size_t i = 0; i < ordre.size(); i++) {
            		m_deck.insert(m_deck.begin(), temp.at(ordre.at(i)));
        	}
    	}
    	std::cout << "Les cartes ont été replacées sur le haut de votre deck." << std::endl;
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

bool Joueur::handContainsDouve(){
  for(size_t i = 0; i < m_hand.size(); i++){
    if(m_hand.at(i)->getName() == "Douve")return true;
  }
  return false;
}

std::vector<bool> Joueur::decideDefausse(std::vector<Joueur> listeJoueur){
	std::vector<bool> reponse;
	std::string res;
	for(size_t i = 0; i < listeJoueur.size(); i++){  
		std::cout << "Voulez-vous défausser la carte de " << listeJoueur.at(i).getPseudo() << " ? Répondez par 'OUI' ou par 'NON'. "<< std::endl;
		std::cin >> res;
		if(res == "OUI") reponse.push_back(true);
		if(res == "NON") reponse.push_back(false);
	}
	return reponse;
}

void Joueur::resetDraws(){
  m_draws = 0;
}

bool Joueur::queJardins(){
  for(size_t i = 0; i < m_hand.size(); i++){
    if(m_hand.at(i)->getType() == TypeCarte::Action && m_hand.at(i)->getName() != "Jardins"){
      return false;
    }
  }
  return true;
}
