#ifndef _MATRICE_H
#define _MATRICE_H

#include <iostream>
#include <string>
#include <vector>

#include "geometrie.h"

class Matrice {
   public:
    static std::vector<std::vector<double>> createMatrice(int ligne, int colonne);
    static std::vector<std::vector<double>> createMatFromVec(Vecteur const &v);
    static std::vector<std::vector<double>> mult(std::vector<std::vector<double>> const &mat1, std::vector<std::vector<double>> const &mat2);
    static std::vector<std::vector<double>> mult(double x, std::vector<std::vector<double>> const &mat1);
    static std::vector<std::vector<double>> minus(std::vector<std::vector<double>> const &mat1, std::vector<std::vector<double>> const &mat2);
    static std::vector<std::vector<double>> plus(std::vector<std::vector<double>> const &mat1, std::vector<std::vector<double>> const &mat2);
    static void printMat(std::vector<std::vector<double>> const &mat1);
};

#endif