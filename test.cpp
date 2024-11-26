#include "Jeu.h"
#include <ncurses.h>
#include <vector>
#include <cstdlib>
#include <ctime>


int main(){

  //Test partie de jeu
  
  Plateau plat = Plateau(2);
  Joueur j1 = Joueur("Morgan");
  Joueur j2 = Joueur("Ny Fenohery");
  std::vector<Joueur> liste = {j1,j2};
  Jeu jeu = Jeu(plat, liste);
  jeu.jouerPartie();
  return 0;


  // Partie presque à la fin 
  
  /*
  Plateau plat2 = Plateau(2, true);
  Joueur j21 = Joueur("Morgan");
  Joueur j22 = Joueur("Ny Fenohery");
  std::vector<Joueur> liste2 = {j21,j22};
  Jeu jeu2 = Jeu(plat2, liste2, true);
  jeu2.jouerPartie();
  */

  return 0;
}

