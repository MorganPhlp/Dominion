#include <iostream>
#include "Joueur.h"

/*Constructeur*/
Joueur::Joueur(std::string pseudo) : m_pseudo(pseudo) {}

/*Desctruction*/
Joueur::~Joueur() {}

void Joueur::addBuys(buys){m_nb_buys += buys;}

void Joueur::addActions(actions){m_nb_actions += actions;}

void Joueur::addCoins(coins){m_nb_coins += coins;}

void Joueur::addWinPoints(win_points){m_nb_win_points += win_points;}

