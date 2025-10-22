
// tesy

#include <iostream>
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


int Bombs[boardSize][boardSize] = {  2,2,2,2,2,2,2,2,2,2,2,
									 2,0,0,0,0,0,0,0,0,0,2,
									 2,0,0,1,1,0,0,0,1,0,2,
									 2,0,0,0,1,0,1,0,1,0,2,
									 2,0,1,0,1,0,0,1,0,0,2,
									 2,0,1,0,0,1,0,0,1,0,2,
									 2,0,0,1,0,0,0,1,0,0,2,
									 2,0,0,0,0,0,0,0,0,0,2,
									 2,0,0,0,0,0,1,1,1,0,2,
									 2,0,0,0,0,0,0,0,0,0,2,
									 2,2,2,2,2,2,2,2,2,2,2, };

//void initBombs() {
//	for (int i = 0; i < boardSize; i++) {
//		for (int j = 0; j < boardSize; j++) {
//			Bombs[i][j] = 0;
//		}
//	}
//}

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
				cout << " " << Bombs[i][j] << " "; 
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

int countAdjacentBombs(int i, int j) {
	int count = 0;
	for (int di = -1; di <= 1; ++di) {
		for (int dj = -1; dj <= 1; ++dj) {
			if (di == 0 && dj == 0) continue;
			if (Bombs[i + di][j + dj] == 1) count++;
		}
	}
	return count;
}

void CaseAdj(int i, int j) {
	if (i <= 0 || j <= 0 || i >= boardSize - 1 || j >= boardSize - 1) return;
	if (Board[i][j] != PLEIN) return;
	if (Bombs[i][j] == 1) return; // ne pas révéler une bombe

	int adj = countAdjacentBombs(i, j);

	if (adj > 0) {
		Board[i][j] = BOMBADJ;
		Bombs[i][j] = adj;
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
}




void revealCase(int i, int j) {


	if (Bombs[i][j] == 0) {
		CaseAdj(i, j);

	}
	else {
		cout << "\n BOMBE !\n";
		Board[i][j] = BOMBE;
		gameOver = true;
		return;
	}
	CaseAdj(i, j);
}




int main() {


	initBoard();
	//initBombs();

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
				choice = 0;
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
