#include <iostream>
#include <string>
#include "CarteVictoire.h"
#include "Joueur.h"

// Constructeur
CarteVictoire::CarteVictoire(std::string name, std::string description, int cost, int win_points) : Carte(name, description, cost, TypeCarte::Victoire), m_win_points(win_points) {}

// Destructeur
CarteVictoire::~CarteVictoire() {}

// Getters
int CarteVictoire::getWinPoints() const{return m_win_points;}

void CarteVictoire::printCard() const{ // Méthode pour afficher la carte
	Carte::printCard();
	if(m_win_points != 0) std::cout << "+" << m_win_points << " Points Victoires" << std::endl;
}

// Méthode pour jouer la carte (ce qui est impossible)
void CarteVictoire::play(Joueur &p, Plateau &plat, int index, Jeu &j) {
  (void)p; (void)plat; (void)index; (void)j; // Pour éviter les erreurs car comme play est virtuelle à la base si je ne la définis pas ma classe sera virtuelle
  throw std::logic_error("play() ne doit pas être appelée sur une CarteVictoire."); // Lever une exception car cette carte ne doit pas pouvoir être joué
}
