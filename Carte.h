#ifndef __Carte_H__
#define __Carte_H__

#include <iostream>
#include <string>

class Joueur;
class Plateau;
class Jeu; //Des Forward Declarations ici car on a besoin de ces objets dans la méthode virtuelle play et qu'ils sont compilés après Carte

enum class TypeCarte { // Représente le type de carte
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
  	Carte(std::string name, std::string description, int cost, TypeCarte type); // Constructeur
  	virtual ~Carte(); // Destructeur virtuel car au moins une méthode virtuelle
  	virtual void play(Joueur &p, Plateau &plat, int index, Jeu &j) = 0; // Méthode virtuelle pour jouer la carte qui ne sera défini que dans ses classes héritées
  	//void buy(Joueur &p, Jeu &j);
  	
  	// Getters
  	std::string getName() const;
   	std::string getDescription() const;
   	int getCost() const;
   	TypeCarte getType() const;
   	
   	virtual void printCard() const = 0; // Méthode pour afficher la carte
};

#endif
