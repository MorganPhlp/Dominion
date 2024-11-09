#ifndef __Pile_H__
#define __Pile_H__

#include <iostream>
#include <vector>
#include <memory>
#include "Carte.h"

class Pile{
	private :
		TypeCarte m_type;
		std::vector<std::shared_ptr<Carte>> m_cartes;
 
	public :
		Pile(TypeCarte type);
		void addCard(std::shared_ptr<Carte> c);
		void takeCard();
		int nb_cartesRestantes();
		TypeCarte getType();
	

};

#endif
