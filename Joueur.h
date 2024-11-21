#ifndef __Joueur_H__
#define __Joueur_H__

#include <iostream>
#include <string>
#include <vector>
//#include "Carte.h"
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
		int getNbActions();
		int getNbBuys();
		int getNbWinPoints();
		int getCoins();
		int getDraws();
		std::string getPseudo();
		std::vector<Carte*> getHand();
		std::vector<Carte*> getDeck();
		std::vector<Carte*>& getDefausse();
		std::vector<Carte*>& getRebut();
		
		void addActions(int actions);
		void addBuys(int buys);
		void drawCards(int nb); // le joueur pioche nb cartes de son deck
		void addCoins(int coins);
		void addDraws(int draws);
		void addWinPoints(int win_points);
		void playCard(int indexCarte);
		int calculateWinPoints();
		void initDeck(Plateau p);
		void printHand() const;
		void assembleDeckDefausse();
		void makeHand();
		void defausser();
		void receiveCard(int n, Plateau &plat);
		void throwMax(int n);
		void defausseCarte(int index);
		void initNouveauTour();
		void buyCard(int index, Plateau plat);
		void jeterCarte(int index);
		void receiveMalediction(Plateau &plat);
		void putCardFromHandToDeck();
		void devoiler2Cartes(Plateau &plat);
		void receiveOr(Plateau &plat);
		void receiveArgent(Plateau &plat);
		void defaussePuisPioche();
		void piocher();
		void printDefausse();
		void regarderDefausseEtPrendre();
		void demandeDefausse();
		std::pair<size_t,std::vector<size_t>> getNbCarteVictoireHand();
		void carteVictoireOnDeck();
		void piocherJusquaEtDefausseAction(size_t n);
		std::pair<size_t,std::vector<size_t>> getNbCarteTresorHand();
		void receiveTresor(int n, Plateau &plat);
		void jeterTresorPourRecuperPlus(size_t n, Plateau &plat);
		void trocCuivrePieces();
		int renovation();
		void defausseCarteDeck(size_t index);
		void vassal(Plateau &plat, Jeu &j);
		void sentinelle();
		void putDeckInDefausse();
		int calculerPoints();
		std::vector<bool> decideDefausse(std::vector<Joueur> listeJoueur);
		bool handContainsDouve();
};

#endif
