#ifndef __Joueur_H__
#define __Joueur_H__

#include <iostream>
#include <string>
#include <vector>
//#include "Carte.h"
#include "Plateau.h"

class Joueur{
  private :
    std::string m_pseudo;
    int m_nb_actions, m_nb_buys, m_nb_win_points, m_coins, m_draws;
    std::vector<Carte*> m_hand, m_deck, m_defausse, m_rebut;
    template<typename T>
    void addCardsByNameInDeck(const std::string& name, int nb, std::vector<T>& cardList, int winPoints = 0); // Permet d'ajouter plusieurs copies d'une carte spécifique identifiée par son nom dans le deck du joueur
    void shuffleCartes(std::vector<Carte*>& v);
    void transferCards(std::vector<Carte*>& source, std::vector<Carte*>& destination, size_t nb);
  public :
    Joueur(std::string pseudo);
    ~Joueur();
    // Getters d'attributs simples
    int getNbActions();
    int getNbBuys();
    int getNbWinPoints();
    int getCoins();
    int getDraws();
    std::string getPseudo();
    
    // Getters de vector
    std::vector<Carte*>& getHand();
    std::vector<Carte*>& getDeck();
    std::vector<Carte*>& getDefausse();
    std::vector<Carte*>& getRebut();

    // Ajouts de ressources au joueur
    void addActions(int actions);
    void addBuys(int buys);
    void drawCards(int nb);
    void addCoins(int coins);
    void addDraws(int draws);
    void addWinPoints(int win_points);
    
    // Essai de sauvegarde de partie mais non fini
    /*
    void saveCards(const std::string& filename, const std::vector<Carte*>& cards);
    void loadCards(const std::string& filename, std::vector<Carte*>& cards, const Plateau& plateau);
    void loadPlayer(const Plateau& plateau, std::string pseudo);
    void savePlayer();
    */
    
    void initDeck(Plateau& p);  // Méthode pour initialiser le deck du joueur
    void initDeckFin(Plateau& p); // Méthode spéciale pour la soutenance
    void printCards(const std::vector<Carte*>& cards, const std::string& name) const;  //méthode qui permet d'afficher des cartes dans un vecteur
    
    //Méthodes pour afficher la main, la défausse, le deck et le rébut
    void printHand() const;
    void printDefausse() const;
    void printDeck() const;
    void printRebut() const;
    
    void assembleDeckDefausse();  //Permet de récupérer des cartes de la défausse et de les transférer dans le deck
    void makeHand();  //Permet de créer une main si cette dernière contient moins de 5 cartes
    void defausser();  //Permet de défausser toute une main
    void throwMax(int n);  //Permet au joueur de jeter jusqu'à n cartes
    
    void defausseCarte(int index);  //Permet de défausser une carte d'un certain index
    void initNouveauTour();  //Permet d'initialiser les attributs du joueur actif lors d'un nouveau tour
    void buyCard(int index, Plateau &plat);  //Permet d'acheter une carte disponible sur le plateau, qui sera ajoutée à la défausse
    void jeterCarte(int index);  //Permet de jeter une carte qui se trouve à un certain index
    
    void putCardFromHandToDeck();  //Permet de mettre une carte de la main directement sur le haut du deck
    void devoiler2Cartes(Plateau &plat);  //Permet de dévoiler les 2 premières cartes du deck du joueur
    void receiveCard(int n, Plateau &plat);  //Permet de recevoir une carte dans sa défausse
    
    void receiveMalediction(Plateau &plat);  //Permet de recevoir une carte Malédiction
    void receiveOr(Plateau &plat);  //Permet au joueur de recevoir une carte Or, qui sera ajoutée à sa défausse
    void receiveArgent(Plateau &plat);  //Permet au joueur de recevoir une carte Argent qui sera ajoutée sur le haut de son deck
    void receiveTresor(int n, Plateau &plat);  //Permet de recevoir une carte Tresor
    
    void defaussePuisPioche();  //Permet au joueur de défausser un nombre n de cartes et de piocher n cartes
    void piocher();  //Permet de piocher la carte la plus haute du deck
    void regarderDefausseEtPrendre();  //Permet de regarder la défausse et de prendre une carte pour la mettre au dessus du deck
    void demandeDefausse();  //Demande au joueur quelle carte il souhaite défausser
    
    std::pair<size_t, std::vector<size_t>> getNbCarteHandByType(TypeCarte type);  //Permet d'obtenir le nombre de carte d'un certain type et les indices de ces cartes dans la main du joueur
    
    //Permet d'obtenir le nombre de carte Victoire, Trésor et Action et les indices de ces cartes dans la main du joueur
    std::pair<size_t,std::vector<size_t>> getNbCarteVictoireHand();
    std::pair<size_t,std::vector<size_t>> getNbCarteTresorHand();
    std::pair<size_t,std::vector<size_t>> getNbCarteActionHand();
    
    void carteVictoireOnDeck();  //Permet de savoir si un deck contient des cartes Victoire ou non et en fonction du nombre de cartes Victoire, de les mettre sur le haut du deck
    void piocherJusquaEtDefausseAction(size_t n);  //Permet de piocher jusqu'à avoir n cartes dans sa main, et si on pioche une carte Action, on peut la garder dans sa main ou bien la défausser
     
    void jeterTresorPourRecuperPlus(size_t n, Plateau &plat);  //Permet de jeter une carte Trésor et de recevoir une carte Trésor coûtant jusqu'à 3 pièces de plus
    void trocCuivrePieces();  //Permet au joueur de troquer une carte Cuivre pour 3 pièces
    int renovation();  //Oblige le joueur à jeter une carte de sa main mais il reçoit une carte coutant jusqu a 2 pieces de plus que celle qu'il a jetée
    void defausseCarteDeck(size_t index);  //Permet de défausser une carte du deck
    
    void vassal(Plateau &plat, Jeu &j);  //Permet de défausser la premiere carte en haut du deck, si c'est une carte Action, le joueur peut la jouer
    void sentinelle();  //Permet de regarder les 2 premieres cartes du deck, et le joueur choisit de jeter et/ou defausser autant qu'il le souhaite. Ensuite le reste est remis sur le haut du deck dans l ordre que le joueur souhaite
    void putDeckInDefausse();  //Permet de défausser tout le deck ou non
    int calculerPoints();  //Permet de calculer les points de Victoire dans le deck du joueur
    
    std::vector<bool> decideDefausse(std::vector<Joueur> listeJoueur);  //Permet de décider si l'on veut défausser une carte ou non des joueurs adverses
    bool handContainsDouve();  //Vérifie si la main d'un joueur contient une carte Douve
    void resetDraws();  //Permet de remmettre à 0 le nombre de cartes piochables par un joueur
    
    bool queJardins();  //Permet de vérifier dans la main d'un joueur ne contient que des cartes Jardins
    void calculerScore();  //Calcule le score total d'un joueur en parcourant sa main, son deck et sa défausse
    void defausserCarteParPileVide(Plateau& plat);  //Permet de défausser un certain nombre de cartes par pile vide sur le plateau
};

#endif
