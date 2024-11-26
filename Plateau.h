#ifndef __Plateau_H__
#define __Plateau_H__
#include "CarteVictoire.h"
#include "CarteTresor.h"
#include "CarteAction.h"
#include <vector>
#include <utility>

class Plateau{
  private :
    std::vector<CarteAction> m_listeCarteActionChoisie; // Contient les 10 cartes action qui auront été choisies pour la partie
    
    // Les piles de chaque catégorie de carte
    std::vector<std::pair<CarteAction, int>> m_PilesAction;
    std::vector<std::pair<CarteTresor, int>> m_PilesTresor;
    std::vector<std::pair<CarteVictoire, int>> m_PilesVictoire;
    
    // Les listes de toutes les cartes disponibles pour chaque catégorie de carte
    static std::vector<CarteAction> listeCarteAction;
    static std::vector<CarteVictoire> listeCarteVictoire;
    static std::vector<CarteTresor> listeCarteTresor;
    
    // Les listes de cartes dévoilées et écartées pour certains effets de carte action
    std::vector<std::vector<Carte*>> m_listeCartesDevoilees;
    std::vector<Carte*> m_listeCartesEcartees;
    
    static const size_t m_maxIndex; // L'index maximum qu'on peut avoir pour acheter une carte (le nombre de cartes différentes sur le plateau)

    static int lireInt(std::stringstream& ss); // Lire les int dans un csv
    static bool lireBool(std::stringstream& ss); // Lire les booléens dans un csv
    static std::string lireGuillemets(std::stringstream& ss); // Lire un texte entre guillemets dans un csv (pour les textes qui ont des virgules)
    static std::vector< std::tuple< std::string, std::string, int, std::string, int, int, int, int, int, bool, bool>> loadCard(); // Charger les cartes dans un vecteur général depuis le fichier csv
    static void remplirListeCarte(); // Remplir les listes pour chaque type de cartes chargées
    void choisirCarteActionHasard(); // Choisir au hasard les 10 cartes action de la partie
    void choisirCarteActionSetBase(); // Choisir les 10 cartes action du set de base
    void choisirCarteActionCreation();
    void remplirPiles(int nb_joueurs); // Remplir les piles de cartes
    template <typename T>
    size_t chercherCarte(const std::vector<std::pair<T, int>>& pile, const std::string& name); // Trouver l'id de la carte cherchée par nom pour pouvoir l'acheter
  public :
    Plateau(int nb_joueurs); // Constructeur
    Plateau(int nb_joueurs, bool finPartie); // Constructeur spécial pour la soutenance
    ~Plateau(); // Destructeur
    
    // Getters
    std::vector<CarteAction>& getListeCarteAction();
    std::vector<CarteTresor>& getListeCarteTresor();
    std::vector<CarteVictoire>& getListeCarteVictoire();
    std::vector<std::pair<CarteAction, int>> getPilesAction();
    std::vector<std::pair<CarteTresor, int>> getPilesTresor();
    std::vector<std::pair<CarteVictoire, int>> getPilesVictoire();
    std::vector<std::vector<Carte*>>& getListeCartesDevoilees();
    std::vector<Carte*>& getListeCartesEcartees();
    size_t getMaxIndex() const;
    
    bool getVide(size_t index);
    int getNbPileVide();
 	
    void print(std::string pseudo, int coins, int buys, int score) const; // Afficher le plateau
    
    // Trouver l'id d'une carte d'un type en rentrant son nom
    size_t chercherCarteAction(std::string name);
    size_t chercherCarteTresor(std::string name);
    size_t chercherCarteVictoire(std::string name);
    
    Carte* buyCard(size_t index); // Prendre une carte sur le plateau
    std::vector<std::pair<Carte*, int>> getMax(int n); // Avoir toutes les cartes disponibles d'un coût maximal n
    std::vector<std::pair<Carte*, int>> getMaxTresor(int n); // Avoir toutes les cartes trésor disponibles d'un coût maximal n
    int chercherCoutParIndex(size_t index);
    void modifierPlateauFin(); // Méthode spéciale pour la soutenance

};

#endif
