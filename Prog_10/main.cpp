#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <cmath>


using namespace std;

void clearScreen () {
    cout << "\033[H\033[2J";
}
//couleurs
const unsigned KReset   (0);
const unsigned KNoir    (30);
const unsigned KRouge   (31); //si je le mets à 41 alors le texte aura un fond rouge.
const unsigned KVert    (32);
const unsigned KJaune   (33);
const unsigned KBleu    (34);
const unsigned KMAgenta (35);
const unsigned KCyan    (36);
//mouvements
const char haut = 'z'; //peut être mis en vector si je veux avoir plus de touches correspondant aux directions.
const char gauche = 'q';
const char bas = 's';
const char droite = 'd';
//case vide
const int KImpossible = 9;
//appel couleur
void couleur (const unsigned & coul) {
    cout << "\033[" << coul <<"m";
}


typedef vector <unsigned> line; // un type représentant une ligne de la grille
typedef vector <line> mat; // un type représentant la grille
struct maPosition {
    unsigned abs; //colonnes
    unsigned ord; //lignes
};

// profil de la grille
mat initGrid (mat & grid, const size_t& matSize)
{
    grid.resize(matSize);
    for (size_t i=0; i<matSize; i++) //on passe à travers les lignes
    {
        grid[i].resize(matSize);
        for (size_t j=0; j<matSize; j++) //on passe à travers les cases dans une ligne donc colonnes
        {
            grid[i][j] = rand()%5;//obtenir un nombre entre 0 et 4 inclus.
        }
        cout << endl;
    }
    return grid;
}

void  displayGrid (const mat & grid)
{
    const vector<unsigned> lesCouleurs = {KRouge,KVert,KJaune,KMAgenta,KCyan};
    unsigned KNbCandies = 4;
    // clearScreen();
    for (vector<unsigned> line: grid) //pour toutes les vecteurs unsigned(leslignes), dans grid.
    {
        for (unsigned laCase: line) //pour toutes les cases dans line.
        {
            if (laCase > KNbCandies)
            {
                cout << "   ";
            }
            else
            {
                couleur(lesCouleurs[laCase]+10);
                cout << " " << laCase << " ";
            }
            couleur(KReset);
        }
        cout << endl;
    }
    //cette fonction doit faire appel à le fonction clearscreen()
    //remettre la couleur des caractères à sa valeur par défaut
    /*afficher case / case le contenu de la case courante.
     * Si la case n’est pas occupé par un nombre compris entre 1 et KNbCandies,
     * on affiche une case vide, sinon, on affiche le nombre courant.*/
}


//permute l'élément actif avec celui choisi.
mat makeAMove (mat & grid, const maPosition & pos, const char& direction, const size_t taille)
{
    unsigned bordmin = 0; //pour les côtés de la matrice haut et gauche
    unsigned bordmax = taille-1; //pour les côtés de la matrice bas et droite
    unsigned coordonnees = grid[pos.ord][pos.abs];
    if ((direction == haut) && pos.abs != bordmin){
        grid[pos.abs][pos.ord] = grid[pos.abs-1][pos.ord];
        grid[pos.abs-1][pos.ord] = coordonnees;
    }
    else if ((direction == gauche) && pos.ord != bordmin){
        grid[pos.abs][pos.ord] = grid[pos.abs][pos.ord-1];
        grid[pos.abs][pos.ord-1] = coordonnees;
    }
    else if ((direction == bas) && pos.abs != bordmax){
        grid[pos.abs][pos.ord] = grid[pos.abs+1][pos.ord];
        grid[pos.abs+1][pos.ord] = coordonnees;
    }
    else if ((direction == droite) && pos.ord != bordmax){
        grid[pos.abs][pos.ord] = grid[pos.abs][pos.ord+1];
        grid[pos.abs][pos.ord+1] = coordonnees;
    }
    else{
        cout << "entrez une direction valide" << endl;
    }
    return grid;
    // Cette fonction permute, dans la grille,
    // le nombre situé initialement à la position pos (aux coordonnées pos.abs, pos.ord)
    // avec la case de destination selon la valeur du caractère direction.
}

bool atLeastThreeInAColumn (const mat & grid, maPosition & pos, unsigned& howMany)
{
    for (unsigned i=0; i<grid.size(); ++i){
        howMany =1; //réinitialise lors du changement de colonnes
        for (unsigned j=0; j<grid.size(); ++j){
            unsigned valeurCase = grid[j][i]; //j en abs pour passer en colonne
            if ((j < grid.size()-1) && (valeurCase == grid[j+1][i])) //grid.size()-1 pour pas faire de débordements.
            {
                ++howMany; // y'a bien une égalité entre deux cases.
            }
            else if (howMany > 2)
            {
                pos = {j-howMany,i};
                cout << "YOU DID IT, AMAZING! COL" << j << i << " " << howMany << endl;
                return true;
            }
            else if ((j < grid.size()-1) && (valeurCase != grid[j+1][i]))
            {
                howMany = 1; // parce que la suite est interrompue on reprend depuis la nouvelle valeur comparé
            }
        }
    }
    cout << "J'aurais fais mieux, mais ok" << endl;
    return false;
    // Cette fonction parcourt la grille afin de trouver une suite d’au moins 3 nombres identiques
    // sur la même colonne. Elle renvoie vrai si une telle suite a été trouvée, faux sinon.
    // Si elle a renvoyée vrai, on a une suite de howMany tous égaux à partir des coordonnées pos.
}

bool atLeastThreeInARow (const mat & grid, maPosition & pos, unsigned& howMany)
{
    for (unsigned i=0; i<grid.size(); i++) 
    {
        howMany =1; //on réinitialise howmany à chaque changement de lignes, 1 parce qu'on compte le nombre comparé
        for (unsigned j=0; j<grid.size(); j++) 
        {
            unsigned valeurCase = grid[i][j]; // j en ord pour passer en ligne
            if ((i < grid.size()-1) && (valeurCase == grid[i][j+1])) //grid.size()-1 pour pas faire de débordements.
            {
                howMany = howMany+1; // y'a bien une égalité entre deux cases.
            }
            else if (howMany > 2)
            {
                pos = {i,j-howMany};
                cout << "YOU DID IT, AMAZING! ROW" << i << j << " " << howMany << endl;
                return true; //pour sortir
            }
            else if ((i < grid.size()-1) && (valeurCase != grid[i][j+1]))
            {
                howMany = 1; // parce que la suite est interrompue on reprend depuis la nouvelle valeur comparé
            }
        }
    }
    cout << "J'aurais fais mieux, mais ok" << endl;
    return false;
    // Cette fonction parcourt la grille afin de trouver une suite d’au moins 3 nombres identiques
    // sur la même colonne. Elle renvoie vrai si une telle suite a été trouvée, faux sinon.
    // Si elle a renvoyée vrai, on a une suite de howMany tous égaux à partir des coordonnées pos.
}

void removalInColumn (mat & grid, const maPosition & pos, unsigned& howMany)
{
    unsigned col = pos.ord;
    int start = pos.abs+1; /*le +1 c'est parce que la valeur enregistré en abscisse est celle une fois que la suite est fini*/
    int end = start + (int)howMany-1;
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
            j--;
        }
    }

}

void removalInRow (mat & grid, const maPosition & pos, unsigned& howMany)
{
    unsigned ligne = pos.abs; //similaire à removalInColumn
    int start = pos.ord+1; 
    int end = start + (int)howMany-1;
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
    // for(int i=end+1; i<(int)grid.size(); ++i){
    //     int j=i;
    //     while (j > start && grid[ligne][j-1] == KImpossible){
    //         grid[ligne][j-1] = grid[ligne][j];
    //         grid[ligne][j] = KImpossible;
    //         j--;
    //     }
    // }
}



void jouer(mat & matrice, size_t taille)
{
    displayGrid(matrice);
    cout << "selectionner la direction dans laquelle vous déplacer." << endl;
    char direction;
    cin >> direction;
    maPosition posDepart = {1,1};
    makeAMove(matrice,posDepart,direction,taille);
    displayGrid(matrice);
    unsigned howMany = 0;
    if (atLeastThreeInAColumn(matrice, posDepart, howMany)){
        cout << "colonne" << endl;
        //on supprime les éléments de la colonne
        removalInColumn(matrice, posDepart, howMany);
        displayGrid(matrice);
    }
    if (atLeastThreeInARow (matrice, posDepart, howMany)){
        cout << "ligne" << endl;
        //on supprime les éléments
        removalInRow(matrice,posDepart, howMany);
    }
    displayGrid(matrice);
}

int main()
{
    srand(time(0)); //pour que les grilles soient bien aléatoire

    couleur (KRouge);
    cout << "Rouge" << endl;
    couleur (KVert);
    cout << "Vert" << endl;
    couleur (KReset);
    cout << "Retour à la normale" << endl;
    mat matrice;
    size_t taille;
    cin >> taille;
    initGrid(matrice,taille);
    cout << "déplacez-vous avec zqsd, vous ne pouvez pas sortir du tableau" << endl;
    jouer(matrice, taille);

    // unsigned fini;
    // cin >> fini;
    // clearScreen();

    return 0;
}
//attention aux changements de couleurs, le prof à son terminal en blanc, on doit s'adapter à lui, pas l'inverse.

//pour se déplacer, il faut vraiment utiliser zqsd ou azes.
//Pour le deuxième joueur ne pas utiliser le clavier numérique parce que le prof n'a pas de clavier numérique.
//Donc privilégié les touches de guacher pour le second joueur.
