#include <iostream>
#include <string>
#include "CarteVictoire.h"
#include "Joueur.h"

/*Constructeur*/
//CarteVictoire::CarteVictoire(Carte c, int win_points) : Carte(c.getName(), c.getDescription(), c.getCost()), m_win_points(win_points) {}

CarteVictoire::CarteVictoire(std::string name, std::string description, int cost, int win_points) : Carte(name, description, cost, TypeCarte::Victoire), m_win_points(win_points) {}

/*Destructeur*/
CarteVictoire::~CarteVictoire() {}

void CarteVictoire::play(Joueur &p, Plateau &plat, int index, Jeu &j){
  (void) j;
  (void) plat;
  p.addWinPoints(m_win_points);
  p.defausseCarte(index);
}

int CarteVictoire::getWinPoints() const{return m_win_points;}

void CarteVictoire::printCard() const{
	Carte::printCard();
	if(m_win_points != 0) std::cout << "+" << m_win_points << " Points Victoires" << std::endl;
}
