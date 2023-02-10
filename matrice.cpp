#include "matrice.h"

std::vector<std::vector<double>> Matrice::createMatrice(int ligne, int colonne) {
    return std::vector<std::vector<double>>(ligne, std::vector<double>(colonne, 0));
}

std::vector<std::vector<double>> Matrice::mult(std::vector<std::vector<double>> const &mat1, std::vector<std::vector<double>> const &mat2) {
    if (mat1[0].size() != mat2.size()) {
        throw std::runtime_error("Attention : Le nombre de colones de la première matrice doit être égale au nombre de ligne de la deuxième.");
    }

    std::vector<std::vector<double>> res = Matrice::createMatrice(mat1.size(), mat2[0].size());
    for (int i1 = 0; i1 < res.size(); i1++) {
        for (int j1 = 0; j1 < res[i1].size(); j1++) {
            double tot = 0;
            for (int k = 0; k < mat1[0].size(); k++) {
                tot += mat1[i1][k] * mat2[k][j1];
            }
            res[i1][j1] = tot;
        }
    }

    return res;
}

std::vector<std::vector<double>> Matrice::mult(double x, std::vector<std::vector<double>> const &mat1) {
    std::vector<std::vector<double>> res = Matrice::createMatrice(mat1.size(), mat1[0].size());

    for (int i = 0; i < mat1.size(); i++) {
        for (int j = 0; j < mat1[i].size(); j++) {
            res[i][j] = x * mat1[i][j];
        }
    }

    return res;
}

void Matrice::printMat(std::vector<std::vector<double>> const &mat1) {
    std::string res{};
    for (int i = 0; i < mat1.size(); i++) {
        res += "[";
        for (int j = 0; j < mat1[i].size(); j++) {
            if (j + 1 == mat1[i].size()) {
                res += std::to_string(mat1[i][j]);
            } else {
                res += std::to_string(mat1[i][j]) + ",";
            }
        }

        res += "]\n";
    }

    std::cout << res << std::endl;
}

std::vector<std::vector<double>> Matrice::createMatFromVec(Vecteur const &v) {
    auto res = Matrice::createMatrice(3, 1);
    res[0][0] = v.x;
    res[1][0] = v.y;
    res[2][0] = v.z;

    return res;
}

std::vector<std::vector<double>> Matrice::minus(std::vector<std::vector<double>> const &mat1, std::vector<std::vector<double>> const &mat2) {
    if (mat1.size() != mat2.size() || mat1[0].size() != mat2[0].size()) {
        throw std::runtime_error("Pour additionner 2 matrice, il faut que celles-ci soient de même taille.");
    }

    auto res = Matrice::createMatrice(mat1.size(), mat1[0].size());

    for (int i = 0; i < res.size(); i++) {
        for (int j = 0; j < res[i].size(); j++) {
            res[i][j] = mat1[i][j] - mat2[i][j];
        }
    }

    return res;
}

std::vector<std::vector<double>> Matrice::plus(std::vector<std::vector<double>> const &mat1, std::vector<std::vector<double>> const &mat2) {
    if (mat1.size() != mat2.size() || mat1[0].size() != mat2[0].size()) {
        throw std::runtime_error("Pour soustraire 2 matrice, il faut que celles-ci soient de même taille.");
    }

    auto res = Matrice::createMatrice(mat1.size(), mat1[0].size());

    for (int i = 0; i < res.size(); i++) {
        for (int j = 0; j < res[i].size(); j++) {
            res[i][j] = mat1[i][j] + mat2[i][j];
        }
    }

    return res;
}