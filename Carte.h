#ifndef __Carte_H__
#define __Carte_H__

class Carte{
  private :
  	std::string m_name, m_description;
  	int m_cost;
  public :
  	Carte(std::string name, std::string description, int cost);
  	~Carte();
  	void play(Joueur &p, Jeu &j);
  	void buy(Joueur &p, Jeu &j);
  	std::string getName();
   	std::string getDescription();
   	int getCost();
  	
};

#endif
