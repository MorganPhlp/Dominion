#include <iostream>
#include <string>
#include "Carte.h"
#include <ncurses.h>

// Constructeur
Carte::Carte(std::string name, std::string description, int cost, TypeCarte type) : m_name(name), m_description(description), m_cost(cost), m_type(type) {}

// Destructeur
Carte::~Carte() = default;

//void Carte::buy(Joueur &p, Jeu &j) {p.nbCoins -= cost;}

//Getters
std::string Carte::getName() const{return m_name;}

std::string Carte::getDescription() const{return m_description;}

TypeCarte Carte::getType() const{return m_type;}

int Carte::getCost() const{return m_cost;}

// Méthode pour afficher la carte
/*
void Carte::printCard() const{
	std::cout<< "Nom : " << m_name << std::endl;
	std::cout<< "Type : ";
	switch (m_type) { // Affiche le type de la carte en traduisant l'enum en string
		case TypeCarte::Action:
			std::cout<< "Action";
			break;
		case TypeCarte::Tresor:
			std::cout<< "Tresor";
			break;
		case TypeCarte::Victoire:
			std::cout<< "Victoire";
			break;
	}
	std::cout<< std::endl;
	std::cout<< "Coût : " << m_cost << std::endl;
	if(!m_description.empty()) std::cout<< "Description : " << m_description << std::endl; // Affiche la description seulement s'il y'en a une
}
*/

// Méthode pour afficher la carte avec ncurses
void Carte::printCard() const {
    initscr();            // Initialiser ncurses
    start_color();        // Activer les couleurs
    cbreak();
    noecho();

    // Initialisation des couleurs pour les différents types de cartes
    init_pair(1, COLOR_RED, COLOR_BLACK);    // Action
    init_pair(2, COLOR_YELLOW, COLOR_BLACK); // Tresor
    init_pair(3, COLOR_GREEN, COLOR_BLACK);  // Victoire

    clear(); // Nettoyer l'écran

    // Dimensions de la carte
    const int card_width = 30;
    const int card_height = 10;
    int start_x = (COLS - card_width) / 2; // Centrer horizontalement
    int start_y = (LINES - card_height) / 2; // Centrer verticalement

    // Choisir la couleur en fonction du type de carte
    int color_pair;
    switch (m_type) {
        case TypeCarte::Action:
            color_pair = 1;
            break;
        case TypeCarte::Tresor:
            color_pair = 2;
            break;
        case TypeCarte::Victoire:
            color_pair = 3;
            break;
    }
    attron(COLOR_PAIR(color_pair));

    // Dessiner la bordure de la carte
    mvhline(start_y, start_x, '-', card_width);                         // Bordure haut
    mvhline(start_y + card_height - 1, start_x, '-', card_width);       // Bordure bas
    mvvline(start_y, start_x, '|', card_height);                        // Bordure gauche
    mvvline(start_y, start_x + card_width - 1, '|', card_height);       // Bordure droite
    mvaddch(start_y, start_x, '+');                                     // Coin haut-gauche
    mvaddch(start_y, start_x + card_width - 1, '+');                    // Coin haut-droit
    mvaddch(start_y + card_height - 1, start_x, '+');                   // Coin bas-gauche
    mvaddch(start_y + card_height - 1, start_x + card_width - 1, '+');  // Coin bas-droit

    // Afficher les informations de la carte au centre
    mvprintw(start_y + 1, start_x + 2, "Nom: %s", m_name.c_str());
    mvprintw(start_y + 2, start_x + 2, "Type: %s",
             (m_type == TypeCarte::Action ? "Action" :
              m_type == TypeCarte::Tresor ? "Tresor" : "Victoire"));
    mvprintw(start_y + 3, start_x + 2, "Coût: %d", m_cost);

    // Afficher la description, si disponible
    if (!m_description.empty()) {
        mvprintw(start_y + 5, start_x + 2, "Description:");
        mvprintw(start_y + 6, start_x + 2, "%s", m_description.substr(0, card_width - 4).c_str());
    }

    attroff(COLOR_PAIR(color_pair)); // Désactiver la couleur

    // Pause pour visualiser
    mvprintw(LINES - 1, 0, "Appuyez sur une touche pour quitter...");
    refresh();
    getch();

    endwin(); // Terminer ncurses
}

