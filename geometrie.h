#ifndef _H_GEOMETRIE
#define _H_GEOMETRIE

#include <cmath>
#include <string>

#include "tgaimage.h"

struct Vecteur {
    double x{};
    double y{};
    double z{};

   public:
    void normaliser();
};

struct Triangle {
    Vecteur A{};
    Vecteur B{};
    Vecteur C{};
};

void ligne(Vecteur const &A, Vecteur const &B, TGAImage &image, TGAColor const &color);
void triangle(Triangle const &triangle, TGAImage &image, TGAColor const &color);
void trianglePlein(Triangle const &triangle, double zbuffer[], TGAImage &image, TGAColor const &color);

#endif