#include <iostream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <ctime>

using namespace std;

typedef vector<unsigned> line;
typedef vector<line> mat;

struct maPosition {
    unsigned ord;
    unsigned abs;
};

const unsigned KReset      = 0;
const unsigned KNoir       = 30;
const unsigned KRouge      = 31;
const unsigned KVert       = 32;
const unsigned KJaune      = 33;
const unsigned KBleu       = 34;
const unsigned KMAgenta    = 35;
const unsigned KCyan       = 36;
const unsigned KImpossible = 0;
const int      KNbCandies  = 6;

vector<unsigned> colors = {KNoir, KRouge, KVert, KJaune, KBleu, KMAgenta, KCyan};

void couleur(const unsigned &coul) {
    cout << "\033[" << coul << "m";
}

void clearScreen() {
    cout << "\033[H\033[2J";
}

void initGridLevel_1(mat & grid, unsigned &nbCoups) {
    grid = {{2, 1, 1, 3},{1, 2, 3, 2},{4, 3, 2, 1},{3, 4, 4, 4}};
    nbCoups = 3;
}

void restartLevel(mat &grid, const mat &levelGrid, unsigned &nbCoups, const unsigned &initialNbCoups) {
    grid = levelGrid; // Réinitialise la grille de jeu à son état initial
    nbCoups = initialNbCoups; // Réinitialise le nombre de coups
}
bool levelCompleted(const mat &grid) {
    for (size_t i = 0; i < grid.size(); ++i) {
        for (size_t j = 0; j < grid[i].size(); ++j) {
            if (grid[i][j] != KImpossible) {
                return false; // Il reste des bonbons dans la grille, le niveau n'est pas fini
            }
        }
    }
    return true; // Tous les bonbons sont supprimés, la grille de jeu est vide
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


void makeAMove(mat &grid, maPosition &pos, const char &direction) {
    size_t i = pos.ord;
    size_t j = pos.abs;
    size_t matSize = grid.size();
    //on vérifie que le joueur n'essaye pas de déplacer une case vide.
    if (grid[i][j] == KImpossible);
    //on regarde dans toutes les directions autorisées.
    else if ((direction == 'z' || direction == 'Z') && i > 0 && grid[i-1][j] !=KImpossible){
        swap(grid[i][j], grid[i-1][j]);
        pos.ord--;
    }
    else if ((direction == 's' || direction == 'S') && i < matSize-1 && grid[i+1][j] !=KImpossible){
        swap(grid[i][j], grid[i+1][j]);
        pos.ord++;
    }
    else if ((direction == 'q' || direction == 'Q') && j > 0 && grid[i][j-1] !=KImpossible){
        swap(grid[i][j], grid[i][j-1]);
        pos.abs--;
    }
    else if ((direction == 'd' || direction == 'D') && j < matSize-1 && grid[i][j+1] !=KImpossible){
        swap(grid[i][j], grid[i][j+1]);
        pos.abs++;
    }
    //la direction donnée n'était pas valide.
    else {
        cout << "Coup invalide ou hors des limites !" << endl;
    }
}

bool atLeastThreeInAColumn(const mat &grid, maPosition &pos, unsigned &howMany) {
    for (unsigned j = 0; j < grid[0].size(); ++j) {
        for (unsigned i = 0; i <= grid.size() - 3; ++i) {
            if (grid[i][j] == KImpossible) continue; //pour ne pas considérer une suite de KImpossible comme une suite.
            //on réinitialise la longueur de suite qui pouvait être enregistré.
            howMany = 1;
            size_t k = i + 1;
            //on regarde la longueur de la suite.
            while (k < grid.size() && grid[i][j] == grid[k][j]) {
                ++howMany;
                ++k;
            }
            //si la suite est d'au moins 3 de long.
            if (howMany >= 3) {
                pos = {i,j};
                return true;
            }
        }
    }
    return false;
}

bool atLeastThreeInARow(const mat &grid, maPosition &pos, unsigned &howMany) {
    for (unsigned i = 0; i < grid.size(); ++i) {
        for (unsigned j = 0; j <= grid[i].size() - 3; ++j) {
            if (grid[i][j] == KImpossible) continue; //pour ne pas considérer une suite de KImpossible comme une suite.
            //on réinitialise la longueur de suite qui pouvait être enregistré.
            howMany = 1;
            size_t k = j + 1;
            //on regarde la longueur de la suite.
            while (k < grid[i].size() && grid[i][j] == grid[i][k]) {
                ++howMany;
                ++k;
            }
            //si la suite est d'au moins 3 de long.
            if (howMany >= 3) {
                pos = {i,j};
                return true;
            }
        }
    }
    return false;
}

void removalInColumn(mat &grid, const maPosition &pos, unsigned &howMany) {
    int col = pos.abs;
    int start = pos.ord;
    int end = start + (int)howMany-1;//int entre parenthèse met howManny à une valeur numérique int.
    //on met les éléments à supprimer à valeur KImpossible
    for (int i=start; i<=end; ++i){
        grid[i][col] = KImpossible;
    }

    //remonter les éléments qui était en dessous.
    for(int i=end+1; i<(int)grid.size(); ++i){
        int j=i;
        while (j > start && grid[j-1][col] == KImpossible){
            grid[j-1][col] = grid[j][col];
            grid[j][col] = KImpossible;
            --j;
        }
    }
}

void removalInRow(mat &grid, const maPosition &pos, unsigned &howMany) {
    int ligne = pos.ord; //similaire à removalInColumn
    int start = pos.abs;
    int end = start + (int)howMany-1; //int entre parenthèse met howManny à une valeur numérique int.
    //on met les éléments à supprimer à valeur KImpossible
    for (int i=start; i<=end; ++i){
        grid[ligne][i] = KImpossible;
    }
    //pour chaque colonne de la suite horizontale on refait ce qu'on a fait dans removalColumn
    for(int col=start; col<=end; ++col){
        for (int i=ligne+1; i<(int)grid.size(); ++i){
            int j=i;
            while (j > ligne && grid[j-1][col] == KImpossible){
                grid[j-1][col] = grid[j][col];
                grid[j][col] = KImpossible;
                --j;
            }
        }
    }
}


int main() {
    
    cout << "Voici le mode histoire, ecrivez \"commencer\" pour commencer." << endl;
    string start;
    cin >> start;
    if (start == "commencer") {
        mat grid;
        
        unsigned howMany = 0;
        unsigned nbCoups = 0;
        initGridLevel_1(grid, nbCoups);
        const mat levelGrid = grid;
        const unsigned initialNbCoups = nbCoups;
        unsigned t_mat = grid.size();
        cout << "Bienvenue sur Candy Crush Histoire !" << endl;
        while(nbCoups > 0){
            cout << "Affichage de la grille :" << endl;
            displayGrid(grid, colors);
            cout << " coups restant : " << nbCoups << endl;
            maPosition pos;
            char direction;
            cout << "Selectionnez la ligne (entre 0 et " << t_mat-1 << "), puis la colonne (entre 0 et " << t_mat-1 << "), puis la direction (z/q/s/d)" << endl;

            if (!(cin >> pos.ord >> pos.abs >> direction)) {
                cout << "Erreur de saisie. Fin du jeu." << endl;
                break;
            }
            if (pos.ord >= t_mat || pos.abs >= t_mat) {
                cout << "Position hors limites. Reessayez." << endl;
                continue;
            }

            makeAMove(grid, pos, direction);

            while (atLeastThreeInAColumn(grid, pos, howMany)){
                removalInColumn(grid, pos, howMany);
            }
            while (atLeastThreeInARow(grid, pos, howMany)){
                removalInRow(grid, pos, howMany);
            }
            --nbCoups;
            if (levelCompleted(grid)) {
                displayGrid(grid, colors);
                cout << "Félicitations ! Vous avez terminé le niveau !" << endl;
                //ici on pourrait passer au niveau suivant
            }
            else if (!(levelCompleted(grid)) && nbCoups == 0) {
                displayGrid(grid, colors);
                cout << "Il reste des bonbons dans la grille ! " << endl;
                string restart = "non";
                cout << "Voulez vous recommencer le niveau ?" << endl << "entrez 'oui' pour recommencer, 'non' pour continuer" << endl;
                cin >> restart;
                if (restart == "oui") {
                    restartLevel(grid, levelGrid, nbCoups, initialNbCoups);
                }
            }
        } 
    }
    else{
        cout << "Problème détecté" << endl;
    }
}

