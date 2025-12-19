#include "display.h"
#include "constantes.h"
#include <iostream>

using namespace std;

void affichage(ifstream &fichier){
    string ligne;
    while (getline (fichier, ligne))
    {
        cout << ligne << endl;
    }
}

void displayGrid(const mat &grid, const vector<unsigned> &colors) {
    clearScreen();
    couleur(KReset);
    cout << "  |";
    for (size_t j = 0; j < grid[0].size(); ++j) {
        cout << " " << j << " ";
    }
    cout << endl;

    for (size_t i = 0; i < grid.size(); ++i) {
        cout << i << " |";
        for (size_t j = 0; j < grid[i].size(); ++j) {
            if (grid[i][j] == KImpossible) {
                cout << "   ";
            }
            else {
                couleur(colors[grid[i][j]] + 10);
                cout << " " << grid[i][j] << " ";
                couleur(KReset);
            }
        }
        cout << endl;
    }
    cout << endl << endl;
}
