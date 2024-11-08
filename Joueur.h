#ifndef __Joueur_H__
#define __Joueur_H__

class Joueur{
	private :
		std::string m_pseudo;
		int m_nb_actions, m_nb_buys, m_nb_win_points, m_coins;
	public :
		Joueur(std::string pseudo);
		~Joueur();
		void addActions(actions);
		void addBuys(buys);
		/*int addDraws();*/		/*TODO faire en sorte que le joueur pioche des cartes et qu'elles s'ajoutent directement à son deck*/
		void addCoins(coins);
		void addWinPoints();
};

#endif
