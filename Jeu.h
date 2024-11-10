#ifndef __Jeu_H__
#define __Jeu_H__
#include <vector>
#include <tuple>
#include <fstream>
#include <sstream>

#include "CarteAction.h"
#include "CarteVictoire.h"
#include "CarteTresor.h"

class Jeu{
  private :
    static std::vector< std::tuple< std::string, std::string, int, std::string, int, int, int, int, int, bool, bool>> loadCard();
    int m_nb_joueurs;
    static std::vector<CarteAction> listeCarteAction;
    static std::vector<CarteVictoire> listeCarteVictoire;
    static std::vector<CarteTresor> listeCarteTresor;
    
  public :
    static void remplirListeCarte();
    static void printTotalCard();
    std::vector<CarteAction> choisirCarteActionHasard();
    
    Jeu(int nb_joueurs);
    ~Jeu();
};

#endif
