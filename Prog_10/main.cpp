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

const unsigned KReset     = 0;
const unsigned KNoir      = 30;
const unsigned KRouge     = 31;
const unsigned KVert      = 32;
const unsigned KJaune     = 33;
const unsigned KBleu      = 34;
const unsigned KMAgenta   = 35;
const unsigned KCyan      = 36;
const unsigned KImpossible = 0;

vector<unsigned> colors = {KNoir, KRouge, KVert, KJaune, KBleu, KMAgenta, KCyan};

void couleur(const unsigned &coul) {
    cout << "\033[" << coul << "m";
}

void clearScreen() {
    cout << "\033[H\033[2J";
}

void initGrid(mat &grid, const size_t &matSize, const int KNbCandies) {
    grid.resize(matSize);  //on redimensionne le nombre de lignes.
    for (size_t i = 0; i < matSize; ++i) {
        grid[i].resize(matSize); //on doit redimensionner le nombre de colonnes de chaque ligne.
        for (size_t j = 0; j < matSize; ++j)
            grid[i][j] = rand() % KNbCandies + 1;
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

void makeAMove(mat &grid, maPosition &pos, const char &direction) {
    size_t i = pos.ord;
    size_t j = pos.abs;
    size_t matSize = grid.size();
    //on regarde dans toutes les directions autorisées.
    if ((direction == 'z' || direction == 'Z') && i > 0) {
        swap(grid[i][j], grid[i - 1][j]);
        pos.ord--;
    }
    else if ((direction == 's' || direction == 'S') && i < matSize - 1) {
        swap(grid[i][j], grid[i + 1][j]);
        pos.ord++;
    }
    else if ((direction == 'q' || direction == 'Q') && j > 0) {
        swap(grid[i][j], grid[i][j - 1]);
        pos.abs--;
    }
    else if ((direction == 'd' || direction == 'D') && j < matSize - 1) {
        swap(grid[i][j], grid[i][j + 1]);
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
    unsigned col = pos.abs;
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
    unsigned ligne = pos.ord; //similaire à removalInColumn
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

void cleanGridBeforeGame(mat &grid) {
    maPosition pos;
    unsigned howMany = 0;
    bool found;
    do {
        found = false;
        
        if (atLeastThreeInAColumn(grid, pos, howMany)) {
            removalInColumn(grid, pos, howMany);
            found = true;
        }
        else if (atLeastThreeInARow(grid, pos, howMany)) {
            removalInRow(grid, pos, howMany);
            found = true;
        }
        else {
            found = false;
        }
    } while (found);
}


int main() {
    mat grid;
    unsigned int t_mat = 8;
    int KNbCandies = 6;
    unsigned nbCoups = 10;
    
    srand(time(NULL)); 
    initGrid(grid, t_mat, KNbCandies);
    int score = 0;
    cleanGridBeforeGame(grid); //on vérifie qu'il n'y ait pas déjà des alignements de 3 
    displayGrid(grid, colors);
    string answer = "non";
    while (answer != "oui"){
        cout << "Bienvenue sur Candy Crush! Voici votre grille de jeu!" << endl << "Vous ne pouvez pas sortir, si vous essayez, le tour sera passé. Et si votre entrée est invalide, cela provoquera l'arrêt du jeux." << endl << "entrez 'oui' afin de continuer" << endl;
        cin >> answer;
    }
    
    unsigned howMany = 0;

    while(nbCoups > 0){
        cout << "votre score : " << score << " coups restant : " <<nbCoups << endl;
        displayGrid(grid, colors); 
        
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
            score = score+howMany;
        }
        while (atLeastThreeInARow(grid, pos, howMany)){
            removalInRow(grid, pos, howMany);
            score = score+howMany;
        }
        --nbCoups;
    }
    
    cout << "Fin du jeu ! Votre score final est : " << score << endl;
    couleur(KReset);
    return 0;
}


