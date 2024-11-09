#include "CarteVictoire.h"

/*Constructeur*/
CarteVictoire::CarteVictoire(Carte c, int win_points) : Carte(c.getName(), c.getDescription(), c.getCost()), m_win_points(win_points) {}

CarteVictoire::CarteVictoire(std::string name, std::string description, int cost, int win_points) : Carte(name, description, cost), m_win_points(win_points) {}

/*Destructeur*/
CarteVictoire::~CarteVictoire() {}

//void CarteVictoire::play(Joueur &p, Jeu &j){p.addWinPoints(win_points);}

int CarteVictoire::getWinPoints() const{return m_win_points;}

void CarteVictoire::printCard() const{
	Carte::printCard();
	if(m_win_points != 0) std::cout << "+" << m_win_points << " Points Victoires" << std::endl;
}
