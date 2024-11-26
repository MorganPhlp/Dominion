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
    Plateau getPlateau();
    void choixAchatCarte();
    void tousSaufActifMalediction();
    void volerCartesAdversaires();
    std::vector<size_t> revelerCartes();
    void tousSaufActifPiochent();
    void tousSaufActifDefausseJusqua(size_t n);
    void tousSaufActifPoseCarteVictoire();
    void banditisme();
    bool verifWin();
    void calculerScoreFinal();
    void jouerPartie();
    void espionnage();
    void verifDouve();
    void viderImmunises();
    void addNbMarchand();
};

#endif

//mode fin de partie à tester
