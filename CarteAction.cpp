#include <iostream>
#include <string>
#include "CarteAction.h"
#include "Joueur.h"
#include "Plateau.h"
#include "Jeu.h"

CarteAction::CarteAction(std::string name, std::string description, int cost, int actions, int buys, int draws, int coins, bool isAttack, bool isReaction) : Carte(name, description, cost, TypeCarte::Action), m_actions(actions), m_buys(buys), m_draws(draws), m_coins(coins), m_isAttack(isAttack), m_isReaction(isReaction) {}

/*Destructeur*/
CarteAction::~CarteAction() {}


void CarteAction::play(Joueur &p, Plateau &plat, int index, Jeu &j){
  if(!getDescription().empty()) playDescription(getName(), p, plat, index, j);
  p.addActions(m_actions);
  p.addBuys(m_buys);
  p.addDraws(m_draws);
  p.addCoins(m_coins);
  p.defausseCarte(index);
  if(m_isAttack) j.viderImmunises();
}


/*Getters*/
int CarteAction::getDraws() const{return m_draws;}

int CarteAction::getBuys() const{return m_buys;}

int CarteAction::getActions() const{return m_actions;}

int CarteAction::getCoins() const{return m_coins;}

bool CarteAction::getIsAttack() const{return m_isAttack;}

bool CarteAction::getIsReaction() const{return m_isReaction;}

void CarteAction::printCard() const{
	Carte::printCard();
	if(m_draws != 0) std::cout << "+" << m_draws << " Cartes" << std::endl;
	if(m_actions != 0) std::cout << "+" << m_actions << " Actions" << std::endl;
	if(m_buys != 0) std::cout << "+" << m_buys << " Achats" << std::endl;
	if(m_coins != 0) std::cout << "+" << m_coins << " Pièces" << std::endl;
	if(m_isAttack) std::cout << "Carte Attaque" << std::endl;
	if(m_isReaction) std::cout << "Carte Réaction" << std::endl;
}



void CarteAction::playDescription(std::string name, Joueur &p, Plateau &plat, int index, Jeu &j){ 
        if(m_isAttack){
          j.verifDouve();
        }
	if(name == "Atelier") p.receiveCard(4, plat);
	else if(name == "Chapelle") p.throwMax(4);
	else if(name == "Festin"){
	  p.jeterCarte(index);
	  p.receiveCard(5, plat);
        }
        else if(name == "Sorcière") j.tousSaufActifMalediction();
        else if(name == "Voleur") j.volerCartesAdversaires();
        else if(name == "Artisan"){
          p.receiveCard(5, plat);
          p.putCardFromHandToDeck();
        }
        else if(name == "Bandit"){
          p.receiveOr(plat);
          j.banditisme();
        }
        else if(name == "Bureaucrate"){
          p.receiveArgent(plat); //Argent sur haut du deck
          j.tousSaufActifPoseCarteVictoire();
        }
        else if(name == "Cave") p.defaussePuisPioche();
        else if(name == "Chambre du conseil") j.tousSaufActifPiochent();
        else if(name == "Harbinger") p.regarderDefausseEtPrendre();
        else if(name == "Bibliothèque") p.piocherJusquaEtDefausseAction(7);
        else if(name == "Marchand") {} // TODO Fonctionnement spécial à faire après
        else if(name == "Milice") j.tousSaufActifDefausseJusqua(3);
        else if(name == "Mine") p.jeterTresorPourRecuperPlus(3, plat);
        else if(name == "Prêteur sur gages") p.trocCuivrePieces();
        else if(name == "Braconnier") j.defausserCarteParPileVide(plat);
        else if(name == "Rénovation") {
        	int cost = p.renovation();
        	p.receiveCard(cost,plat);
        }

        else if(name == "Sentinelle")p.sentinelle();
        else if(name == "Salle du Trône") {} //TODO Fonctionnement spécial à faire après
        else if (name == "Vassal") p.vassal(plat,j);
        else if(name == "Chancellier") p.putDeckInDefausse();
        else if(name == "Espion") {}
}



