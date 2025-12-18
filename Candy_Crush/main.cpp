#include <iostream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <fstream>
#include <iomanip>

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

//mouvements
vector<char> haut   = {'z', 'Z', 'i', 'I'};
vector<char> droite = {'d', 'D', 'l', 'L'};
vector<char> gauche = {'q', 'Q', 'j', 'J'};
vector<char> bas    = {'s', 'S', 'k', 'K'};

vector<unsigned> colors = {KNoir, KRouge, KVert, KJaune, KBleu, KMAgenta, KCyan};

void couleur(const unsigned &coul) {
    cout << "\033[" << coul << "m";
}

void clearScreen() {
    cout << "\033[H\033[2J";
}

void initGrid(mat &grid, const size_t &matSize) {
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
    //on vérifie que le joueur n'essaye pas de déplacer une case vide
    if (grid[i][j] == KImpossible);
    //on regarde dans toutes les directions autorisées.
    //on utilise find() pour parcourir les vecteurs de mouvement et trouver une correspondance.
    else if (find(haut.begin(), haut.end(), direction) != haut.end() && i > 0 && grid[i-1][j] !=KImpossible){
        swap(grid[i][j], grid[i-1][j]);
        pos.ord--;
    }
    else if ((find(bas.begin(), bas.end(), direction) != bas.end()) && i < matSize-1 && grid[i+1][j] !=KImpossible){
        swap(grid[i][j], grid[i+1][j]);
        pos.ord++;
    }
    else if (find(gauche.begin(), gauche.end(), direction) != gauche.end() && j > 0 && grid[i][j-1] !=KImpossible){
        swap(grid[i][j], grid[i][j-1]);
        pos.abs--;
    }
    else if (find(droite.begin(), droite.end(), direction) != droite.end() && j < matSize-1 && grid[i][j+1] !=KImpossible){
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

void fullGrid(mat &grid){
    for (size_t i(0); i < grid.size(); ++i){
        for (size_t j(0); j < grid.size(); ++j){
            if(grid[i][j] == KImpossible){
                grid[i][j] = rand() %KNbCandies + 1;
            }
        }
    }
}

void cleanGridBeforeGame(mat &grid) {
    maPosition pos;
    unsigned howMany = 0;
    bool found;
    do {
        if (atLeastThreeInAColumn(grid, pos, howMany)) {
            removalInColumn(grid, pos, howMany);
            fullGrid(grid);
            found = true;
        }
        else if (atLeastThreeInARow(grid, pos, howMany)) {
            removalInRow(grid, pos, howMany);
            fullGrid(grid);
            found = true;
        }
        else {
            found = false;
        }
    } while (found);
}

void progCandyCrush(){
    unsigned nbCoups = 10;
    mat grid;
    unsigned int t_mat = 8;
    srand(time(NULL));
    initGrid(grid, t_mat);
    int score = 0;
    cleanGridBeforeGame(grid); //on vérifie qu'il n'y ait pas déjà des alignements de 3
    displayGrid(grid, colors);
    string answer = "non";
    while (answer != "oui"){
        cout << "Bienvenue sur Candy Crush Basic! Voici votre grille de jeu!"
             << endl << "Vous ne pouvez pas sortir de la grille, ni échanger avec une case vide, "
                "si vous essayez, le tour sera passé. Et si votre entrée est invalide, "
                "cela provoquera l'arrêt du jeux." << endl << "entrez 'oui' afin de continuer" << endl;
        cin >> answer;
    }

    unsigned howMany = 0;

    while(nbCoups > 0){
        displayGrid(grid, colors);
        cout << "votre score : " << score << " coups restant : " <<nbCoups << endl;
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
}

void infinityCandyCrush(){
    mat grid;
    unsigned int t_mat = 8;
    srand(time(NULL));
    initGrid(grid, t_mat);
    cout << "voici la grille de jeu" << endl;
    displayGrid(grid, colors);
    int score = 0;
    cleanGridBeforeGame(grid);

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

            if (matched){
                fullGrid(grid);
                cout << "Match trouvé! Score: " << score << endl;
                displayGrid(grid, colors);
            }
        }
    }

    cout << "Fin du jeu ! Votre score final est : " << score << endl;
    couleur(KReset);
}

void pvpCandyCrush(){
    mat grid;
    unsigned int t_mat = 8;
    unsigned nbCoups = 20; //nombre de coup qui peuvent être jouer à un de plus pour affichage
    maPosition pos;
    char direction;
    srand(time(NULL));
    initGrid(grid, t_mat);

    int score_joueur1 = 0;
    int score_joueur2 = 0;
    int score = 0;
    cleanGridBeforeGame(grid); //on vérifie qu'il n'y ait pas déjà des alignements de 3
    displayGrid(grid, colors);
    string answer = "non";
    while (answer != "oui"){
        cout << "Bienvenue sur Candy Crush PVP! Voici votre grille de jeu!" << endl << "Vous ne pouvez pas sortir, ni échanger avec une case vide, si vous essayez, le tour sera passé. Et si votre entrée est invalide, cela provoquera l'arrêt du jeux." << endl << "entrez 'oui' afin de continuer" << endl;
        cin >> answer;
    }
    unsigned howMany = 0;

    while(nbCoups > 0){
        displayGrid(grid, colors);
        if (nbCoups%2==0){
            score = score_joueur1; //le score prend la valeur du score du joueur qui joue.
            cout << "Tour Joueur 1" << endl; //on prévient quel joueur doit jouer.
        }
        else { //si c'est pas pair, c'est impair
            score = score_joueur2;
            cout << "Tour Joueur 2" << endl;
        }
        cout << "Votre score : " << score << ",  et le nombre de coup(s) qu'il vous reste à jouer : " << (int)ceil(nbCoups/2.0) << endl;

        cout << "Selectionnez la ligne (entre 0 et " << t_mat-1 << "), puis sélectionnez la colonne (entre 0 et " << t_mat-1 << "), puis la direction joueur1:(z/q/s/d) joueur2:(i/j/k/l) d'après un format (haut/gauche/bas/droite)" << endl;

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
        //on sauvegarde le score sur le joueur qui vient de jouer.
        if (nbCoups%2==0){
            score_joueur1 = score;
        }
        else {
            score_joueur2 = score;
        }
        --nbCoups;
    }
    clearScreen();
    cout << "Fin du jeu ! Le score final du joueur 1 est " << score_joueur1 << endl << "Le score final du joueur 2 est " << score_joueur2 << endl;
    if (score_joueur1 == score_joueur2) {
        cout << "C'est une égalité !" << endl;
    }
    else if (score_joueur1 > score_joueur2) {
        cout << "Joueur 1 a gagné !" << endl;
    }
    else { // si c'est pas une égalité et que joueur 1 n'a pas un score supérieur alors c'est joueur 2 qui a gagné.
        cout << "Joueur 2 a gagné !" << endl;
    }
    couleur(KReset);
}


void affichageMenu(ifstream &fichier){
    string ligne;
    while (getline (fichier, ligne))
    {
        cout << ligne << endl;
    }
}
int main() {
    ifstream reglesGenerales("../../reglesGenerales.txt");
    if (!reglesGenerales){
        cout << "erreur"<< endl;
        return 1;
    }
    affichageMenu(reglesGenerales);
    string answer = "non";
    while (answer != "oui"){
        cin >> answer;
    }
    ifstream menu("../../menu.txt");
    if (!menu){
        cout << "erreur"<< endl;
        return 1;
    }
    affichageMenu(menu);
    int choix;
    cin >> choix;
    if (choix == 1) progCandyCrush();
    else if (choix == 2) infinityCandyCrush();
    else if (choix == 3) pvpCandyCrush();
    //else if (choix == 4) storyCandyCrush();
    else {
        cout << "WTF fais une entrée correcte.";
    }
    return 0;
}
