#ifndef __Jeu_H__
#define __Jeu_H__
#include <vector>
#include "CarteAction.h"
#include "CarteVictoire.h"
#include "CarteTresor.h"

class Jeu{
  private :
    int m_nb_joueurs;
    
  public :
    
    Jeu(int nb_joueurs);
    ~Jeu();
};

#endif
