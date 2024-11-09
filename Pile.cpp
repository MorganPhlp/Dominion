#include <iostream>
#include <string>
#include <vector>

#include "Pile.h"

/*Constructeur*/
Pile::Pile(TypeCarte type) : m_type(type) {}

/*Destructeur*/
Pile::~Pile() {}

void Pile::addCard(std::shared_ptr<Carte> c){m_cartes.push_back(c);}

std::shared_ptr<Carte> Pile::takeCard(){
	if(!m_cartes.empty()){
		auto carte = m_cartes.back();
		m_cartes.pop_back();
		return carte;
	}
	return null_ptr;
}

int Pile::nb_cartesRestantes(){return m_cartes.size();}

TypeCarte Pile::getType(){return m_type;}
