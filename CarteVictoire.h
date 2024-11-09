#ifndef __CarteVictoire_H__
#define __CarteVictoire_H__
#include "Carte.h"

#include <iostream>
#include <string>
#include "Carte.h"
#include "Joueur.h"
#include "Jeu.h"

class CarteVictoire : public Carte{
  private :
  	int m_win_points;
  public :
  	CarteVictoire(std::string name, std::string description, int cost, int win_points);
  	CarteVictoire(Carte c, int win_points);
    	~CarteVictoire();
    	//void play(Joueur &p, Jeu &j);
    	int getWinPoints() const;
    	void printCard() const;
};

#endif
