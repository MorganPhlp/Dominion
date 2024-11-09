#include <iostream>
#include <string>
#include "CarteTresor.h"

/*Constructeur*/
//CarteTresor::CarteTresor(Carte c, int coins) : Carte(c.getName(), c.getDescription(), c.getCost()), m_coins(coins) {}

CarteTresor::CarteTresor(std::string name, std::string description, int cost, int coins) : Carte(name, description, cost, TypeCarte::Tresor), m_coins(coins) {}

/*Destructeur*/
CarteTresor::~CarteTresor() {}

//void CarteTresor::play(Joueur &p, Jeu &j){p.addCoins(coins);}

int CarteTresor::getCoins() const{return m_coins;}

void CarteTresor::printCard() const{
	Carte::printCard();
	if(m_coins != 0) std::cout << "+" << m_coins << " Pièces" << std::endl;
}
