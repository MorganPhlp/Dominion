#ifndef __Jeu_H__
#define __Jeu_H__
#include <vector>
#include "Joueur.h"
#include "Plateau.h"

class Jeu{
  private :
    int m_nb_joueurs;
    Plateau m_plateau;
    std::vector<Joueur> m_listeJoueur;
    Joueur* m_joueurActif;
    void initJoueurActif(std::vector<Joueur>& liste);
  public :
    Jeu(int nb_joueurs, Plateau plateau, std::vector<Joueur> listeJoueur);
    ~Jeu();
};

#endif
