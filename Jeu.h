#ifndef __Jeu_H__
#define __Jeu_H__
#include <vector>
#include "Joueur.h"
#include "Plateau.h"
#include <unordered_map>

class Jeu{
  private :
    size_t m_nb_joueurs;
    Plateau m_plateau;
    std::vector<Joueur> m_listeJoueur;
    std::vector<size_t> m_joueursImmunises;
    std::unordered_map<size_t, std::vector<Carte*>> mapCartesDevoilees;
    Joueur* m_joueurActif;
    int m_nbMarchand;
    void initJoueurActif(std::vector<Joueur>& liste);
    void tourJoueur(Joueur* j);
  public :
    Jeu(Plateau plateau, std::vector<Joueur> listeJoueur);
    Jeu(Plateau plateau, std::vector<Joueur> listeJoueur, bool finPartie);
    ~Jeu();
    Plateau getPlateau(); //Retourne le plateau
    void tousSaufActifMalediction();  //Tous les joueurs sauf le joueur actif recoivent une carte malédiction	
    void volerCartesAdversaires();  //Les adversaires doivent choisir s'ils veulent écarter une carte Trésor ou non, et le joueur Actif décide s'il veut récupérer des cartes parmis ces cartes dévoilées
    std::vector<size_t> revelerCartes();  //Permet de révéler les 2 premières cartes du deck de tous les joueurs adverses
    void tousSaufActifPiochent(); //Tous les joueurs adverses piochent une carte
    void tousSaufActifDefausseJusqua(size_t n);  //Tous les joueurs adverses doivent défausser jusqu'à n cartes, sauf s'ils sont immunisés
    void tousSaufActifPoseCarteVictoire();  //Tous les joueurs adverses doivent dévoiler les cartes Victoire de leur main s'il y en a, ou bien ils dévoilent toute la main sinon, sauf s'ils sont immunisés
    void banditisme();  //Les adverses révèlent les 2 premieres cartes de leur deck, jettent une carte Tresor révélées autre que le Cuivre et défaussent le reste
    bool verifWin();  //Permet de vérifier une des conditions de victoire (pile de cartes Province vide)
    void calculerScoreFinal();  //Permet de calculer le tableau des scores finaux des joueurs à la fin d'une partie
    void jouerPartie();  //Méthode qui fait tourner la partie, avec des affichages explicites par rapport à où on en est dans la partie
    void espionnage();  //Permet de révéler la 1ère carte du deck de chaque joueur (sauf s'ils sont immunisés) et le joueur Actif décide s'il veut que cette carte soit défaussée ou si elle revient dans le deck du joueur
    void verifDouve();  //Permet de vérifier si un joueur possède une carte Douve, auquel cas le joueur sera immunisé
    void viderImmunises();  //Permet de vider le vecteur de joueurs immunisés
    void addNbMarchand();  //Permet d'incrémenter le nombre de cartes Marchand pendant une partie
};

#endif

//mode fin de partie à tester
