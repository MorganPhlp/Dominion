#include <iostream>
#include "CarteAction.h"

/*Constructeur*/
CarteAction::CarteAction(Carte c, int actions, int buys, int draws, int coins, bool isAttack, bool isReaction) : Carte(c.getName(), c.getDescription(), c.getCost()), m_actions(actions), m_buys(buys), m_draws(draws), m_coins(coins), m_isAttack(isAttack), m_isReaction(isReaction) {}

CarteAction::CarteAction(std::string name, std::string description, int cost, int actions, int buys, int draws, int coins, bool isAttack, bool isReaction) : Carte(name, description, cost), m_actions(actions), m_buys(buys), m_draws(draws), m_coins(coins), m_isAttack(isAttack), m_isReaction(isReaction) {}

/*Destructeur*/
CarteAction::~CarteAction() {}

void CarteAction::play(Joueur &p, Jeu &j){
  p.addActions(actions);
  p.addBuys(buys);
  p.addDraws(draws);
  p.addCoins(coins);
}

/*Getters*/
int CarteAction::getDraws(){return draws;}

int CarteAction::getBuys(){return buys;}

int CarteAction::getActions(){return actions;}

int CarteAction::getCoins(){return coins;}

bool CarteAction::getIsAttack(){return isAttack;}

bool CarteAction::getIsReaction(){return isReaction;}
