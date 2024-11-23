#ifndef __CarteAction_H__
#define __CarteAction_H__

#include <iostream>
#include <string>
#include "Carte.h"

// Forward Declarations car on utilise les méthodes de ces classes mais elles sont compilées plus tard
class Joueur;
class Plateau;
class Jeu;

class CarteAction : public Carte{
  private :
    int m_actions, m_buys, m_draws, m_coins;
    bool m_isAttack, m_isReaction;
    void playDescription(std::string d, Joueur &p, Plateau &plat, int &index, Jeu &j); //Méthode play mais pour les cartes qui ont une description et donc un fonctionnement particulier
  public :
    CarteAction(std::string name, std::string description, int cost, int actions, int buys, int draws, int coins, bool isAttack, bool isReaction); // Constructeur
    ~CarteAction() override = default; // Destructeur
    void play(Joueur &p, Plateau &plat, int index, Jeu &j) override; // Méthode pour jouer une carte
    
    // Getters
    int getDraws() const;
    int getBuys() const;
    int getActions() const;
    int getCoins() const;
    bool getIsAttack() const;
    bool getIsReaction() const;
    
    void printCard() const; // Méthode pour afficher la carte
};

#endif
