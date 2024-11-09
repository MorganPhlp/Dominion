#include <iostream>
#include <string>
#include "Joueur.h"

/*Constructeur*/
Joueur::Joueur(std::string pseudo) : m_pseudo(pseudo) {}

/*Desctruction*/
Joueur::~Joueur() {}

void Joueur::addBuys(int buys){m_nb_buys += buys;}

void Joueur::addActions(int actions){m_nb_actions += actions;}

void Joueur::addCoins(int coins){m_nb_coins += coins;}

void Joueur::addWinPoints(int win_points){m_nb_win_points += win_points;}

