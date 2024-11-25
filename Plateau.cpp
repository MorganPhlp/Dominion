#include "Plateau.h"
#include <random>
#include <chrono>
#include <functional>
#include <tuple>
#include <fstream>
#include <sstream>
#include <ncurses.h>

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

/*
// Méthode pour afficher toutes les cartes (seulement pour vérifier qu'on les avait toutes)
void Plateau::printTotalCard(){
    int index = 0;
    std::cout << "========== CARTES TRESOR ==========" << std::endl;
    for (const CarteTresor& carte : listeCarteTresor) {
        carte.printCard();
        std::cout << "-----------------------------------" << std::endl;
        index++;
    }

    std::cout << "\n========== CARTES VICTOIRE ==========" << std::endl;
    for (const CarteVictoire& carte : listeCarteVictoire) {
        carte.printCard();
        std::cout << "-----------------------------------" << std::endl;
        index++;
    }

    std::cout << "\n========== CARTES ACTION ==========" << std::endl;
    for (const CarteAction& carte : listeCarteAction) {
        carte.printCard();
        std::cout << "-----------------------------------" << std::endl;
        index++;
    }
}
*/

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

/*
// Méthode pour afficher le plateau
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
*/

/*
void Plateau::print() const {
    
    initscr();
    start_color();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    // Initialisation des paires de couleurs (si ce n'est pas déjà fait dans Plateau::print())
    init_pair(1, COLOR_YELLOW, COLOR_BLACK); // Trésor
    init_pair(2, COLOR_GREEN, COLOR_BLACK);  // Victoire
    init_pair(3, COLOR_RED, COLOR_BLACK);    // Action


    int y = 0; // Position verticale initiale

    attron(A_BOLD);
    mvprintw(y++, 0, "======== PLATEAU DOMINION ========");
    attroff(A_BOLD);

    // Affichage des piles Trésor
    attron(COLOR_PAIR(1));
    mvprintw(y++, 0, "========== PILES TRESOR ==========");
    attroff(COLOR_PAIR(1));

    for (size_t i = 0; i < m_PilesTresor.size(); i++) {
        attron(COLOR_PAIR(1));
        mvprintw(y++, 2, "Quantité: %d | Nom: %s | Coût: %d | Valeur: %d",
                 m_PilesTresor[i].second,
                 m_PilesTresor[i].first.getName().c_str(),
                 m_PilesTresor[i].first.getCost(),
                 m_PilesTresor[i].first.getCoins());
        attroff(COLOR_PAIR(1));
    }

    // Affichage des piles Victoire
    attron(COLOR_PAIR(2));
    mvprintw(y++, 0, "========== PILES VICTOIRE ==========");
    attroff(COLOR_PAIR(2));

    for (size_t i = 0; i < m_PilesVictoire.size(); i++) {
        attron(COLOR_PAIR(2));
        mvprintw(y++, 2, "Quantité: %d | Nom: %s | Coût: %d | Points: %d",
                 m_PilesVictoire[i].second,
                 m_PilesVictoire[i].first.getName().c_str(),
                 m_PilesVictoire[i].first.getCost(),
                 m_PilesVictoire[i].first.getWinPoints());
        attroff(COLOR_PAIR(2));
    }

    // Affichage des piles Action
    attron(COLOR_PAIR(3));
    mvprintw(y++, 0, "========== PILES ACTION ==========");
    attroff(COLOR_PAIR(3));

    for (size_t i = 0; i < m_PilesAction.size(); i++) {
        attron(COLOR_PAIR(3));
        mvprintw(y++, 2, "Quantité: %d | Nom: %s | Coût: %d",
                 m_PilesAction[i].second,
                 m_PilesAction[i].first.getName().c_str(),
                 m_PilesAction[i].first.getCost());
        attroff(COLOR_PAIR(3));
    }

    // Rafraîchir l'affichage
    refresh();
    
    // Attendre une entrée avant de quitter
    mvprintw(LINES - 1, 0, "Appuyez sur une touche pour quitter...");
    refresh();
    getch();

    // Terminaison de ncurses
    endwin();
}
*/

void Plateau::print(std::string pseudo, int coins, int buys, int score) const {
    initscr();
    start_color();
    cbreak();
    noecho();
    keypad(stdscr, TRUE); // Activer la prise en charge des touches spéciales comme TAB

    // Initialisation des couleurs
    init_pair(1, COLOR_YELLOW, COLOR_BLACK); // Trésor
    init_pair(2, COLOR_GREEN, COLOR_BLACK);  // Victoire
    init_pair(3, COLOR_RED, COLOR_BLACK);    // Action
    init_pair(4, COLOR_WHITE, COLOR_BLACK);  // Informations du joueur

    while (true) {
        clear();

        // Affichage des informations du joueur actif
        attron(COLOR_PAIR(4));
        mvprintw(0, 0, "Joueur: %s", pseudo.c_str());
        mvprintw(1, 0, "Pièces: %d | Achats: %d", coins, buys); // En haut à gauche
        mvprintw(1, COLS - 20, "Score: %d", score); // En haut à droite
        attroff(COLOR_PAIR(4));

        // Centrer le titre
        std::string title = "======== PLATEAU DOMINION ========";
        int title_x = (COLS - title.size()) / 2;
        mvprintw(1, title_x, "%s", title.c_str());

        // Fonction pour dessiner une carte complète
        auto draw_card_rectangle = [&](int y, int x, int width, int height, const std::string& name, int quantity, int cost, const std::string& extra, int color_pair, int index, const std::string& action_type = "") {
            attron(COLOR_PAIR(color_pair));

            // Dessiner la bordure haut
            mvhline(y, x, '-', width);
            mvaddch(y, x, '+');
            mvaddch(y, x + width - 1, '+');

            // Dessiner la bordure bas
            mvhline(y + height - 1, x, '-', width);
            mvaddch(y + height - 1, x, '+');
            mvaddch(y + height - 1, x + width - 1, '+');

            // Dessiner les bordures verticales
            for (int i = 1; i < height - 1; ++i) {
                mvaddch(y + i, x, '|');
                mvaddch(y + i, x + width - 1, '|');
            }

            // Gestion des noms UTF-8 (éviter les débordements)
            std::string truncated_name = name;
            if (truncated_name.size() > 20) {
                truncated_name = truncated_name.substr(0, 17) + "..."; // Tronquer avec "..."
            }

            // Contenu bien aligné
            mvprintw(y + 1, x + 2, "Nom: %s", truncated_name.c_str());
            mvprintw(y + 2, x + 2, "Quantité: %d", quantity);
            mvprintw(y + 3, x + 2, "Coût: %d", cost);
            mvprintw(y + 4, x + 2, "%s", extra.c_str());
            if (!action_type.empty()) {
                mvprintw(y + 5, x + 2, "Type: %s", action_type.c_str());
            }

            // Ajouter l'index au milieu de la dernière ligne
            std::string index_str = std::to_string(index);
            int index_x = x + (width - index_str.size()) / 2;
            mvprintw(y + height - 2, index_x, "%s", index_str.c_str());

            attroff(COLOR_PAIR(color_pair));
        };

        const int card_width = 28;  // Largeur fixe de la carte
        const int card_height = 8; // Hauteur augmentée pour inclure le type
        const int width_spacing = 3;     // Espacement entre les cartes en largeur
        const int height_spacing = 2;	// Espacement entre les cartes en hauteur

        int current_y = 3; // Position verticale initiale après les informations du joueur

        // Calcul pour centrer les cartes horizontalement
        int cards_per_line = 5; // Nombre de cartes par ligne pour les Actions
        int total_width = (cards_per_line * card_width) + ((cards_per_line - 1) * width_spacing);
        int start_x = (COLS - total_width) / 2;

        int current_x = start_x;
        int index = 0;

        // Affichage des piles Trésor
        attron(COLOR_PAIR(1));
        mvprintw(current_y++, (COLS - 25) / 2, "========== PILES TRESOR ==========");
        attroff(COLOR_PAIR(1));

        for (size_t i = 0; i < m_PilesTresor.size(); i++) {
            draw_card_rectangle(
                current_y, current_x, card_width, card_height,
                m_PilesTresor[i].first.getName(),
                m_PilesTresor[i].second,
                m_PilesTresor[i].first.getCost(),
                "Pieces ajoutees: " + std::to_string(m_PilesTresor[i].first.getCoins()),
                1, index++
            );
            current_x += card_width + width_spacing; // Passe à la position suivante
        }

        current_y += card_height + height_spacing;
        current_x = start_x;

        // Affichage des piles Victoire
        attron(COLOR_PAIR(2));
        mvprintw(current_y++, (COLS - 25) / 2, "========== PILES VICTOIRE ==========");
        attroff(COLOR_PAIR(2));

        for (size_t i = 0; i < m_PilesVictoire.size(); i++) {
            draw_card_rectangle(
                current_y, current_x, card_width, card_height,
                m_PilesVictoire[i].first.getName(),
                m_PilesVictoire[i].second,
                m_PilesVictoire[i].first.getCost(),
                "Points Victoire: " + std::to_string(m_PilesVictoire[i].first.getWinPoints()),
                2, index++
            );
            current_x += card_width + width_spacing;
        }

        current_y += card_height + height_spacing;
        current_x = start_x;

        // Affichage des piles Action
        attron(COLOR_PAIR(3));
        mvprintw(current_y++, (COLS - 25) / 2, "========== PILES ACTION ==========");
        attroff(COLOR_PAIR(3));

        for (size_t i = 0; i < m_PilesAction.size(); i++) {
            std::string action_type;
            if (m_PilesAction[i].first.getIsAttack()) {
                action_type = "Attaque";
            } else if (m_PilesAction[i].first.getIsReaction()) {
                action_type = "Réaction";
            } else {
                action_type = "Action";
            }

            draw_card_rectangle(
                current_y, current_x, card_width, card_height,
                m_PilesAction[i].first.getName(),
                m_PilesAction[i].second,
                m_PilesAction[i].first.getCost(),
                "",
                3, index++, action_type
            );
            current_x += card_width + width_spacing;

            // Si on a affiché 5 cartes, passer à la ligne suivante
            if ((i + 1) % cards_per_line == 0) {
                current_x = start_x;
                current_y += card_height + height_spacing;
            }
        }

        refresh(); // Actualisation de l'affichage

        // Instructions pour l'utilisateur
        mvprintw(LINES - 2, 0, "Appuyez sur TAB pour voir une carte en détail, ou sur 'q' pour quitter.");

        // Gestion des interactions utilisateur
        int ch = getch();

        if (ch == 'q') {
            break; // Quitter
        } else if (ch == '\t') { // Touche TAB
            clear();
            mvprintw(LINES / 2 - 1, (COLS - 40) / 2, "Entrez l'index de la carte (0-%d): ", index - 1);
            refresh();

            int selected_index = -1;
            echo();
            scanw("%d", &selected_index);
            noecho();

            if (selected_index >= 0 && selected_index < index) {
                clear();

                if (selected_index < static_cast<int>(m_PilesTresor.size())) {
                    m_PilesTresor[selected_index].first.printCard();
                } else if (selected_index < static_cast<int>(m_PilesTresor.size() + m_PilesVictoire.size())) {
                    int victoire_index = selected_index - m_PilesTresor.size();
                    m_PilesVictoire[victoire_index].first.printCard();
                } else {
                    int action_index = selected_index - m_PilesTresor.size() - m_PilesVictoire.size();
                    m_PilesAction[action_index].first.printCard();
                }

                mvprintw(LINES - 1, 0, "Appuyez sur une touche pour revenir...");
                refresh();
                getch();
            } else {
                mvprintw(LINES - 1, 0, "Index invalide. Appuyez sur une touche pour continuer...");
                refresh();
                getch();
            }
        }
    }

    endwin();
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

bool Plateau::getVide(size_t index){
  if(index > m_PilesTresor.size() - 1){ // Vérifie si l'index est plus loin que les cartes trésor
    index -= m_PilesTresor.size();
    if(index > m_PilesVictoire.size() - 1){ // Vérifie si l'index est plus loin que les cartes victoire
      index -= m_PilesVictoire.size(); 
      return (m_PilesAction.at(index).second == 0); // On est dans les cartes action
    }
    else{ // On est dans les cartes victoire
      return (m_PilesVictoire.at(index).second == 0);
    }
  }
  else{ // On est dans les cartes trésor
    return (m_PilesTresor.at(index).second == 0);
  }
}
