#include <iostream>
#include "CarteAction.h"

CarteAction::CarteAction(std::string name, std::string description, int cost, int actions, int buys, int draws, int coins) : m_name(name), m_description(description), m_cost(cost), m_actions(actions), m_buys(buys), m_draws(draws), m_coins(coins) {}

CarteAction::~CarteAction() {}

void CarteAction::play(Joueur &p, Jeu &j){
  p.addActions(actions);
  p.addBuys(buys);
  p.addDraws(draws);
  p.addCoins(coins);
}
