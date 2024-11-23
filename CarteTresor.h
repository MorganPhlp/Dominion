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
	CarteTresor(std::string name, std::string description, int cost, int coins); // Constructeur
	~CarteTresor() override = default; // Destructeur
	void play(Joueur &p, Plateau &plat, int index, Jeu &j) override; // Méthode pour jouer la carte
	int getCoins() const;
	void printCard() const override; // Méthode pour afficher la carte
};

#endif
