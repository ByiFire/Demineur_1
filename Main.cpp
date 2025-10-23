#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;


const int boardSize = 11;
void revealCase(int i, int j);
enum Tab {
	PLEIN,
	VIDE,
	BOMBE,
	FLAG,
	BOMBADJ,
	BORDER,


};


int Bombs[boardSize][boardSize];
int AdjCount[boardSize][boardSize];

int countAdjacentBombs(int i, int j) {
	int count = 0;
	for (int di = -1; di <= 1; ++di) {
		for (int dj = -1; dj <= 1; ++dj) {
			if (di == 0 && dj == 0) continue;
			int ni = i + di, nj = j + dj;
			if (ni <= 0 || nj <= 0 || ni >= boardSize - 1 || nj >= boardSize - 1) continue;
			if (Bombs[ni][nj] == 1) count++;
		}
	}
	return count;
}


void initBombs() {
	srand(time(0)); // initialisation du générateur aléatoire

	int totalPlayable = (boardSize - 2) * (boardSize - 2);
	int totalBombs = totalPlayable * 0.15; 
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

enum Tab Board[boardSize][boardSize];

void initBoard() {
	for (int i = 0; i < boardSize; i++) {
		for (int j = 0; j < boardSize; j++) {
			if (j == 0 || i == 0 || i == 10 || j == 10) {
				Board[i][j] = BORDER;
			}
			else {
				Board[i][j] = PLEIN;
			}
		}
	}
}

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


void PutFlag(int i, int j) {
	Board[i][j] = FLAG;
}



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

bool gameOver = false;

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


	initBoard();
	initBombs();

	printTab();

	int choice = -1;
	while (choice != 0 && !gameOver) {
		cout << "1. Print" << endl;
		cout << "2. Reveal une case" << endl;
		cout << "3. Placer un drapeau" << endl;

		cout << "0. Exit" << endl;
		cin >> choice;

		int i, j;

		switch (choice) {
		case 1:
			printTab();
			break;

		case 2:
			cout << "Choisir les coordonnees d'une case a decouvrir :";
			cin >> i >> j;
			revealCase(i, j);
			printTab();
			
			if (gameOver) {
				cout << "G A M E   O V E R !";
				revealAllBombs();
				
			}
			
			
			break;
		case 3:
			cout << "Choisir des coordonnees pour placer un drapeau :";
			cin >> i >> j;
			PutFlag(i, j);
			printTab();
			break;
		}
	}
}
