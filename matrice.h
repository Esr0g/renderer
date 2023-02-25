#ifndef _MATRICE_H
#define _MATRICE_H

#include <iostream>
#include <string>
#include <vector>

#include "geometrie.h"

using Mat = std::vector<std::vector<double>>;

class Matrice {
   public:
    static Mat createMatrice(int ligne, int colonne);
    static Mat createMatFromVec(Vecteur const &v);
    static Mat mult(Mat const &mat1, Mat const &mat2);
    static Mat mult(double x, Mat const &mat1);
    static Mat minus(Mat const &mat1, Mat const &mat2);
    static Mat plus(Mat const &mat1, Mat const &mat2);
    static Mat get_minor(Mat const &m, const int row, const int col);
    static Mat adjugate(Mat const &m);
    static Mat invert_transpose(Mat const &m);
    static Mat transpose(Mat const &m);
    static Mat invert(Mat const &m);
    static double det(Mat const &mat);
    static void printMat(Mat const &mat1);
    static double cofacteur(Mat const &m, const int row, const int col);
};

#endif