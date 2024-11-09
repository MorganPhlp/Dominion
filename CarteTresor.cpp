#include <iostream>
#include <string>
#include "CarteTresor.h"
#include "Joueur.h"
#include "Jeu.h"
#include "Carte.h"

/*Constructeur*/
CarteTresor::CarteTresor(Carte c, int coins) : Carte(c.getName(), c.getDescription(), c.getCost()), m_coins(coins) {}

CarteTresor::CarteTresor(std::string name, std::string description, int cost, int coins) : Carte(name, description, cost), m_coins(coins) {}

/*Destructeur*/
CarteTresor::~CarteTresor() {}

void CarteTresor::play(Joueur &p, Jeu &j){p.addCoins(coins);}

int CarteTresor::getCoins(){return coins;}
