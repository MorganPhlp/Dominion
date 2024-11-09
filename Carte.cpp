#include <iostream>
#include <string>
#include "Carte.h"
#include "Joueur.h"
#include "Jeu.h"

/*Constructeur*/
Carte::Carte(std::string name, std::string description, int cost, TypeCarte type) : m_name(name), m_description(description), m_cost(cost), m_type(type) {}

/*Destructeur*/
Carte::~Carte() {}

//void Carte::play(Joueur &p, Jeu &j) {}

//void Carte::buy(Joueur &p, Jeu &j) {p.nbCoins -= cost;}

/*Getters*/
std::string Carte::getName() const{return m_name;}

std::string Carte::getDescription() const{return m_description;}

TypeCarte Carte::getType(){return m_type;}

int Carte::getCost() const{return m_cost;}

void Carte::printCard() const{
	std::cout<< "Nom : " << m_name << std::endl;
	std::cout<< "Type : ";
	switch (m_type) {
		case TypeCarte::Action:
			std::cout<< "Action";
			break;
		case TypeCarte::Tresor:
			std::cout<< "Tresor";
			break;
		case TypeCarte::Victoire:
			std::cout<< "Victoire";
			break;
	}
	std::cout<< std::endl;
	std::cout<< "Coût : " << m_cost << std::endl;
	std::cout<< "Description : " << m_description << std::endl;
}
