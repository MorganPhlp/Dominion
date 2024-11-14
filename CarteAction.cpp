#include <iostream>
#include <string>
#include "CarteAction.h"
#include "Joueur.h"
#include "Plateau.h"

CarteAction::CarteAction(std::string name, std::string description, int cost, int actions, int buys, int draws, int coins, bool isAttack, bool isReaction) : Carte(name, description, cost, TypeCarte::Action), m_actions(actions), m_buys(buys), m_draws(draws), m_coins(coins), m_isAttack(isAttack), m_isReaction(isReaction) {}

/*Destructeur*/
CarteAction::~CarteAction() {}


void CarteAction::play(Joueur &p, Plateau &plat){ //TODO Ajouter le int index
  if(!getDescription().empty()) playDescription(getName(), p, plat);
  p.addActions(m_actions);
  p.addBuys(m_buys);
  p.addDraws(m_draws);
  p.addCoins(m_coins);
  // TODO Ajouter la méthode défausseCarte
}


/*Getters*/
int CarteAction::getDraws() const{return m_draws;}

int CarteAction::getBuys() const{return m_buys;}

int CarteAction::getActions() const{return m_actions;}

int CarteAction::getCoins() const{return m_coins;}

bool CarteAction::getIsAttack() const{return m_isAttack;}

bool CarteAction::getIsReaction() const{return m_isReaction;}

void CarteAction::printCard() const{
	Carte::printCard();
	if(m_draws != 0) std::cout << "+" << m_draws << " Cartes" << std::endl;
	if(m_actions != 0) std::cout << "+" << m_actions << " Actions" << std::endl;
	if(m_buys != 0) std::cout << "+" << m_buys << " Achats" << std::endl;
	if(m_coins != 0) std::cout << "+" << m_coins << " Pièces" << std::endl;
	if(m_isAttack) std::cout << "Carte Attaque" << std::endl;
	if(m_isReaction) std::cout << "Carte Réaction" << std::endl;
}



void CarteAction::playDescription(std::string name, Joueur &p, Plateau &plat){
	if(name == "Atelier") p.receiveCard(4, plat);
	if(name == "Chapelle") p.throwMax(4);
}



