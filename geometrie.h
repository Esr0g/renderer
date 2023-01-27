#ifndef _H_GEOMETRIE
#define _H_GEOMETRIE

#include <cmath>
#include <string>
#include <vector>

#include "tgaimage.h"

struct Vecteur {
    double x{};
    double y{};
    double z{};

   public:
    void set(std::vector<std::vector<double>> const &mat);
    void normaliser();
    static Vecteur cross(Vecteur const &v1, Vecteur const &v2);
};

struct Triangle {
    Vecteur A{};
    Vecteur B{};
    Vecteur C{};
};

void ligne(Vecteur const &A, Vecteur const &B, TGAImage &image, TGAColor const &color);
void triangle(Triangle const &triangle, TGAImage &image, TGAColor const &color);
void trianglePlein(Triangle const &triangle, Triangle const &coordTexture, double zbuffer[], TGAImage &image, TGAImage const &texture, double intensity);

#endif