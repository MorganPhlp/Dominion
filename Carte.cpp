#include <iostream>
#include "Carte.h"

/*Constructeur*/
Carte::Carte(std::string name, std::string description, int cost) : m_name(name), m_description(description), m_cost(cost) {}

/*Destructeur*/
Carte::~Carte() {}

//void Carte::play(Joueur &p, Jeu &j) {}

//void Carte::buy(Joueur &p, Jeu &j) {p.nbCoins -= cost;}

/*Getters*/
std::string Carte::getName() const{return m_name;}

std::string Carte::getDescription() const{return m_description;}

int Carte::getCost() const{return m_cost;}

void Carte::printCard() const{
	std::cout<< "Nom : " << m_name << std::endl;
	std::cout<< "Coût : " << m_cost << std::endl;
	std::cout<< "Description : " << m_description << std::endl;
}
