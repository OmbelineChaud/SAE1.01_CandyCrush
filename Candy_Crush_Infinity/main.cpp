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
    grid.resize(matSize);
    for (size_t i = 0; i < matSize; ++i) {
        grid[i].resize(matSize);
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
                couleur(colors[grid[i][j] - 1] + 10); 
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
    else {
        cout << "Coup invalide ou hors des limites !" << endl;
    }
}

bool atLeastThreeInAColumn(const mat &grid, maPosition &pos, unsigned &howMany) {
    for (size_t j = 0; j < grid[0].size(); ++j) { 
        for (size_t i = 0; i <= grid.size() - 3; ++i) { 
            if (grid[i][j] == KImpossible) continue;
            
            howMany = 1;
            size_t k = i + 1;
            while (k < grid.size() && grid[i][j] == grid[k][j]) {
                ++howMany;
                ++k;
            }
            
            if (howMany >= 3) {
                pos.ord = i;
                pos.abs = j;
                return true;
            }
        }
    }
    return false;
}

bool atLeastThreeInARow(const mat &grid, maPosition &pos, unsigned &howMany) {
    for (size_t i = 0; i < grid.size(); ++i) { 
        for (size_t j = 0; j <= grid[i].size() - 3; ++j) { 
            if (grid[i][j] == KImpossible) continue;
            
            howMany = 1;
            size_t k = j + 1;
            while (k < grid[i].size() && grid[i][j] == grid[i][k]) {
                ++howMany;
                ++k;
            }
            
            if (howMany >= 3) {
                pos.ord = i;
                pos.abs = j;
                return true;
            }
        }
    }
    return false;
}

void removalInColumn(mat &grid, const maPosition &pos, unsigned howMany) {
    for (unsigned i = 0; i < howMany; ++i) {
        grid[pos.ord + i][pos.abs] = KImpossible;
    }
}

void removalInRow(mat &grid, const maPosition &pos, unsigned howMany) {
    for (unsigned j = 0; j < howMany; ++j) {
        grid[pos.ord][pos.abs + j] = KImpossible;
    }
}

void applyGravity(mat &grid) {
    size_t matSize = grid.size();

    for (size_t j = 0; j < matSize; ++j) {
        std::vector<unsigned> non_impossible_candies;
        
        for (size_t i = 0; i < matSize; ++i) {
            if (grid[i][j] != KImpossible) {
                non_impossible_candies.push_back(grid[i][j]);
            }
        }

        size_t num_impossible = matSize - non_impossible_candies.size();
        
        for (size_t i = 0; i < num_impossible; ++i) {
            grid[i][j] = KImpossible;
        }

        for (size_t i = 0; i < non_impossible_candies.size(); ++i) {
            grid[num_impossible + i][j] = non_impossible_candies[i];
        }
    }
}

void fillNewCandies(mat &grid, const int KNbCandies) {
    size_t matSize = grid.size();
    for (size_t j = 0; j < matSize; ++j) {
        for (size_t i = 0; i < matSize; ++i) {
            if (grid[i][j] == KImpossible) {
                grid[i][j] = rand() % KNbCandies + 1;
            }
        }
    }
}

void cleanGridBeforeGame(mat &grid, const int KNbCandies) {
    maPosition pos;
    unsigned howMany = 0;
    bool found;

    do {
        found = false;
        
        while (atLeastThreeInAColumn(grid, pos, howMany)) {
            removalInColumn(grid, pos, howMany);
            found = true;
        }
        while (atLeastThreeInARow(grid, pos, howMany)) {
            removalInRow(grid, pos, howMany);
            found = true;
        }

        if (found) {
            applyGravity(grid);
            fillNewCandies(grid, KNbCandies);
        }
    } while (found);
}

int main() {
    mat grid;
    unsigned int t_mat = 8;
    int KNbCandies = 6;
    
    srand(time(NULL)); 
    initGrid(grid, t_mat, KNbCandies);
    cout << "voici la grille de jeu" << endl;
    displayGrid(grid, colors);

    int score = 0;
    cleanGridBeforeGame(grid, KNbCandies);
    
    unsigned howMany = 0;

    while(score < 100){
        
        cout << "votre score : " << score << endl;
        displayGrid(grid, colors); 
        
        maPosition pos;
        char direction;
        cout << "Selectionnez la ligne (entre 0 et " << t_mat-1
        << "), puis la colonne (entre 0 et " << t_mat-1
        << "), puis la direction (z/q/s/d)" << endl;
        
        if (!(cin >> pos.ord >> pos.abs >> direction)) {
             cout << "Erreur de saisie. Fin du jeu." << endl;
             break;
        }
        if (pos.ord >= t_mat || pos.abs >= t_mat) {
             cout << "Position hors limites. Reessayez." << endl;
             continue;
        }

        makeAMove(grid, pos, direction);
        
        bool matched = true;
        while (matched) {
            matched = false;
            
            while (atLeastThreeInAColumn(grid, pos, howMany)){
                removalInColumn(grid, pos, howMany);
                score = score + howMany;
                matched = true;
            }
            
            while (atLeastThreeInARow(grid, pos, howMany))
            {
                removalInRow(grid, pos, howMany);
                score = score + howMany;
                matched = true;
            }
            
            if (matched) {
                applyGravity(grid); 
                fillNewCandies(grid, KNbCandies);
                
                cout << "Match trouvé! Score: " << score << endl;
                displayGrid(grid, colors); 
            }
        }
    }
    
    cout << "Fin du jeu ! Votre score final est : " << score << endl;
    couleur(KReset);
    return 0;
}
