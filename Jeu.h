#ifndef __Jeu_H__
#define __Jeu_H__
#include <vector>
#include "Joueur.h"
#include "Plateau.h"

class Jeu{
  private :
    int m_nb_joueurs;
    Plateau m_plateau;
    std::vector<Joueur> m_listeJoueur;
    Joueur* m_joueurActif;
    void initJoueurActif(std::vector<Joueur>& liste);
    void tourJoueur(Joueur* j);
  public :
    Jeu(int nb_joueurs, Plateau plateau, std::vector<Joueur> listeJoueur);
    ~Jeu();
    Plateau getPlateau();
    void choixAchatCarte();
    void tousSaufActifMalediction();
    void volerCartesAdversaires();
    void revelerCartes();
    void tousSaufActifPiochent();
    void tousSaufActifDefausseJusqua(size_t n);
    void tousSaufActifPoseCarteVictoire();
    void banditisme();
    void defausserCarteParPileVide(Plateau& plat);
    bool verifWin();
    void calculerScoreFinal();
    void jouerPartie();
    void espionnage();
};

#endif

//mode fin de partie à tester
