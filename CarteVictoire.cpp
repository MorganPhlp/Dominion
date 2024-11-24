#include <iostream>
#include <string>
#include "CarteVictoire.h"
#include "Joueur.h"
#include "Utilities.h"
#include <ncurses.h>
#include <sstream>
#include <vector>

// Constructeur
CarteVictoire::CarteVictoire(std::string name, std::string description, int cost, int win_points) : Carte(name, description, cost, TypeCarte::Victoire), m_win_points(win_points) {}

// Getters
int CarteVictoire::getWinPoints() const{return m_win_points;}

/*
void CarteVictoire::printCard() const{ // Méthode pour afficher la carte
	Carte::printCard();
	if(m_win_points != 0) std::cout << "+" << m_win_points << " Points Victoires" << std::endl;
}
*/

// Méthode pour afficher une carte Victoire avec ncurses
/*
void CarteVictoire::printCard() const {
    initscr();
    start_color();
    cbreak();
    noecho();

    // Initialisation des couleurs pour les cartes Victoire
    init_pair(1, COLOR_GREEN, COLOR_BLACK); // Couleur pour les cartes Victoire

    clear();

    // Dimensions de la carte
    const int card_width = 35;  // Largeur de la carte
    const int card_height = 8; // Hauteur de la carte
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
    mvprintw(start_y + 3, start_x + 2, "+%d Points Victoire", this->getWinPoints());

    // Afficher la description, si disponible
    if (!this->getDescription().empty()) {
        std::string truncated_desc = this->getDescription().substr(0, card_width - 4); // Tronquer si trop long
        mvprintw(start_y + 5, start_x + 2, "Description: %s", truncated_desc.c_str());
    }

    attroff(COLOR_PAIR(1));

    // Pause pour visualisation
    mvprintw(LINES - 1, 0, "Appuyez sur une touche pour continuer...");
    refresh();
    getch();

    endwin();
}
*/

void CarteVictoire::printCard() const {
    initscr();
    start_color();
    cbreak();
    noecho();

    // Initialisation des couleurs pour les cartes Victoire
    init_pair(1, COLOR_GREEN, COLOR_BLACK); // Couleur pour les cartes Victoire

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
    mvprintw(start_y + 3, start_x + 2, "+%d Points Victoire", this->getWinPoints());

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



// Méthode pour jouer la carte (ce qui est impossible)
void CarteVictoire::play(Joueur &p, Plateau &plat, int index, Jeu &j) {
  (void)p; (void)plat; (void)index; (void)j; // Pour éviter les erreurs car comme play est virtuelle à la base si je ne la définis pas ma classe sera virtuelle
  throw std::logic_error("play() ne doit pas être appelée sur une CarteVictoire."); // Lever une exception car cette carte ne doit pas pouvoir être joué
}
