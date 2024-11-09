#ifndef __CarteAction_H__
#define __CarteAction_H__
#include "Carte.h"

#include <iostream>
#include <string>
#include "Carte.h"
#include "Joueur.h"
#include "Jeu.h"

class CarteAction : public Carte{
  private :
    int m_actions, m_buys, m_draws, m_coins;
    bool m_isAttack, m_isReaction;
  public :
    CarteAction(std::string name, std::string description, int cost, int actions, int buys, int draws, int coins, bool isAttack, bool isReaction);
    CarteAction(Carte c, int actions, int buys, int draws, int coins, bool isAttack, bool isReaction);
    ~CarteAction();
    //void play(Joueur &p, Jeu &j);
    int getDraws() const;
    int getBuys() const;
    int getActions() const;
    int getCoins() const;
    bool getIsAttack() const;
    bool getIsReaction() const;
    void printCard() const;
};

#endif
