#include "Jeu.h"
#include <random>
#include <chrono>
#include <functional>
#include <iomanip>

std::vector<size_t> m_joueursImmunises = {};
int m_nbMarchand = 0;

Jeu::Jeu(Plateau plateau, std::vector<Joueur> listeJoueur) : m_nb_joueurs(listeJoueur.size()), m_plateau(plateau), m_listeJoueur(listeJoueur) {
	for(size_t i = 0; i < m_listeJoueur.size(); i++){
		m_listeJoueur.at(i).initDeck(m_plateau);
		m_listeJoueur.at(i).makeHand();
	}
	initJoueurActif(m_listeJoueur);
}

Jeu::~Jeu() {}

void Jeu::initJoueurActif(std::vector<Joueur>& liste){
	std::default_random_engine re(std::chrono::system_clock::now().time_since_epoch().count());
	int taille = liste.size();
	std::uniform_int_distribution<int> distrib{0, taille-1};
	auto rd = bind(distrib, re);
	int alea = rd();
	m_joueurActif = &liste.at(alea);
	std::cout << "Le joueur actif initial est : " << m_joueurActif->getPseudo() << std::endl;
}

Plateau Jeu::getPlateau(){
	return m_plateau;
}

void Jeu::tourJoueur(Joueur* j){
  j->initNouveauTour();
  j->printHand();

  //Phase Action
  std::cout << "======== Phase Action ========" << std::endl;
  std::string choix;
  size_t index;
  while(j->getNbActions() > 0 && j->getNbCarteActionHand().first != 0){
    std::cout << "Nombre d'actions restantes : " << j->getNbActions() << std::endl;
    j->printHand();
    std::cout << "Entrez le numéro de la carte que vous souhaitez jouer (ou ACHAT pour passer à la phase Achat : " << std::endl;
    std::cin >> choix;
    if(choix == "ACHAT") break;
    try{
      index = std::stoi(choix);
      if(index < j->getHand().size()){
        Carte* carte = j->getHand().at(index);
        if(carte->getType() != TypeCarte::Action || carte->getName() == "Jardins"){
          std::cout << "Vous ne pouvez pas jouer cette carte" << std::endl;
        }
        else{
          carte->play(*j, m_plateau, index, *this);
          j->addActions(-1);
        }		
      }
      else{
        std::cout << "Index invalide" << std::endl;
      }
    }
    catch (...){
      std::cerr << "Erreur lors de la lecture de l'index" << std::endl;
    }
  }	
  
  //Phase Achat
  std::cout << "======== Phase Achat ========" << std::endl;
  while(j->getNbCarteTresorHand().first > 0){
    j->printHand();
    std::cout << "Entrez l'index d'une carte Trésor à jouer (ou PASSER pour passer) : " << std::endl;
    std::cin >> choix;
    if(choix == "PASSER") break;
    try{
      index = std::stoi(choix);
      if(index < j->getHand().size()){
        Carte* carte = j->getHand().at(index);
        if(carte->getType() == TypeCarte::Tresor){
          if(carte->getName() == "Argent" && m_nbMarchand != 0){
            carte->play(*j, m_plateau, index, *this);
            j->addCoins(m_nbMarchand);
            m_nbMarchand = 0;
          }
          else{
            carte->play(*j, m_plateau, index, *this);
          }
        }
        else{
          std::cout << "Vous ne pouvez pas jouer cette carte" << std::endl;
        }
      }
      else{
        std::cout << "Index Invalide" << std::endl;
      }
    }
    catch (...){
      std::cerr << "Erreur lors de la lecture de l'index" << std::endl;
    }
  }
  
  while(j->getNbBuys() > 0){
    std::cout << "Vous avez " << j->getCoins() << " pièces et " << j->getNbBuys() << " achats restants : " << std::endl;
    m_plateau.print();
    std::cout << "Choisissez l'index de la pile pour acheter une carte ou PASSER pour passer : " << std::endl;
    std::cin >> choix;
    if (choix == "PASSER") break;
    try{
      if (index < m_plateau.getMaxIndex()) {
        try {
          j->buyCard(index, m_plateau);
        } catch (const std::exception& e) {
          std::cerr << "Erreur : " << e.what() << std::endl;
        }
      }
      else{
        std::cout << "Index invalide" << std::endl;
      }
    }
    catch (...){
      std::cerr << "Erreur lors de la lecture de l'index" << std::endl;
    }
  }
	
  //phase ajustement
  std::cout << "======== Phase Ajustement ========" << std::endl;
  j->defausser(); //Défausser toutes les cartes jouées et les cartes en main

  //Reformer le deck si nécessaire et piocher 5 cartes pour une nouvelle main
  if (j->getDeck().size() < 5) {
    j->assembleDeckDefausse();
  }
  j->makeHand();  // Piocher 5 nouvelles cartes

  std::cout << "Votre nouvelle main pour le prochain tour est prête !" << std::endl;
  j->printHand();
}

void Jeu::tousSaufActifMalediction(){
  for(size_t i = 0; i < m_listeJoueur.size(); i++){
    if(&m_listeJoueur.at(i) != m_joueurActif && std::find(m_joueursImmunises.begin(), m_joueursImmunises.end(), i) == m_joueursImmunises.end()){
      m_listeJoueur.at(i).receiveMalediction(m_plateau);
    }
  }
}

void Jeu::revelerCartes(){
	for(size_t i = 0; i < m_listeJoueur.size(); i++){
		if(&m_listeJoueur.at(i) != m_joueurActif && std::find(m_joueursImmunises.begin(), m_joueursImmunises.end(), i) == m_joueursImmunises.end()){
			m_listeJoueur.at(i).devoiler2Cartes(m_plateau);
		}
	}
}

void Jeu::banditisme(){
	revelerCartes();
	std::vector<std::vector<Carte*>>& listeCartesDevoilees = m_plateau.getListeCartesDevoilees();
	for(size_t i = 0; i < listeCartesDevoilees.size(); i++){
		if(&m_listeJoueur.at(i) != m_joueurActif && std::find(m_joueursImmunises.begin(), m_joueursImmunises.end(), i) == m_joueursImmunises.end()){
			for(size_t j = 0; j < 2; j++){
				Carte* c = listeCartesDevoilees.at(i).at(j);
				//c->printCard();
				if(c->getType() == TypeCarte::Tresor && c->getName() != "Cuivre"){
					std::vector<Carte*>& rebut = m_listeJoueur.at(i).getRebut();
					rebut.push_back(c);
				}
				std::vector<Carte*>& defausse = m_listeJoueur.at(i).getDefausse();
				defausse.push_back(c);
			}
		}
	}	
}

void Jeu::volerCartesAdversaires(){
	std::vector<std::vector<Carte*>>& listeCartesDevoilees = m_plateau.getListeCartesDevoilees();
	std::vector<Carte*>& listeCartesEcartees = m_plateau.getListeCartesEcartees();
	size_t index;
	bool stop = false;
	std::string res1,res2;
	revelerCartes();
	for(size_t i = 0; i < listeCartesDevoilees.size(); i++){
		for(size_t j = 0; j < 2; j++){
			Carte* c = listeCartesDevoilees.at(i).at(j);
			c->printCard();
			if(c->getType() == TypeCarte::Tresor){
				std::cout << "Voulez-vous écarter cette carte ? Répondez par 'Oui' ou 'oui' ou 'O' ou 'o' si vous le souhaitez" << std::endl;
				std::cin >> res1;
				if(res1 == "O" or res1 == "o" or res1 == "Oui" or res1 == "oui"){
					listeCartesEcartees.push_back(c);
					break;
				}
			}
		}
	}
	while(!stop or res2 != "O" or res2 != "o" or res2 != "Oui" or res2 != "oui"){
		std::cout << "Souhaitez-vous récupérer une des cartes écartées ?" << std::endl;
		std::cin >> res2;
		if(res2 == "O" or res2 == "o" or res2 == "Oui" or res2 == "oui"){
			std::cout << "Entrer l'index de la carte que vous souhaitez récupérer : " << std::endl;
			std::cin >> index;
			if(index >= listeCartesEcartees.size()){
				stop = true;
			}
			Carte* c = listeCartesEcartees.at(index);
			m_joueurActif->getDefausse().push_back(c);
			listeCartesEcartees.erase(listeCartesEcartees.begin() + index);
		}
		
	}
	for(size_t i = 0; i < listeCartesEcartees.size(); i++){
		Carte* c = listeCartesEcartees.at(i);
		m_joueurActif->getRebut().push_back(c);
	}
	listeCartesDevoilees.clear();
	listeCartesEcartees.clear();
}

void Jeu::tousSaufActifPiochent(){
  for(size_t i = 0; i < m_listeJoueur.size(); i++){
    if(&m_listeJoueur.at(i) != m_joueurActif){
      m_listeJoueur.at(i).piocher();
    }
  }
}

void Jeu::tousSaufActifDefausseJusqua(size_t n){
  for(size_t i = 0; i < m_listeJoueur.size(); i++){
    if(&m_listeJoueur.at(i) != m_joueurActif && std::find(m_joueursImmunises.begin(), m_joueursImmunises.end(), i) == m_joueursImmunises.end()){
      m_listeJoueur.at(i).printHand();
      while(m_listeJoueur.at(i).getHand().size() > n){
        m_listeJoueur.at(i).demandeDefausse();
      }
    }
  }
}

void Jeu::tousSaufActifPoseCarteVictoire(){
  for(size_t i = 0; i < m_listeJoueur.size(); i++){
    if(&m_listeJoueur.at(i) != m_joueurActif && std::find(m_joueursImmunises.begin(), m_joueursImmunises.end(), i) == m_joueursImmunises.end()){
      m_listeJoueur.at(i).printHand();
      if(m_listeJoueur.at(i).getNbCarteVictoireHand().first == 0){
        m_listeJoueur.at(i).printHand(); // TODO Dévoiler la main aux autres joueurs (peut-être à changer)
      }
      else m_listeJoueur.at(i).carteVictoireOnDeck();
    }
  }
}

void Jeu::defausserCarteParPileVide(Plateau& plat){
	size_t taillePileAction = plat.getPilesAction().size();
	for(size_t i = 0; i < taillePileAction; i++){
		if(plat.getPilesAction().at(i).second == 0){
			for(size_t j = 0; j < m_listeJoueur.size(); i++){
				for(size_t k = 0; k < m_listeJoueur.at(j).getHand().size(); k++){
					if(m_listeJoueur.at(j).getHand().at(k)->getName() == plat.getPilesAction().at(i).first.getName()){
						m_listeJoueur.at(j).getDefausse().push_back(m_listeJoueur.at(j).getHand().at(k));
					}
				}
			}
		}
	}
	size_t taillePileTresor = plat.getPilesTresor().size();
	for(size_t i = 0; i < taillePileTresor; i++){
		if(plat.getPilesTresor().at(i).second == 0){
			for(size_t j = 0; j < m_listeJoueur.size(); i++){
				for(size_t k = 0; k < m_listeJoueur.at(j).getHand().size(); k++){
					if(m_listeJoueur.at(j).getHand().at(k)->getName() == plat.getPilesTresor().at(i).first.getName()){
						m_listeJoueur.at(j).getDefausse().push_back(m_listeJoueur.at(j).getHand().at(k));
					}
				}
			}
		}
	}
	size_t taillePileVictoire = plat.getPilesVictoire().size();
	for(size_t i = 0; i < taillePileVictoire; i++){
		if(plat.getPilesVictoire().at(i).second == 0){
			for(size_t j = 0; j < m_listeJoueur.size(); i++){
				for(size_t k = 0; k < m_listeJoueur.at(j).getHand().size(); k++){
					if(m_listeJoueur.at(j).getHand().at(k)->getName() == plat.getPilesVictoire().at(i).first.getName()){
						m_listeJoueur.at(j).getDefausse().push_back(m_listeJoueur.at(j).getHand().at(k));
					}
				}
			}
		}
	}
}

bool Jeu::verifWin(){
  int pilesVide = 0;
  
  for(auto& pile : m_plateau.getPilesVictoire()){
    if(pile.second == 0){
      if(pile.first.getName() == "Province") return true;
      pilesVide++;
    }
  }
  for(auto& pile : m_plateau.getPilesAction()){
    if(pile.second == 0) pilesVide++;
  }
  for(auto& pile : m_plateau.getPilesTresor()){
    if(pile.second == 0) pilesVide++;
  }
  
  return pilesVide >= 3;
}

void Jeu::calculerScoreFinal() {
    std::cout << "======== Affichage des scores ========" << std::endl;
    std::vector<std::pair<Joueur, int>> tabScore;
    for (auto& joueur : m_listeJoueur) {
        int score = joueur.calculerPoints();
        tabScore.push_back(std::make_pair(joueur, score));
    }
    
    std::sort(tabScore.begin(), tabScore.end(), [](const auto &p1, const auto &p2){
      return p1.second > p2.second;
    });
    
    std::cout << std::left << std::setw(15) << "Joueur" << "Score" << std::endl;
    std::cout << "-------------------------------" << std::endl;
    for (size_t i = 0; i < tabScore.size(); ++i) {
        if (i == 0) {
            std::cout << std::left << std::setw(15) << tabScore[i].first.getPseudo() 
                      << tabScore[i].second << " (GAGNANT)" << std::endl;
        } else {
            std::cout << std::left << std::setw(15) << tabScore[i].first.getPseudo() 
                      << tabScore[i].second << std::endl;
        }
    }
    std::cout << "===============================" << std::endl;
}

void Jeu::jouerPartie(){
  std::cout << "======== Début de la partie ========" << std::endl;
  int nbTour = 0;
  while (!verifWin()) {
    nbTour++;
    std::cout << "\n======== Tour numéro " << nbTour << " ========" << std::endl;
    for (size_t i = 0; i < m_listeJoueur.size(); i++) {
      m_joueurActif = &m_listeJoueur.at(i);
      std::cout << "\nTour de " << m_joueurActif->getPseudo() << std::endl;
      tourJoueur(m_joueurActif);
      if (verifWin()) {
        std::cout << "Fin de la partie atteinte en " << nbTour << " tours" << std::endl;
        break;
      }
    }
  }
  calculerScoreFinal();
}


void Jeu::espionnage(){
	std::vector<bool> reponse = m_joueurActif->decideDefausse(m_listeJoueur);
	
	for(size_t i = 0; i < m_listeJoueur.size(); i++){
		if(std::find(m_joueursImmunises.begin(), m_joueursImmunises.end(), i) == m_joueursImmunises.end()){
			std::cout << m_listeJoueur.at(i).getPseudo() << " dévoile la 1ère carte de son deck : " << std::endl;
			m_listeJoueur.at(i).getDeck().at(0)->printCard();
			if(reponse.at(i) == true){
				m_listeJoueur.at(i).defausseCarteDeck(0);
			}		
		}
		
	}
}
	

void Jeu::verifDouve(){
  for(size_t i = 0; i < m_listeJoueur.size(); i++){
    if(m_listeJoueur.at(i).handContainsDouve()){
      std::cout << "Le joueur " << m_listeJoueur.at(i).getPseudo() << " est immunisé grâce à sa carte Douve" << std::endl;
      m_joueursImmunises.push_back(i);
    }
  }
}

void Jeu::viderImmunises(){
  m_joueursImmunises = {};
}

void Jeu::addNbMarchand(){
  m_nbMarchand++;
}
