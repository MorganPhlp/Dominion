#ifndef __CarteAction_H__
#define __CarteAction_H__

class CarteAction{
  private :
    std::string m_name, m_description;
    int m_cost, m_draws, m_buys, m_actions, m_coins;
  public :
    CarteAction(std::string name, std::string description, int cost, int actions, int buys, int draws, int coins);
    ~CarteAction();
    void play(Joueur &p, Jeu &j);
};

#endif
