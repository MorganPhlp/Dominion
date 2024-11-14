#ifndef __Joueur_H__
#define __Joueur_H__

#include <iostream>
#include <string>
#include <vector>
#include "Plateau.h"

class Joueur{
	private :
		std::string m_pseudo;
		int m_nb_actions, m_nb_buys, m_nb_win_points, m_coins, m_draws;
		std::vector<Carte*> m_hand, m_deck, m_defausse, m_rebut;
		void shuffleCartes(std::vector<Carte*> v);
	public :
		Joueur(std::string pseudo);
		~Joueur();
		void addActions(int actions);
		void addBuys(int buys);
		void drawCards(int nb);		/*le joueur pioche nb cartes de son deck*/
		void addCoins(int coins);
		void addDraws(int draws);
		void addWinPoints(int win_points);
		void playCard(int indexCarte);
		int calculateWinPoints();
		void initDeck(Plateau p);
		void printDeck() const;
		void assembleDeckDefausse();
		void makeHand();
		void defausser();
		void printHand() const;
		void receiveCard(int n, Plateau &plat);
		void throwMax(int n);
};

#endif
