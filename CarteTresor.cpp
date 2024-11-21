#include <iostream>
#include <string>
#include "CarteTresor.h"
#include "Joueur.h"

// Constructeur
CarteTresor::CarteTresor(std::string name, std::string description, int cost, int coins) : Carte(name, description, cost, TypeCarte::Tresor), m_coins(coins) {}

// Destructeur
CarteTresor::~CarteTresor() {}

void CarteTresor::play(Joueur &p, Plateau &plat, int index, Jeu &j){ // Méthode pour jouer la carte qui ajoute les pièces correspondantes au joueur et défausse la carte utilisée
  (void) j; 
  (void) plat; // Obligatoires car le Plateau et le Jeu sont dans la définition de base de Carte mais non utilisés ici
  p.addCoins(m_coins);
  p.defausseCarte(index);
}

// Getters
int CarteTresor::getCoins() const{return m_coins;}

void CarteTresor::printCard() const{ // Méthode pour afficher la carte
	Carte::printCard();
	if(m_coins != 0) std::cout << "+" << m_coins << " Pièces" << std::endl;
}
