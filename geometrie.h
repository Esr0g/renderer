#ifndef _H_GEOMETRIE
#define _H_GEOMETRIE

#include <cmath>
#include <iostream>
#include <string>
#include <vector>

#include "tgaimage.h"

struct Vecteur {
    double x{};
    double y{};
    double z{};
    double w{};

   public:
    void set(std::vector<std::vector<double>> const &mat);
    void normaliser();
    double norm();
    friend std::ostream &operator<<(std::ostream &os, const Vecteur &v);
    static Vecteur cross(Vecteur const &v1, Vecteur const &v2);
};

struct IShader {
    // virtual ~IShader();
    virtual std::vector<std::vector<double>> vertex(int iface, char coord) = 0;
    virtual bool fragment(Vecteur pt, Vecteur bar, TGAColor &color) = 0;
};

struct Triangle {
    Vecteur A{};
    Vecteur B{};
    Vecteur C{};
};

void ligne(Vecteur const &A, Vecteur const &B, TGAImage &image, TGAColor const &color);
void triangle(Triangle const &triangle, TGAImage &image, TGAColor const &color);
void trianglePlein(Triangle const &triangle, double zbuffer[], TGAImage &image, IShader &shader);

#endif