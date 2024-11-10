#ifndef __Plateau_H__
#define __Plateau_H__
#include "CarteVictoire.h"
#include "CarteTresor.h"
#include "CarteAction.h"
#include <vector>
#include <utility>

class Plateau{
 private :
 	std::vector<CarteAction> m_listeCarteActionHasard;
 	std::vector<std::pair<CarteAction, int>> m_PilesAction;
 	std::vector<std::pair<CarteTresor, int>> m_PilesTresor;
 	std::vector<std::pair<CarteVictoire, int>> m_PilesVictoire;
 	
 	static std::vector<CarteAction> listeCarteAction;
 	static std::vector<CarteVictoire> listeCarteVictoire;
 	static std::vector<CarteTresor> listeCarteTresor;
 	
 	static std::vector< std::tuple< std::string, std::string, int, std::string, int, int, int, int, int, bool, bool>> loadCard();
 	static void remplirListeCarte();
 	void choisirCarteActionHasard();
 	void remplirPiles(int nb_joueurs);
 public :
 	static void printTotalCard();
 	void init(int nb_joueurs);
 	std::vector<CarteAction> getListeCarteAction() const;
 	std::vector<CarteTresor> getListeCarteTresor() const;
 	std::vector<CarteVictoire> getListeCarteVictoire() const;
 	
 	Plateau();
 	~Plateau();

};

#endif
