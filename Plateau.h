#ifndef __Plateau_H__
#define __Plateau_H__
#include "CarteVictoire.h"
#include "CarteTresor.h"
#include "CarteAction.h"
#include <vector>
#include <utility>

class Plateau{
  private :
    std::vector<CarteAction> m_listeCarteActionChoisie;
    std::vector<std::pair<CarteAction, int>> m_PilesAction;
    std::vector<std::pair<CarteTresor, int>> m_PilesTresor;
    std::vector<std::pair<CarteVictoire, int>> m_PilesVictoire;
 	
    static std::vector<CarteAction> listeCarteAction;
    static std::vector<CarteVictoire> listeCarteVictoire;
    static std::vector<CarteTresor> listeCarteTresor;
 	
    std::vector<std::vector<Carte*>> m_listeCartesDevoilees;
    std::vector<Carte*> m_listeCartesEcartees;
 	
    static const size_t m_maxIndex;

    static int lireInt(std::stringstream& ss);
    static bool lireBool(std::stringstream& ss);
    static std::string lireGuillemets(std::stringstream& ss);
    static std::vector< std::tuple< std::string, std::string, int, std::string, int, int, int, int, int, bool, bool>> loadCard();
    static void remplirListeCarte();
    void choisirCarteActionHasard();
    void choisirCarteActionSetBase();
    void remplirPiles(int nb_joueurs);
  public :
    Plateau();
    ~Plateau();
 
    std::vector<CarteAction> getListeCarteAction() const;
    std::vector<CarteTresor> getListeCarteTresor() const;
    std::vector<CarteVictoire> getListeCarteVictoire() const;
    std::vector<std::pair<CarteAction, int>> getPilesAction();
    std::vector<std::pair<CarteTresor, int>> getPilesTresor();
    std::vector<std::pair<CarteVictoire, int>> getPilesVictoire();
    std::vector<std::vector<Carte*>>& getListeCartesDevoilees();
    std::vector<Carte*>& getListeCartesEcartees();
    size_t getMaxIndex() const;
 	
    void init(int nb_joueurs);
    static void printTotalCard();
    void print() const;
    int chercherCarteAction(std::string name);
    int chercherCarteTresor(std::string name);
    int chercherCarteVictoire(std::string name);
    Carte* buyCard(int index);
    std::vector<std::pair<Carte*, int>> getMax(int n);
    std::vector<std::pair<Carte*, int>> getMaxTresor(int n);

};

#endif
