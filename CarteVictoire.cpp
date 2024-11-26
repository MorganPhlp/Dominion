#include <iostream>
#include <string>
#include "CarteVictoire.h"
#include "Joueur.h"
#include <ncurses.h>
#include <sstream>
#include <vector>

// Constructeur
CarteVictoire::CarteVictoire(std::string name, std::string description, int cost, int win_points) : Carte(name, description, cost, TypeCarte::Victoire), m_win_points(win_points) {}

// Getters
int CarteVictoire::getWinPoints() const{return m_win_points;}

// Méthode pour afficher une carte Victoire
void CarteVictoire::printCard() const {
    initscr();
    start_color();
    cbreak();
    noecho();

    // Initialisation des couleurs pour les cartes Victoire
    init_pair(1, COLOR_GREEN, COLOR_BLACK); // Couleur pour les cartes Victoire

    clear();

    // Dimensions de la carte
    const int card_width = 40;  // Largeur de la carte
    const int card_height = 15; // Hauteur de la carte
    const int padding = 2;      // Décalage intérieur
    int start_x = (COLS - card_width) / 2;   // Centrer horizontalement
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

    // Variables pour le contenu
    int current_y = start_y + 1; // Position initiale pour l'affichage des attributs

    // Ajouter les informations principales
    mvprintw(current_y++, start_x + padding, "Nom: %s", this->getName().c_str());
    mvprintw(current_y++, start_x + padding, "Coût: %d", this->getCost());
    mvprintw(current_y++, start_x + padding, "+%d Points de Victoire", this->getWinPoints());

    // Gestion de la description
    if (!this->getDescription().empty()) {
        std::string description = this->getDescription();
        mvprintw(current_y++, start_x + padding, "Description:");

        // Découper la description pour éviter les débordements
        size_t max_line_width = card_width - 2 * padding;
        std::istringstream desc_stream(description);
        std::string word;
        std::string line;

        while (desc_stream >> word) {
            if (line.size() + word.size() + 1 > max_line_width) {
                mvprintw(current_y++, start_x + padding, "%s", line.c_str());
                line.clear();
            }
            if (!line.empty()) line += " ";
            line += word;
        }
        if (!line.empty()) {
            mvprintw(current_y++, start_x + padding, "%s", line.c_str());
        }
    }

    attroff(COLOR_PAIR(1));

    // Pause pour visualisation et attendre 'q' pour quitter
    int ch;
    do {
        mvprintw(LINES - 1, 0, "Appuyez sur 'q' pour quitter.");
        refresh();
        ch = getch();
    } while (ch != 'q');

    endwin();
}


// Méthode pour jouer la carte (ce qui est impossible)
void CarteVictoire::play(Joueur &p, Plateau &plat, int index, Jeu &j) {
  (void)p; (void)plat; (void)index; (void)j; // Pour éviter les erreurs car comme play est virtuelle à la base si je ne la définis pas ma classe sera virtuelle
  throw std::logic_error("play() ne doit pas être appelée sur une CarteVictoire."); // Lever une exception car cette carte ne doit pas pouvoir être joué
}
