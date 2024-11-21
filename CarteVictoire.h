#ifndef __CarteVictoire_H__
#define __CarteVictoire_H__

#include <iostream>
#include <string>
#include "Carte.h"

class Joueur;

class CarteVictoire : public Carte{
  private :
  	int m_win_points;
  public :
  	CarteVictoire(std::string name, std::string description, int cost, int win_points); // Constructeur
    	~CarteVictoire(); // Destructeur
    	int getWinPoints() const;
    	void printCard() const; // Méthode pour afficher la carte
    	void play(Joueur &p, Plateau &plat, int index, Jeu &j) override; // Méthode pour jouer la carte (ce qui est impossible)
};

#endif
