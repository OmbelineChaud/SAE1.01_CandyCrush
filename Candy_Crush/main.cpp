#include <iostream>
#include <vector>
#include <cstdlib>
#include <algorithm>
#include <cmath>
#include <ctime>
#include <fstream>
#include <iomanip>
#include "constantes.h"

using namespace std;

typedef vector<unsigned> line;
typedef vector<line> mat;

struct maPosition {
    unsigned ord;
    unsigned abs;
};

struct Level {
    int numero;
    string title;
    string description;
    int objectif;
    int nbCoups;
    unsigned matSize;
    unsigned NbCandiesStory;
};

//mouvements
vector<char> haut   = {'z', 'Z', 'i', 'I'};
vector<char> droite = {'d', 'D', 'l', 'L'};
vector<char> gauche = {'q', 'Q', 'j', 'J'};
vector<char> bas    = {'s', 'S', 'k', 'K'};
//liste des couleurs
vector<unsigned> colors = {KRouge, KVert, KJaune, KBleu, KMAgenta, KCyan, KNoir};

//fonctions
void couleur(const unsigned &coul) {
    cout << "\033[" << coul << "m";
}

void clearScreen() {
    cout << "\033[H\033[2J";
}

void initGrid(mat &grid, const size_t &matSize, const unsigned KNbCandies) {
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
                couleur(colors[grid[i][j]-1] + 10);
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
    //on vérifie que le joueur n'essaie pas de déplacer une case vide
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
            //on réinitialise la longueur de suite qui pouvait être enregistrée.
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
            //on réinitialise la longueur de suite qui pouvait être enregistrée.
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
    int end = start + (int)howMany-1;//int entre parenthèse met howManny à une valeur numérique int
    //on met les éléments à supprimer à la valeur KImpossible
    for (int i=start; i<=end; ++i){
        grid[i][col] = KImpossible;
    }

    //remonter les éléments qui étaient en dessous
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
    //pour chaque colonne de la suite horizontale on refait ce qu'on a fait dans removalInColumn
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

void fullGrid(mat &grid, const unsigned KNbCandies) {
    for (size_t i(0); i < grid.size(); ++i){
        for (size_t j(0); j < grid.size(); ++j){
            if(grid[i][j] == KImpossible){
                grid[i][j] = rand() %KNbCandies + 1;
            }
        }
    }
}

void affichage(ifstream &fichier){
    string ligne;
    while (getline (fichier, ligne))
    {
        cout << ligne << endl;
    }
}

void cleanGridBeforeGame(mat &grid, const unsigned KNbCandies) {
    maPosition pos;
    unsigned howMany = 0;
    while (true) {
        if (atLeastThreeInAColumn(grid, pos, howMany)) {
            removalInColumn(grid, pos, howMany);
            fullGrid(grid, KNbCandies);
            
        }
        else if (atLeastThreeInARow(grid, pos, howMany)) {
            removalInRow(grid, pos, howMany);
            fullGrid(grid, KNbCandies);
            
        }
        else {
            break;
        }
    } 
}

vector<Level> loadCampagneFromFile(const string &filename) {
    vector<Level> campagne;
    ifstream file(filename);
    if (!file) {
        cerr << "Erreur : impossible d'ouvrir " << filename << endl;
        return campagne;
    }

    string line;

        // lecture du prologue
    while (getline(file, line)) {
        if (!line.empty() && line[0] == '*') {
            cout << line << endl;
        } else {
            break; // première ligne de niveau
        }
    }
    while (getline(file, line)) {
        Level lvl;

    
        // numero
        if (!line.empty()) {
            lvl.numero = stoi(line);
        } else {
            continue; // ignore les lignes vides
        }

        // title
        if (getline(file, line)) {
            lvl.title = line;
        }
        // description
        if (getline(file, line)) {
            lvl.description = line;
        }
        // objectif
        if (getline(file, line)) {
            lvl.objectif = stoi(line);
        }
        // nbCoups
        if (getline(file, line)) {
            lvl.nbCoups = stoi(line);
        }
        // matSize
        if (getline(file, line)) {
            lvl.matSize = stoi(line);
        }
        // NbCandiesStory
        if (getline(file, line)) {
            lvl.NbCandiesStory = stoi(line);
        }
        campagne.push_back(lvl);
    }
    return campagne;
    
}

    
void progCandyCrush(){
    unsigned nbCoups = 10;
    mat grid;
    unsigned int t_mat = 8;
    initGrid(grid, t_mat, KNbCandies);
    int score = 0;
    cleanGridBeforeGame(grid, KNbCandies); //on vérifie qu'il n'y ait pas déjà des alignements de 3
    displayGrid(grid, colors);
    unsigned howMany = 0;

    while(nbCoups > 0){
        displayGrid(grid, colors);
        cout << "Votre score : " << score << " coups restant : " <<nbCoups << endl;
        maPosition pos;
        char direction;
        ifstream Tour("../tourDeJeu");
        affichage(Tour);
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
    bool stop = false;
    unsigned int t_mat = 8;
    initGrid(grid, t_mat, KNbCandies);
    cout << "voici la grille de jeu" << endl;
    displayGrid(grid, colors);
    int score = 0;
    cleanGridBeforeGame(grid, KNbCandies);

    unsigned howMany = 0;

    while(!(stop)){
        displayGrid(grid, colors);
        cout << "Votre score : " << score << endl;
        maPosition pos;
        char direction;
        ifstream Tour("../tourDeJeu");
        affichage(Tour);

        if (!(cin >> pos.ord >> pos.abs >> direction)) {
            cout << "Erreur de saisie. Fin du jeu." << endl;
            stop = true;
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
                fullGrid(grid, KNbCandies);
            }
        }
    }

    cout << "Fin du jeu ! Votre score final est : " << score << endl;
    couleur(KReset);
}

void pvpCandyCrush(){
    mat grid;
    unsigned int t_mat = 8;
    unsigned nbCoups = 20; //nombre de coups qui peuvent être joués à un de plus pour affichage
    maPosition pos;
    char direction;
    initGrid(grid, t_mat, KNbCandies);

    int score_joueur1 = 0;
    int score_joueur2 = 0;
    int score = 0;
    cleanGridBeforeGame(grid, KNbCandies); //on vérifie qu'il n'y ait pas déjà des alignements de 3
    displayGrid(grid, colors);
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

        ifstream Tour("../tourDeJeu");
        affichage(Tour);

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


void storyCandyCrush() {
    vector<Level> campagne = loadCampagneFromFile("../lore.txt");
    bool jeuEnCours = true;
    size_t i(0);

    while (jeuEnCours && i<campagne.size()) {
        const Level &lvl = campagne[i];
        cout << endl << "Ecrivez 'oui' puis appuyez sur Entrée pour commencer." << endl;
        string answer = "non";
        while (answer != "oui"){
            cin >> answer;
        }
        clearScreen();
        couleur(KJaune);
        cout << "CHAPITRE " << lvl.numero << " : " << lvl.title << endl;
        couleur(KReset);
        cout << endl << lvl.description << endl;
        cout << "OBJECTIF : " << lvl.objectif << " points | COUPS : " << lvl.nbCoups << endl;
        cout << endl << "Ecrivez 'oui' puis appuyez sur Entrée pour commencer." << endl;
        while (answer != "oui"){
            cin >> answer;
        }

        mat grid;
        initGrid(grid, lvl.matSize, lvl.NbCandiesStory);
        cleanGridBeforeGame(grid, lvl.NbCandiesStory);
        
        int score = 800;
        int coups = lvl.nbCoups;
        unsigned howMany = 0;
        while (coups > 0 && score < lvl.objectif) {
            displayGrid(grid, colors);
            cout << "votre score : " << score << " / " << lvl.objectif << " coups restants : " << coups << endl;
            maPosition pos;
            char direction;
            ifstream Tour("../tourDeJeu");
            affichage(Tour);
            if (!(cin >> pos.ord >> pos.abs >> direction)) break;
            if (pos.ord >= lvl.matSize || pos.abs >= lvl.matSize) continue;
            makeAMove(grid, pos, direction);
            bool matched = true;
            while (matched) {
                matched = false;
                while (atLeastThreeInAColumn(grid, pos, howMany)){
                    removalInColumn(grid, pos, howMany);
                    score = score + howMany*10;
                    matched = true;
                }

                while (atLeastThreeInARow(grid, pos, howMany))
                {
                    removalInRow(grid, pos, howMany);
                    score = score + howMany*10;
                    matched = true;
                }

                if (matched){
                    fullGrid(grid, lvl.NbCandiesStory);
                }
            }
            --coups;
        }

        if (score >= lvl.objectif) {
            cout << endl << "BRAVO ! Chapitre " << lvl.numero << " termine." << endl;
            if (lvl.numero < 6){
                cout << "Direction le prochain niveau";
                ++i;
            }
            else{
                cout << "FELICITATIONS ! Le Prince Bleu est sauvé !";
                jeuEnCours = false;
            }
        }

        else {
            cout << endl << "DEFAITE... L'histoire s'arrête ici." << endl;
            jeuEnCours = false;
        }
    }
}

int main() {
    srand(time(NULL));
    ifstream reglesGenerales("../reglesGenerales.txt");
    if (!reglesGenerales){
        cout << "erreur"<< endl;
        return 1;
    }
    affichage(reglesGenerales);
    string answer = "non";
    while (answer != "oui"){
        cin >> answer;
    }
    ifstream menu("../menu.txt");
    if (!menu){
        cout << "erreur bg"<< endl;
        return 1;
    }
    affichage(menu);
    int choix;
    cin >> choix;
    if (choix == 1) progCandyCrush();
    else if (choix == 2) infinityCandyCrush();
    else if (choix == 3) pvpCandyCrush();
    else if (choix == 4) storyCandyCrush();
    else {
        cout << "L'entrée saisie ne fait pas partie des possibilités.";
    }
    return 0;
}
