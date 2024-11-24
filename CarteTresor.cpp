#include <iostream>
#include <string>
#include "CarteTresor.h"
#include "Joueur.h"
#include <ncurses.h>
#include "Utilities.h"
#include <sstream>
#include <vector>

// Constructeur
CarteTresor::CarteTresor(std::string name, std::string description, int cost, int coins) : Carte(name, description, cost, TypeCarte::Tresor), m_coins(coins) {}

void CarteTresor::play(Joueur &p, Plateau &plat, int index, Jeu &j){ // Méthode pour jouer la carte qui ajoute les pièces correspondantes au joueur et défausse la carte utilisée
  (void) j; 
  (void) plat; // Obligatoires car le Plateau et le Jeu sont dans la définition de base de Carte mais non utilisés ici
  p.addCoins(m_coins);
  p.defausseCarte(index);
}

// Getters
int CarteTresor::getCoins() const{return m_coins;}
/*
void CarteTresor::printCard() const{ // Méthode pour afficher la carte
	Carte::printCard();
	if(m_coins != 0) std::cout << "+" << m_coins << " Pièces" << std::endl;
}
*/

// Méthode pour afficher une carte Tresor avec ncurses
void CarteTresor::printCard() const {
    initscr();
    start_color();
    cbreak();
    noecho();

    // Initialisation des couleurs pour les cartes Tresor
    init_pair(1, COLOR_YELLOW, COLOR_BLACK); // Couleur pour les cartes Tresor

    clear();

    // Dimensions de la carte
    const int card_width = 35;  // Largeur de la carte
    const int card_height = 10; // Hauteur de la carte
    int start_x = (COLS - card_width) / 2;  // Centrer horizontalement
    int start_y = (LINES - card_height) / 2; // Centrer verticalement

    attron(COLOR_PAIR(1));

    // Dessiner les bordures de la carte
    mvhline(start_y, start_x, '-', card_width);
    mvhline(start_y + card_height - 1, start_x, '-', card_width);
    mvvline(start_y, start_x, '|', card_height);
    mvvline(start_y, start_x + card_width - 1, '|', card_height);
    mvaddch(start_y, start_x, '+');
    mvaddch(start_y, start_x + card_width - 1, '+');
    mvaddch(start_y + card_height - 1, start_x, '+');
    mvaddch(start_y + card_height - 1, start_x + card_width - 1, '+');

    // Ajouter les informations principales
    mvprintw(start_y + 1, start_x + 2, "Nom: %s", this->getName().c_str());
    mvprintw(start_y + 2, start_x + 2, "Coût: %d", this->getCost());
    mvprintw(start_y + 3, start_x + 2, "+%d Pièces", this->getCoins());

    // Afficher la description, si disponible, en gérant les retours à la ligne
    if (!this->getDescription().empty()) {
        int description_start_y = start_y + 5; // Lignes après les informations principales
        int max_lines = card_height - 6; // Nombre de lignes disponibles pour la description
        std::vector<std::string> wrapped_text = wrapText(this->getDescription(), card_width - 4);

        for (size_t i = 0; i < wrapped_text.size() && i < static_cast<size_t>(max_lines); ++i) {
            mvprintw(description_start_y + i, start_x + 2, "Description :");
            mvprintw(description_start_y + i + 1, start_x + 2, "%s", wrapped_text[i].c_str());
        }
    }

    attroff(COLOR_PAIR(1));

    // Pause pour visualisation
    mvprintw(LINES - 1, 0, "Appuyez sur une touche pour continuer...");
    refresh();
    getch();

    endwin();
}

