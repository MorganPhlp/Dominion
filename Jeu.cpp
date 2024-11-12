#include "Jeu.h"
#include <random>
#include <chrono>
#include <functional>

Jeu::Jeu(int nb_joueurs, Plateau plateau, std::vector<Joueur> listeJoueur) : m_nb_joueurs(nb_joueurs), m_plateau(plateau), m_listeJoueur(listeJoueur) {
	initJoueurActif(listeJoueur);
}

Jeu::~Jeu() {}

void Jeu::initJoueurActif(std::vector<Joueur>& liste){
	std::default_random_engine re(std::chrono::system_clock::now().time_since_epoch().count());
	int taille = liste.size();
	std::uniform_int_distribution<int> distrib{0, taille-1};
	auto rd = bind(distrib, re);
	int alea = rd();
	m_joueurActif = &liste.at(alea);
}



