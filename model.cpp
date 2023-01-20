#include "model.h"

void Model::parseObj(std::string const &path) {
    std::vector<Vecteur> vecteursMesh{};
    std::vector<Vecteur> vecteursTextures{};
    std::ifstream myfile(path);
    std::string ligne{};

    if (!myfile) {
        throw std::runtime_error("Fichier impossible à ouvrir ou chemin d'accès incorrect.");
    }

    if (myfile.is_open()) {
        while (std::getline(myfile, ligne)) {
            if (!std::empty(ligne)) {
                // La ligne commence pas "v "
                if (ligne[0] == 'v' && ligne[1] == ' ') {
                    Vecteur vec{};

                    // On récupère la coordonée x
                    ligne = ligne.substr(2);
                    vec.x = std::stod(ligne.substr(0, ligne.find(" ")));

                    // On récupère la coordonée y
                    ligne = ligne.substr(ligne.find(" ") + 1);
                    vec.y = std::stod(ligne.substr(0, ligne.find(" ")));

                    // On récupère la coordonée z
                    ligne = ligne.substr(ligne.find(" ") + 1);
                    vec.z = std::stod(ligne.substr(0, ligne.find(" ")));

                    vecteursMesh.push_back(vec);
                } else if (ligne[0] == 'v' && ligne[1] == 't' && ligne[2] == ' ') {
                    Vecteur vec{};

                    // On récupère la coordonée x
                    ligne = ligne.substr(4);
                    vec.x = std::stod(ligne.substr(0, ligne.find(" ")));

                    // On récupère la coordonée y
                    ligne = ligne.substr(ligne.find(" ") + 1);
                    vec.y = std::stod(ligne.substr(0, ligne.find(" ")));

                    vecteursTextures.push_back(vec);
                } else if (ligne[0] == 'f' && ligne[1] == ' ') {
                    Triangle triangleMesh{};
                    Triangle triangleTexture{};
                    ligne = ligne.substr(2);

                    // On récupère le premier point
                    int a = std::stoi(ligne.substr(0, ligne.find("/"))) - 1;
                    triangleMesh.A = vecteursMesh[a];
                    ligne = ligne.substr(ligne.find("/") + 1);
                    int t1 = std::stoi(ligne.substr(0, ligne.find("/"))) - 1;
                    triangleTexture.A = vecteursTextures[t1];

                    // On récupère le 2è point
                    ligne = ligne.substr(ligne.find(" ") + 1);
                    int b = std::stoi(ligne.substr(0, ligne.find("/"))) - 1;
                    triangleMesh.B = vecteursMesh[b];
                    ligne = ligne.substr(ligne.find("/") + 1);
                    int t2 = std::stoi(ligne.substr(0, ligne.find("/"))) - 1;
                    triangleTexture.B = vecteursTextures[t2];

                    // On récupère le 3è point
                    ligne = ligne.substr(ligne.find(" ") + 1);
                    int c = std::stoi(ligne.substr(0, ligne.find("/"))) - 1;
                    triangleMesh.C = vecteursMesh[c];
                    ligne = ligne.substr(ligne.find("/") + 1);
                    int t3 = std::stoi(ligne.substr(0, ligne.find("/"))) - 1;
                    triangleTexture.C = vecteursTextures[t3];

                    this->faces.push_back(triangleMesh);
                    this->texturesCoord.push_back(triangleTexture);
                }
            }
        }
        myfile.close();
    }
}

int Model::nbFaces() {
    return std::size(this->faces);
}
