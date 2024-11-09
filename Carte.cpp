#include <iostream>
#include <string>
#include "Carte.h"
#include "Joueur.h"
#include "Jeu.h"

/*Constructeur*/
Carte::Carte(std::string name, std::string description, int cost, TypeCarte type) : m_name(name), m_description(description), m_cost(cost), m_type(type) {}

/*Destructeur*/
Carte::~Carte() {}

void Carte::play(Joueur &p, Jeu &j) {}

void Carte::buy(Joueur &p, Jeu &j) {p.nbCoins -= cost;}

/*Getters*/
std::string Carte::getName(){return name;}

std::string Carte::getDescription(){return description;}

int Carte::getCost(){return cost;}

TypeCarte Carte::getType(){return type;}
