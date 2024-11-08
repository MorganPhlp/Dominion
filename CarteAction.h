#ifndef __CarteAction_H__
#define __CarteAction_H__

class CarteAction : public Carte{
  private :
    int m_draws, m_buys, m_actions, m_coins;
    bool m_isAttack, m_isReaction;
  public :
    CarteAction(std::string name, std::string description, int cost, int actions, int buys, int draws, int coins, bool isAttack, bool isReaction);
    CarteAction(Carte c, int actions, int buys, int draws, int coins, bool isAttack, bool isReaction);
    ~CarteAction();
    void play(Joueur &p, Jeu &j);
    int getDraws();
    int getBuys();
    int getActions();
    int getCoins();
    bool getIsAttack();
    bool getIsReaction();
};

#endif
