#include <iostream>
#include "Carte.h"

/*Constructeur*/
Carte::Carte(std::string name, std::string description, int cost) : m_name(name), m_description(description), m_cost(cost) {}

/*Destructeur*/
Carte::~Carte() {}

void Carte::play(Joueur &p, Jeu &j) {}

void Carte::buy(Joueur &p, Jeu &j) {p.nbCoins -= cost;}

/*Getters*/
std::string Carte::getName(){return name;}

std::string Carte::getDescription(){return description;}

int Carte::getCost(){return cost;}
