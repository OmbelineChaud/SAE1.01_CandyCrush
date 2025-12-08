#include <iostream>
#include <vector>
#include <random>


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
    unsigned abs;
    unsigned ord;
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
            const vector<unsigned> lesCouleurs = {KNoir,KRouge,KVert,KJaune,KMAgenta,KCyan};
            couleur(lesCouleurs[laCase]+10);
            cout << " " << laCase << " ";
            couleur(KReset);
            }
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
    unsigned coordonnees = grid[pos.abs][pos.ord];
    if ((direction == haut) && pos.ord != bordmin)
    {
        grid[pos.abs][pos.ord] = grid[pos.abs][pos.ord-1];
        grid[pos.abs][pos.ord-1] = coordonnees;
    }
    else if ((direction == gauche) && pos.abs != bordmin)
    {
        grid[pos.abs][pos.ord] = grid[pos.abs][pos.ord-1];
        grid[pos.abs-1][pos.ord] = coordonnees;
    }
    if ((direction == bas) && pos.ord != bordmax)
    {
        grid[pos.abs][pos.ord] = grid[pos.abs][pos.ord+1];
        grid[pos.abs][pos.ord+1] = coordonnees;
    }
    else if ((direction == gauche) && pos.abs != bordmin)
    {
        grid[pos.abs][pos.ord] = grid[pos.abs][pos.ord+1];
        grid[pos.abs+1][pos.ord] = coordonnees;
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
    for (size_t i=0; i<grid.size(); i++) //colonnes
    {
        for (size_t j=0; j<grid.size(); j++) //lignes
        {
            //grid[j][i] // j étant en premier on va passer dans la case i de toutes les lignes(donc colonnes)
            if ((i < grid.size()-2) && (grid[j][i] == grid[j][i+1]) && (grid[j][i] == grid[j][i+2])) //grid.size()-2 pour pas faire les deux dernières lignes vu qu'on pourra pas avoir une suite de 3 dessus.
            {
                howMany = {grid[j][i], grid[j][i+1], grid[j][i+2]};
                return true; // c'est un bool, je peux pas dire combien de combinaisons sont faites.
            }
        }
        cout << endl;
    }
    // Cette fonction parcourt la grille afin de trouver une suite d’au moins 3 nombres identiques
    // sur la même colonne. Elle renvoie vrai si une telle suite a été trouvée, faux sinon.
    // Si elle a renvoyée vrai, on a une suite de howMany tous égaux à partir des coordonnées pos.
}

// bool atLeastThreeInARow (const mat & grid, maPosition & pos, unsigned & howMany)
// {
//     for (size_t i=0; i<grid.size(); i++) //lignes
//     {
//         for (size_t j=0; j<grid.size(); j++) //colonnes
//         {
//             if ((j < grid.size()-2) && (grid[i][j] == grid[i][j+1]) && (grid[i][j] == grid[i][j+2]))
//             {
//                 howMany = {grid[i][j], grid[i][j+1], grid[i][j+2]};
//                 return true; // c'est un bool, je peux pas dire combien de combinaisons sont faites.
//             }
//         }
//         cout << endl;
//     }
// }






void jouer(mat & matrice, size_t taille) //mon main devenait trop chargé c'est plus super lisible, en plus faudra que je puisse répeter.
{
    displayGrid(matrice);
    cout << "selectionner la direction dans laquelle vous déplacer." << endl;
    char direction;
    cin >> direction;
    unsigned depart = 1;
    maPosition posDepart = {depart,depart};
    makeAMove(matrice,posDepart,direction,taille);
    displayGrid(matrice);
    unsigned jeux = matrice[pos.abs][pos.ord]
    if (atLeastThreeInAColumn(matrice, posDepart, jeux))
    {
        cout << "Colonnes yahooo!";
    }
    // if (atLeastThreeInARow(matrice, posDepart,howMany))
    // {
    //     cout << "lignes yahooo!";
    // }
}

int main()
{
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

