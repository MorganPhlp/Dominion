#include <iostream>
#include <string>
#include "CarteAction.h"
#include "Joueur.h"
#include "Plateau.h"
#include "Jeu.h"
#include "Utilities.h"
#include <ncurses.h>
#include <sstream>
#include <vector>


// Constructeur
CarteAction::CarteAction(std::string name, std::string description, int cost, int actions, int buys, int draws, int coins, bool isAttack, bool isReaction) : Carte(name, description, cost, TypeCarte::Action), m_actions(actions), m_buys(buys), m_draws(draws), m_coins(coins), m_isAttack(isAttack), m_isReaction(isReaction) {}


void CarteAction::play(Joueur &p, Plateau &plat, int index, Jeu &j){ // Méthode pour jouer une carte
  if(!getDescription().empty()) playDescription(getName(), p, plat, index, j);
  p.addActions(m_actions);
  p.addBuys(m_buys);
  p.addDraws(m_draws);
  p.addCoins(m_coins);
  if(getName() != "Festin")p.defausseCarte(index);
  if(m_isAttack) j.viderImmunises();
}


// Getters
int CarteAction::getDraws() const{return m_draws;}

int CarteAction::getBuys() const{return m_buys;}

int CarteAction::getActions() const{return m_actions;}

int CarteAction::getCoins() const{return m_coins;}

bool CarteAction::getIsAttack() const{return m_isAttack;}

bool CarteAction::getIsReaction() const{return m_isReaction;}
/*
void CarteAction::printCard() const{ // Méthode pour afficher les cartes
	Carte::printCard();
	if(m_draws != 0) std::cout << "+" << m_draws << " Cartes" << std::endl;
	if(m_actions != 0) std::cout << "+" << m_actions << " Actions" << std::endl;
	if(m_buys != 0) std::cout << "+" << m_buys << " Achats" << std::endl;
	if(m_coins != 0) std::cout << "+" << m_coins << " Pièces" << std::endl;
	if(m_isAttack) std::cout << "Carte Attaque" << std::endl;
	if(m_isReaction) std::cout << "Carte Réaction" << std::endl;
}
*/

// Méthode pour afficher une carte Action
void CarteAction::printCard() const {
    initscr();
    start_color();
    cbreak();
    noecho();

    // Initialisation des couleurs pour les cartes Action
    init_pair(1, COLOR_RED, COLOR_BLACK); // Carte Action classique
    init_pair(2, COLOR_BLUE, COLOR_BLACK); // Carte Réaction
    init_pair(3, COLOR_MAGENTA, COLOR_BLACK); // Carte Attaque

    clear();

    // Dimensions de la carte
    const int card_width = 40;
    const int card_height = 20;
    int start_x = (COLS - card_width) / 2;  // Centrer horizontalement
    int start_y = (LINES - card_height) / 2; // Centrer verticalement

    // Déterminer la couleur en fonction du type
    int color_pair = 1; // Action classique
    if (this->getIsReaction()) {
        color_pair = 2; // Réaction
    } else if (this->getIsAttack()) {
        color_pair = 3; // Attaque
    }

    attron(COLOR_PAIR(color_pair));

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

    // Ajouter les effets de la carte
    if (m_draws != 0) mvprintw(start_y + 3, start_x + 2, "+%d Cartes", m_draws);
    if (m_actions != 0) mvprintw(start_y + 4, start_x + 2, "+%d Actions", m_actions);
    if (m_buys != 0) mvprintw(start_y + 5, start_x + 2, "+%d Achats", m_buys);
    if (m_coins != 0) mvprintw(start_y + 6, start_x + 2, "+%d Pièces", m_coins);

    // Indiquer si c'est une carte Attaque ou Réaction
    if (m_isAttack) mvprintw(start_y + 8, start_x + 2, "[Carte Attaque]");
    if (m_isReaction) mvprintw(start_y + 9, start_x + 2, "[Carte Réaction]");
     
    /*
    if (!this->getDescription().empty()) {
        int description_start_y = start_y + 5; // Lignes après les informations principales
        int max_lines = card_height - 6; // Nombre de lignes disponibles pour la description
        std::vector<std::string> wrapped_text = wrapText(this->getDescription(), card_width - 4);

        for (size_t i = 0; i < wrapped_text.size() && i < static_cast<size_t>(max_lines); ++i) {
            mvprintw(description_start_y + i, start_x + 2, "Description :");
            mvprintw(description_start_y + i + 1, start_x + 2, "%s", wrapped_text[i].c_str());
        }
    }
    */
    if (!this->getDescription().empty()) {
        std::string truncated_desc = this->getDescription().substr(0, card_width - 4); // Tronquer si trop long
        mvprintw(start_y + 10, start_x + 2, "Description :");
        mvprintw(start_y + 11, start_x + 2, "%s", truncated_desc.c_str());
    }

    attroff(COLOR_PAIR(color_pair));

    // Pause pour visualisation
    mvprintw(LINES - 1, 0, "Appuyez sur une touche pour continuer...");
    refresh();
    getch();

    endwin();
}



void CarteAction::playDescription(std::string name, Joueur &p, Plateau &plat, int &index, Jeu &j){ // Méthode pour gérer les différents cas spécifiques des cartes actions
        if(m_isAttack) j.verifDouve();
        std::cout << "La carte est d'attaque ? " << m_isAttack << std::endl; 
	if(name == "Atelier") p.receiveCard(4, plat);
	else if(name == "Chapelle") p.throwMax(4); // TODO Modifier index suivant les cartes supprimées pour défausser la bonne carte
	else if(name == "Festin"){
	  p.jeterCarte(index);
	  p.receiveCard(5, plat);
        }
        else if(name == "Sorcière") j.tousSaufActifMalediction();
        else if(name == "Voleur") j.volerCartesAdversaires();
        else if(name == "Artisan"){
          p.receiveCard(5, plat);
          p.putCardFromHandToDeck();
        }
        else if(name == "Bandit"){
          p.receiveOr(plat);
          j.banditisme();
        }
        else if(name == "Bureaucrate"){
          p.receiveArgent(plat); //Argent sur haut du deck
          j.tousSaufActifPoseCarteVictoire();
        }
        else if(name == "Cave") p.defaussePuisPioche();
        else if(name == "Chambre du conseil") j.tousSaufActifPiochent();
        else if(name == "Harbinger") p.regarderDefausseEtPrendre();
        else if(name == "Bibliothèque") p.piocherJusquaEtDefausseAction(7);
        else if(name == "Marchand") j.addNbMarchand();
        else if(name == "Milice") j.tousSaufActifDefausseJusqua(3);
        else if(name == "Mine") p.jeterTresorPourRecuperPlus(3, plat);
        else if(name == "Prêteur sur gages") p.trocCuivrePieces();
        else if(name == "Braconnier") j.defausserCarteParPileVide(plat);
        else if(name == "Rénovation") {
        	int cost = p.renovation();
        	p.receiveCard(cost,plat);
        }

        else if(name == "Sentinelle") p.sentinelle();
        else if(name == "Salle du Trône") {} //TODO Fonctionnement spécial à faire après
        else if (name == "Vassal") p.vassal(plat,j);
        else if(name == "Chancellier") p.putDeckInDefausse();
        else if(name == "Espion") j.espionnage();
}



