#ifndef __Carte_H__
#define __Carte_H__

#include <iostream>
#include <string>


enum class TypeCarte {
	Action,
	Tresor,
	Victoire
};

class Carte{
  private :
  	std::string m_name, m_description;
  	int m_cost;
  	TypeCarte m_type;
  public :
  	Carte(std::string name, std::string description, int cost, TypeCarte type);
  	~Carte();
  	//void play(Joueur &p, Jeu &j);
  	//void buy(Joueur &p, Jeu &j);
  	std::string getName() const;
   	std::string getDescription() const;
   	int getCost() const;
   	void printCard() const;
   	TypeCarte getType();   	
};

#endif
