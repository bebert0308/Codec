#include "fenetreprincipale.h"
#include "ui_fenetreprincipale.h"
#include "string.h"
#include <vector>
#include <ctype.h>
#include <iostream>
#include <bitset>
#include <sstream>
#include <QApplication>
#include <QLabel>
#include <QtDebug>
#include <QSound>

string file_way;
string matrice_way;
std::string intermed1;
std::string intermed2;
//string file_size;
string file_content;
int file_size = 4;
int indice_matriciel[4];

MaFenetre::MaFenetre() : QWidget()
{
    setFixedSize(600, 200);

    QSound::play("/Users/guillaume/Downloads/musique_secu.wav");

    QLabel *image = new QLabel(this);
            image->setPixmap(QPixmap("/Users/guillaume/Desktop/cryptage_background.png"));
            image->setGeometry(0, 0, 600, 200);

    m_chargerFichier = new QPushButton("Charger un fichier", this);
    m_chargerFichier->move(100, 40);

    m_chargerMatrice = new QPushButton("Charger une matrice", this);
    m_chargerMatrice->move(95, 120);

    m_cryptage = new QPushButton("Crypter le fichier", this);
    m_cryptage->move(360, 40);

    m_decryptage = new QPushButton("Decrypter le fichier", this);
    m_decryptage->move(354, 120);


    QObject::connect(m_chargerFichier, SIGNAL(clicked(bool)), this, SLOT(chargerFichier()));
    QObject::connect(m_chargerMatrice, SIGNAL(clicked(bool)), this, SLOT(chargerMatrice()));
    QObject::connect(m_cryptage, SIGNAL(clicked(bool)), this, SLOT(cryptage()));
    QObject::connect(m_decryptage, SIGNAL(clicked(bool)), this, SLOT(decryptage()));

}

void MaFenetre::chargerFichier(){
    QString fichier = QFileDialog::getOpenFileName(this, "Ouvrir un fichier", QString(), "Fichier (*.*)");
    file_way = fichier.toStdString();
}

void MaFenetre::chargerMatrice(){
    QString matrice = QFileDialog::getOpenFileName(this, "Ouvrir une matrice", QString(), "Fichier(*.txt)");
    matrice_way = matrice.toStdString();
}

void MaFenetre::cryptage(){
    std::string lien_out;
    std::string file_content;
        string lien = file_way;
        lien_out = lien+"c";
        ifstream fichier(lien, ios::binary);
        long size_div;
        fichier.seekg(0,fichier.end);
        long size = fichier.tellg();
        fichier.seekg (0);
        size_div = size / 4;
        char lettre;
        string intermed;
        string first_part;
        string last_part;
        char symbol;

        ofstream fichier_out(lien_out, ios::out);

        for(int i= 0; i < size-1; i++){
            fichier.get(lettre);
            bitset<8> B(lettre);
            intermed = B.to_string();

            first_part = intermed.substr(0,4);
            last_part = intermed.substr(4,8);


            symbol = crypt(first_part);
            fichier_out.put(symbol);

            symbol = crypt(last_part);
            fichier_out.put(symbol);
        }
    QMessageBox::warning(this, "Félicitation", "Votre fichier à bien été crypté!");

}

void MaFenetre::decryptage(){
        char lettre;
        string last_part;
        char lettre_decrypte;
        string lien_in = file_way;


        string lien_out = lien_in.substr(0, (lien_in.size()-1));

        matrice();

        ifstream fichier(lien_in, ios::binary);

        fichier.seekg(0,fichier.end);
        long size = fichier.tellg();
        fichier.seekg (0);

        ofstream fichier_out(lien_out, ios::out);

        cout << "Taille du fichier: " << size << endl;

        for(int i = 0; i < size/2; i++){

            fichier.get(lettre);
            bitset<8> B(lettre);
            intermed1 = B.to_string();
            fichier.get(lettre);
            bitset<8> A(lettre);
            intermed2 = A.to_string();
            lettre_decrypte = decrypt();
            fichier_out.put(lettre_decrypte);

        }

        fichier.close();
        fichier_out.close();

        QMessageBox::warning(this, "Félicitation", "Votre fichier à bien été décrypté!");

}


char MaFenetre::crypt(std::string file_content){
        int block = file_size/4;                      // calcul le nombre de block a mettre
        string prepare[4][block];                     // chaine dans laquelle sera stocké les blocks de 4 bits
        int nb_1 = 0;
        int avancement = 0;
        int bit_selectionne = 0;
        string str = "";

        // Separation du binaire dans le tableau a 2 dimmension "prepare" par groupe de 4

        for(int i = 0; i < block; i++){
            for(int j = 0; j < 4; j++){
                prepare[j][i] = file_content.substr(avancement,1);
                avancement++;
            }
        }

        // permet de compter le nombre de 1 dans un groupe de 4 pour preparer le tableau de la simplification matriciel

        for(int i = 0; i < 4; i++){
            if(prepare[i][bit_selectionne] == "1")
                nb_1++;
        }

        // ------------------------------- RECUPERATION DE LA MATRICE -------------------------------

        // ouverture du fichier contenant la matrice

        ifstream fichier(matrice_way, ios::in);    // emplacement de la matrice a changer dans le programme avec GUI

        string chaine;
        int debut = 5;

        // verrification de la bonne ouverture, lecture du fichier avec enregistrement dans la variable chaine puis fermeture

        if(fichier){
            getline(fichier, chaine);
            fichier.close();
        }

        else{
            cout << "Impossible d'ouvrir le fichier !" << endl;
        }


        int matrice_size = (chaine.size()-9)/4;
        string matrice[matrice_size][4];

        for(int i = 0; i < 4; i++){
            for(int j = 0; j < matrice_size; j++){
                matrice[j][i] = chaine.substr(debut, 1);
                debut++;
            }
            debut++;
        }

        // ------------------------------- PARTIE CREATION SHORT_MATRICE -------------------------------

        string short_matrice[matrice_size][nb_1];
        int colonne = 0;

        // Inscription des lignes de la matrice utilile dans le short_matrice

        for(int i = 0; i < 4; i++){
            if(file_content[i] == '1'){
                for(int j = 0; j < matrice_size; j++){
                    short_matrice[j][colonne] = matrice[j][i];
                }
                colonne++;
            }
        }

        for(int i = 0; i < nb_1; i++){
            for(int j = 0; j < matrice_size; j++){
            }
        }

        // ------------------------------- PARTIE DE CRYPTAGE MATRICIEL -------------------------------

        int matrice_int[matrice_size][nb_1];
        int et_logique[matrice_size];
        int intermediaire;
        string finnal = "";

        // transformation de la matrice de string en int

        // c'est la que ca plante
        for(int i = 0; i < nb_1; i++){
            for(int j = 0; j < matrice_size; j++){
                matrice_int[j][i] = atoi(short_matrice[j][i].c_str());
            }
        }

        for(int i = 0; i < nb_1; i++){
            for(int j =0; j < matrice_size; j++){
            }
        }

        if(nb_1 == 1){
            for(int i = 0; i < matrice_size; i++){
                et_logique[i] = matrice_int[i][0];
            }
        }

        else{

            for(int i = 0; i < matrice_size; i++){
                intermediaire = matrice_int[i][0] ^ matrice_int[i][1];
                for(int j = 0; j < nb_1-2; j++){
                    intermediaire = intermediaire ^ matrice_int[i][j+2];
                }
                et_logique[i] = intermediaire;
            }
        }

        // affichage de la sting et_logique

        // transformation du et_logique en string

        for(int i = 0; i < matrice_size; i++){
            //cout << et_logique[i];
        }

        for(int i = 0; i < matrice_size; i++){
            finnal = finnal + std::to_string(et_logique[i]);
        }


        std::stringstream sstream(finnal);
        cout << "a = " << finnal << endl;
        while(sstream.good())
        {
            string output;
            std::bitset<8> bits;
            sstream >> bits;
            char symbol = char(bits.to_ulong());
            output += symbol;

            return symbol;
        }
        return 0;
}





void MaFenetre::matrice(){

    ifstream fichier(matrice_way, ios::in);

    string chaine;
    int debut = 5;

    if(fichier){
        getline(fichier, chaine);
        fichier.close();
    }

    else{
        cout << "Impossible d'ouvrir le fichier !" << endl;
    }


    int matrice_size = (chaine.size()-9)/4;
    string matrice[matrice_size][4];

    for(int i = 0; i < 4; i++){
        for(int j = 0; j < matrice_size; j++){
            matrice[j][i] = chaine.substr(debut, 1);
            debut++;
        }
        debut++;
    }

    // ------------------------------- RECUPERATION DE LA BASE MATRICIELLE -------------------------------


    for(int i = 0; i < matrice_size; i++){

        if(matrice[i][0] == "1" && matrice[i][1] == "0" && matrice[i][2] == "0" && matrice[i][3] == "0"){
            indice_matriciel[0] = i;
        }

        else if(matrice[i][0] == "0" && matrice[i][1] == "1" && matrice[i][2] == "0" && matrice[i][3] == "0"){
            indice_matriciel[1] = i;
        }

        else if(matrice[i][0] == "0" && matrice[i][1] == "0" && matrice[i][2] == "1" && matrice[i][3] == "0"){
            indice_matriciel[2] = i;
        }

        else if(matrice[i][0] == "0" && matrice[i][1] == "0" && matrice[i][2] == "0" && matrice[i][3] == "1"){
            indice_matriciel[3] = i;
        }


    }

    for(int i = 0; i < 4; i++){


    }

}

char MaFenetre::decrypt(){

    string char1;
    string char2;
    string data;

    for(int i = 0; i < 4; i++){
        char1 = char1+intermed1[indice_matriciel[i]];
    }

    for(int i = 0; i < 4; i++){
        char2 = char2+intermed2[indice_matriciel[i]];
    }

    data = char1+char2;

    std::stringstream sstream(data);

    std::string output;
    while(sstream.good())
    {
        std::bitset<8> bits;
        sstream >> bits;
        char c = char(bits.to_ulong());
        output += c;
        return c;
    }

    return 0;
}

//printf("net suer guillaume lagny en co")
