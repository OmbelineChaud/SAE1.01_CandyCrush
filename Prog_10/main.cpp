#include <iostream>
#include <vector>
#include <random>
#include <ctime>


using namespace std;

void clearScreen () {
    cout << "\033[H\033[2J";
}
//*
const unsigned KReset   (0);
const unsigned KNoir    (30);
const unsigned KRouge   (31); //si je le mets à 41 alors le texte aura un fond rouge.
const unsigned KVert    (32);
const unsigned KJaune   (33);
const unsigned KBleu    (34);
const unsigned KMAgenta (35);
const unsigned KCyan    (36);

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
            grid[i][j] = rand()%6;//obtenir un nombre entre 0 et 9 inclus.
        }
        cout << endl;
    }
    return grid;
}

void  displayGrid (const mat & grid)
{
    const vector<unsigned> lesCouleurs = {KNoir,KRouge,KVert,KJaune,KMAgenta,KCyan};
    unsigned KNbCandies = 5;
    // clearScreen();
    for (vector<unsigned> line: grid) //pour toutes les vecteurs unsigned(leslignes), dans grid.
    {
        for (unsigned laCase: line) //pour toutes les cases dans line.
        {
            if (laCase < 1 || laCase > KNbCandies)
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

//mouvements
const char haut = 'z'; //peut être mis en vector si je veux avoir plus de touches correspondant aux directions.
const char gauche = 'q';
const char bas = 's';
const char droite = 'd';


//permute l'élément actif avec celui choisi.
mat makeAMove (mat & grid, const maPosition & pos, const char& direction, const size_t taille)
{
    unsigned bordmin = 0; //pour les côtés de la matrice haut et gauche
    unsigned bordmax = taille-1; //pour les côtés de la matrice bas et droite
    unsigned coordonnees = grid[pos.ord][pos.abs];
    if ((direction == haut) && pos.abs != bordmin)
    {
        grid[pos.ord][pos.abs] = grid[pos.ord-1][pos.abs];
        grid[pos.ord-1][pos.abs] = coordonnees;
    }
    else if ((direction == gauche) && pos.ord != bordmin)
    {
        grid[pos.ord][pos.abs] = grid[pos.ord][pos.abs-1];
        grid[pos.ord][pos.abs-1] = coordonnees;
    }
    if ((direction == bas) && pos.abs != bordmax)
    {
        grid[pos.ord][pos.abs] = grid[pos.ord+1][pos.abs];
        grid[pos.ord+1][pos.abs] = coordonnees;
    }
    else if ((direction == droite) && pos.ord != bordmax)
    {
        grid[pos.ord][pos.abs] = grid[pos.ord][pos.abs+1];
        grid[pos.ord][pos.abs+1] = coordonnees;
    }
    else
    {
        cout << "entrez une direction valide" << endl;
    }
    return grid;
// Cette fonction permute, dans la grille,
// le nombre situé initialement à la position pos (aux coordonnées pos.abs, pos.ord)
// avec la case de destination selon la valeur du caractère direction.
}

bool atLeastThreeInAColumn (const mat & grid, maPosition & pos, unsigned& howMany)
{
    for (unsigned j=0; j<grid.size(); j++) //colonnes
    {
        howMany =1; //on réinitialise howmany à chaque changement de colonnes, 1 parce qu'on compte le nombre comparé
        for (unsigned i=0; i<grid.size(); i++) //lignes
        {
            unsigned valeurCase = grid[i][j]; // j étant en premier on va passer dans la case i de toutes les lignes(donc colonnes)
            if ((i < grid.size()-1) && (valeurCase == grid[j][i+1])) //grid.size()-1 pour pas faire de débordements.
            {
                howMany = howMany+1; // y'a bien une égalité entre deux cases.
            }
            else if (howMany > 2)
            {
                pos = {j,i-howMany};
                cout << "YOU DID IT, AMAZING! COL" << j << " " << howMany << endl;
                return true;
            }
            else if ((i < grid.size()-1) && (valeurCase != grid[j][i+1]))
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
    for (unsigned j=0; j<grid.size(); j++) //lignes
    {
        howMany =1; //on réinitialise howmany à chaque changement de lignes, 1 parce qu'on compte le nombre comparé
        for (unsigned i=0; i<grid.size(); i++) //colonnes
        {
            unsigned valeurCase = grid[i][j];// j étant en premier on va passer dans la case i de toutes les lignes(donc colonnes)
            if ((i < grid.size()-1) && (valeurCase == grid[i+1][j])) //grid.size()-1 pour pas faire de débordements.
            {
                howMany = howMany+1; // y'a bien une égalité entre deux cases.
            }
            else if (howMany > 2)
            {
                pos = {i-howMany,j};
                cout << "YOU DID IT, AMAZING! ROW" << j << " " << howMany << endl;
                return true;
            }
            else if ((i < grid.size()-1) && (valeurCase != grid[i+1][j]))
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



void jouer(mat & matrice, size_t taille)
{
    displayGrid(matrice);
    cout << "selectionner la direction dans laquelle vous déplacer." << endl;
    char direction;
    cin >> direction;
    maPosition posDepart = {2,4};
    makeAMove(matrice,posDepart,direction,taille);
    displayGrid(matrice);
    unsigned howMany = 0;
    atLeastThreeInAColumn(matrice, posDepart, howMany);
    atLeastThreeInARow (matrice, posDepart, howMany);
    // if (atLeastThreeInARow(matrice, posDepart,howMany))
    // {
    //     cout << "lignes yahooo!";
    // }
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
