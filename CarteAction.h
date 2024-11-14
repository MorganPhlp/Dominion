#ifndef __CarteAction_H__
#define __CarteAction_H__

#include <iostream>
#include <string>
#include "Carte.h"

class Joueur;
class Plateau;

class CarteAction : public Carte{
  private :
    int m_actions, m_buys, m_draws, m_coins;
    bool m_isAttack, m_isReaction;
    void playDescription(std::string d, Joueur &p, Plateau &plat);
  public :
    CarteAction(std::string name, std::string description, int cost, int actions, int buys, int draws, int coins, bool isAttack, bool isReaction);
    ~CarteAction();
    void play(Joueur &p, Plateau &plat);
    int getDraws() const;
    int getBuys() const;
    int getActions() const;
    int getCoins() const;
    bool getIsAttack() const;
    bool getIsReaction() const;
    void printCard() const;
};

#endif
