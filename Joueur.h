#ifndef __Joueur_H__
#define __Joueur_H__

#include <iostream>
#include <string>
#include <vector>
#include "Carte.h"
#include "Pile.h"

class Joueur{
	private :
		std::string m_pseudo;
		int m_nb_actions, m_nb_buys, m_nb_win_points, m_coins;
		std::vector<Carte*> m_hand, m_deck, m_defausse, m_rebut;
	public :
		Joueur(std::string pseudo);
		~Joueur();
		void addActions(int actions);
		void addBuys(int buys);
		void drawCards(int nb);		/*le joueur pioche nb cartes de son deck*/
		void addCoins(int coins);
		void addWinPoints(int win_points);
		void playCard(int indexCarte);
		void buyCard(Pile &pile);
		int calculateWinPoints();
};

#endif
