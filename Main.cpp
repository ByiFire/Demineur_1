#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;


int boardSize;

// enum pour éviter d'avoir trop de variables

enum Tab {
	PLEIN,
	VIDE,
	BOMBE,        
	FLAG,
	BOMBADJ,
	BORDER,
};
// taille maximale des tableaux en prenant en compte les bordures

int Bombs[32][32];
int AdjCount[32][32];		

// fait en sorte que l'utilisateur tape ce qui lui est demandé

int safeInput(const string& message) {
	int val;
	cout << message;
	while (!(cin >> val)) {
		cin.clear();								
		cin.ignore(10000, '\n');
		cout << "Entree invalide, reessayez : ";
	}
	return val;
}

//Choix de la taille du board selon la difficulté, on ajoute toujours +2 dû aux bordures

void chooseDifficulty() {
	cout << "Choisissez une difficulte :" << endl;
	cout << "1. Facile (9x9)" << endl;
	cout << "2. Moyen (16x16)" << endl;						
	cout << "3. Difficile (30x16)" << endl;
	int choice = safeInput("> ");

	switch (choice) {
	case 1:
		boardSize = 11;
		break;

	case 2:										
		boardSize = 18;
		break;
	case 3: 
		boardSize = 32;
		break;
	default:
		boardSize = 11;
		cout << "Choix invalide, mode facile par defaut." << endl;
		break;
	}
}




// Fonction permettant de calculer le nombre de bombes adjacentes à une case révélée non vide

int countAdjacentBombs(int i, int j) {
	int count = 0;
	for (int di = -1; di <= 1; ++di) { // parcours du tableau
		for (int dj = -1; dj <= 1; ++dj) {
			if (di == 0 && dj == 0) continue; //si case vide on continue de parcourir					
			int ni = i + di, nj = j + dj;
			if (ni <= 0 || nj <= 0 || ni >= boardSize - 1 || nj >= boardSize - 1) continue;
			if (Bombs[ni][nj] == 1) count++;
		}
	}
	return count;
}


void initBombs() {
	srand(time(0)); // initialisation du générateur aléatoire

	int totalPlayable = (boardSize - 2) * (boardSize - 2); // nombre de bombes placées à l'intérieur du tableau
	int totalBombs = totalPlayable * 0.15; //15% du tableau est une bombe
	int placed = 0;

	for (int i = 0; i < boardSize; i++) {
		for (int j = 0; j < boardSize; j++) {
			if (i == 0 || j == 0 || i == boardSize - 1 || j == boardSize - 1) {
				Bombs[i][j] = 2; // la bordure
			}
			else {
				Bombs[i][j] = 0; // on vide tout avant
			}
		}
	}
	
	// Placement aléatoire des bombes à l'intérieur
	while (placed < totalBombs) {
		int i = rand() % (boardSize - 2) + 1; // évite les bordures
		int j = rand() % (boardSize - 2) + 1;

		if (Bombs[i][j] == 0) { // seulement si la case est vide
			Bombs[i][j] = 1;
			placed++;
		}
	}

	//compteur de bombes adjacentes
	for (int i = 1; i < boardSize - 1; i++) {
		for (int j = 1; j < boardSize - 1; j++) {
			if (Bombs[i][j] != 1) {
				AdjCount[i][j] = countAdjacentBombs(i, j);
			}
			else {
				AdjCount[i][j] = -1; // marque les bombes
			}
		}
	}


}

enum Tab Board[32][32];
// initialisation du tableau d'affichage 
void initBoard() {
	for (int i = 0; i < boardSize; i++) {
		for (int j = 0; j < boardSize; j++) {
			if (i == 0 || j == 0 || i == boardSize - 1 || j == boardSize - 1) {
				Board[i][j] = BORDER;
				
			}
			else {
				Board[i][j] = PLEIN;
			}
		}
	}
}
// ce que la console affichera lorsqu'on va print le tableau
void printTab() {
	for (int i = 0; i < boardSize; i++) {
		for (int j = 0; j < boardSize; j++) {
			switch (Board[i][j]) {
			case PLEIN:
				cout << "[ ]";
				break;
			case VIDE:
				cout << "   ";
				break;
			case FLAG:
				cout << " F ";
				break;
			case BOMBE:
				cout << " * ";
				break;
			case BORDER:
				cout << " # ";
				break;
			case BOMBADJ: 
				cout << " " << AdjCount[i][j] << " ";
				break;


			default:
				break;
			}
		}
		cout << endl;
	}
}

//fonction permettant de poser/enlever un drapeau d'une case jugée suspecte
void PutFlag(int i, int j) {
    if (Board[i][j] == FLAG) {
        Board[i][j] = PLEIN;   // retire le drapeau si déjà présent
    } 
    else if (Board[i][j] == PLEIN || Board[i][j] == BOMBADJ) {
        Board[i][j] = FLAG;    // place un drapeau sur une case encore non révélée
    }
    // les cases révélées (VIDE, BOMBE) restent inchangées
}




//fonction réccursive permettant de révéler les cases adjacentes si la case à révéler choisie ne possède aucune bombe autour d'elle
void CaseAdj(int i, int j) {
	if (i <= 0 || j <= 0 || i >= boardSize - 1 || j >= boardSize - 1) return;
	if (Board[i][j] != PLEIN) return;
	if (Bombs[i][j] == 1) return; // ne pas révéler une bombe

	int adj = AdjCount[i][j];

	if (adj > 0) {
		Board[i][j] = BOMBADJ;
	}
	else {
		Board[i][j] = VIDE;
		for (int di = -1; di <= 1; ++di)
			for (int dj = -1; dj <= 1; ++dj)
				if (di != 0 || dj != 0)
					CaseAdj(i + di, j + dj);
	}
}

//fonction permettant de vérifier si toutes les cases sûres sont découvertes
bool gameOver = false;
bool checkVictory() {
	for (int i = 1; i < boardSize - 1; i++) {
		for (int j = 1; j < boardSize - 1; j++) {
			if (Bombs[i][j] == 0 && Board[i][j] == PLEIN)
				return false; // une case sûre n’a pas encore été révélée
		}
	}
	return true; // toutes les cases sûres sont découvertes
}

//en cas de défaite, fonction affichant l'entièreté des bombes du tableau lors de la défaite
void revealAllBombs() {
	for (int i = 0; i < boardSize; i++) {
		for (int j = 0; j < boardSize; j++) {
			if (Bombs[i][j] == 1) {
				Board[i][j] = BOMBE;
			}
		}
	}
	printTab();
}



//fonction permettant de révéler une case choisie en fonction de coordonnées
void revealCase(int i, int j) {
	if (Bombs[i][j] == 1) {
		cout << "\n BOMBE !\n";
		Board[i][j] = BOMBE;
		gameOver = true;
		return;
	}

	CaseAdj(i, j);
}




int main() {

	chooseDifficulty();
	initBoard();
	initBombs();
	printTab();

	int choice = -1;
	while (choice != 0 && !gameOver) {
		choice = safeInput("\n1. Print\n2. Reveal une case\n3. Placer un drapeau\n0. Exit\n> ");

		int i, j;

		switch (choice) {
		
		case 1:
			printTab();
			break;

		case 2:
			cout << "Choisir les coordonnees d'une case a decouvrir :";
			i = safeInput("Ligne (i) : ");
			j = safeInput("Colonne (j) : ");
			revealCase(i, j);
			printTab();
			
			if (gameOver) {
				cout << "\nG A M E   O V E R !\n";
				revealAllBombs();
			}
			else if (checkVictory()) {
				cout << "\nVICTOIRE ! Toutes les cases sures ont ete decouvertes !\n";
				gameOver = true; 
				revealAllBombs(); 
			}

			
			
			break;
		case 3:
			i = safeInput("Ligne (i) : ");
			j = safeInput("Colonne (j) : ");
			if (i > 0 && i < boardSize - 1 && j > 0 && j < boardSize - 1) {
				PutFlag(i, j);
				printTab();
			}
			else {
				cout << "Coordonnees hors limites.\n";
			}
			break;
		case 0:
			cout << "Sortie du jeu.\n";
			break;
		default:
			cout << "Choix invalide.\n";
			break;
		}
	}
}
