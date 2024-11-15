#ifndef __CarteTresor_H__
#define __CarteTresor_H__

#include <iostream>
#include <string>
#include "Carte.h"
class Joueur;

class CarteTresor : public Carte{
  private :
  	int m_coins;
  public :
	CarteTresor(std::string name, std::string description, int cost, int coins);
	//CarteTresor(Carte c, int coins);
	~CarteTresor();
	void play(Joueur &p, Plateau &plat, int index, Jeu &j) override;
	int getCoins() const;
	void printCard() const;
};

#endif
