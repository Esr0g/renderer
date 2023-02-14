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

double Matrice::det(std::vector<std::vector<double>> const &mat) {
    if (mat.size() == 4) {
        // ∣ a b c d | | 00 01 02 03 |
        // | e f g h | | 10 11 12 13 |
        // | i j k l | | 20 21 22 23 |
        // | m n o p ∣ | 30 31 32 33 |

        // =afkp−aflo−agjp+agln+ahjo−ahkn−bekp+belo+bgip−bglm−bhio+bhkm+cejp−celn−cfip+cflm+chin−chjm−dejo+dekn+dfio−dfkm−dgin+dgjm
        return mat[0][0] * mat[1][1] * mat[2][2] * mat[3][3]     // +afkp
               - mat[0][0] * mat[1][1] * mat[2][3] * mat[3][2]   // -aflo
               - mat[0][0] * mat[1][2] * mat[2][1] * mat[3][3]   // -agjp
               + mat[0][0] * mat[1][2] * mat[2][3] * mat[3][1]   // +agln
               + mat[0][0] * mat[1][3] * mat[2][1] * mat[3][2]   //+ahjo
               - mat[0][0] * mat[1][3] * mat[2][2] * mat[3][1]   // −ahkn
               - mat[0][1] * mat[1][0] * mat[2][2] * mat[3][3]   // −bekp
               + mat[0][1] * mat[1][0] * mat[2][3] * mat[3][2]   // +belo
               + mat[0][1] * mat[1][2] * mat[2][0] * mat[3][3]   // +bgip
               - mat[0][1] * mat[1][2] * mat[2][3] * mat[3][0]   // −bglm
               - mat[0][1] * mat[1][3] * mat[2][0] * mat[3][2]   // −bhio
               + mat[0][1] * mat[1][3] * mat[2][2] * mat[3][0]   // +bhkm
               + mat[0][2] * mat[1][0] * mat[2][1] * mat[3][3]   // +cejp
               - mat[0][2] * mat[1][0] * mat[2][3] * mat[3][2]   // −celn
               - mat[0][2] * mat[1][1] * mat[2][0] * mat[3][3]   // −cfip
               + mat[0][2] * mat[1][1] * mat[2][2] * mat[3][0]   // +cflm
               + mat[0][2] * mat[1][3] * mat[2][0] * mat[3][1]   // +chin
               - mat[0][2] * mat[1][3] * mat[2][1] * mat[3][0]   // −chjm
               - mat[0][3] * mat[1][0] * mat[2][1] * mat[3][2]   // −dejo
               + mat[0][3] * mat[1][0] * mat[2][2] * mat[3][1]   // +dekn
               + mat[0][3] * mat[1][1] * mat[2][0] * mat[3][2]   // +dfio
               - mat[0][3] * mat[1][1] * mat[2][2] * mat[3][0]   // −dfkm
               - mat[0][3] * mat[1][2] * mat[2][0] * mat[3][1]   // −dgin
               + mat[0][3] * mat[1][2] * mat[2][1] * mat[3][0];  // +dgjm
    } else if (mat.size() == 3) {
        // | a b c | | 00 01 02 |
        // | d e f | | 10 11 12 |
        // | g h i | | 20 21 22 |
        // det = aei−afh+bfg−bdi+cdh−ceg
        return mat[0][0] * mat[1][1] * mat[2][2]     // +aei
               - mat[0][0] * mat[1][2] * mat[2][1]   // -afh
               + mat[0][1] * mat[1][2] * mat[2][0]   // +bfg
               - mat[0][1] * mat[1][0] * mat[2][2]   // -bdi
               + mat[0][2] * mat[1][0] * mat[2][1]   // +cdh
               - mat[0][2] * mat[1][1] * mat[2][0];  // -ceg
    } else if (mat.size() == 2) {
        return mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0];
    }

    return 0;
}

std::vector<std::vector<double>> Matrice::get_minor(std::vector<std::vector<double>> const &m, const int row, const int col) {
    auto ret = Matrice::createMatrice(m.size() - 1, m[0].size() - 1);
    for (int i = m.size() - 1; i--;) {
        for (int j = m[0].size() - 1; j--; ret[i][j] = m[i < row ? i : i + 1][j < col ? j : j + 1])
            ;
    }

    return ret;
}

double Matrice::cofacteur(std::vector<std::vector<double>> const &m, const int row, const int col) {
    return ((row + col) % 2 ? -1 : 1) * Matrice::det(Matrice::get_minor(m, row, col));
}

std::vector<std::vector<double>> Matrice::adjugate(std::vector<std::vector<double>> const &m) {
    auto ret = Matrice::createMatrice(m.size(), m[0].size());

    for (int i = m.size(); i--;) {
        for (int j = m[0].size(); j--; ret[i][j] = cofacteur(m, i, j))
            ;
    }

    return ret;
}

std::vector<std::vector<double>> Matrice::invert_transpose(std::vector<std::vector<double>> const &m) {
    auto ret = Matrice::adjugate(m);

    double res{0};
    for (int k = 0; k < m[0].size(); k++) {
        res += ret[0][k] * m[0][k];
    }

    for (int i = 0; i < ret.size(); i++) {
        for (int j = 0; j < ret[0].size(); j++) {
            ret[i][j] = ret[i][j] / res;
        }
    }
    return ret;
}

std::vector<std::vector<double>> Matrice::transpose(std::vector<std::vector<double>> const &m) {
    auto ret = Matrice::createMatrice(m[0].size(), m.size());

    for (int i = 0; i < ret.size(); i++) {
        for (int j = 0; j < ret[0].size(); j++) {
            ret[i][j] = m[j][i];
        }
    }

    return ret;
}

std::vector<std::vector<double>> Matrice::invert(std::vector<std::vector<double>> const &m) {
    return Matrice::transpose(Matrice::invert_transpose(m));
}