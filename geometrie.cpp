#include "geometrie.h"

#define MAX(x, y, z) std::max(x, std::max(y, z))
#define MIN(x, y, z) std::min(x, std::min(y, z))

void ligne(Vecteur const &A, Vecteur const &B, TGAImage &image, TGAColor const &color) {
    int dy = B.y - A.y;
    int dx = B.x - A.x;
    int absdx = std::abs(dx);
    int absdy = std::abs(dy);
    int x = A.x;
    int y = A.y;
    int sensY = 1;
    int sensX = 1;

    if (dx < 0) {
        sensX = -1;
    }

    if (dy < 0) {
        sensY = -1;
    }

    if (dy == 0) {
        while (x != B.x + sensX) {
            image.set(x, y, color);
            x += sensX;
        }
    } else if (dx == 0) {
        while (y != B.y + sensY) {
            image.set(x, y, color);
            y += sensY;
        }
    } else {
        if (absdx >= absdy) {
            int cumul = absdx;
            while (x != B.x + sensX) {
                image.set(x, y, color);
                cumul += (2 * absdy);

                if (cumul >= 2 * absdx) {
                    y += sensY;
                    cumul -= (2 * absdx);
                }
                x += sensX;
            }
        } else {
            int cumul = absdy;
            while (y != B.y + sensY) {
                image.set(x, y, color);
                cumul += (2 * absdx);

                if (cumul >= 2 * absdy) {
                    x += sensX;
                    cumul -= (2 * absdy);
                }
                y += sensY;
            }
        }
    }
}

void triangle(Triangle const &triangle, TGAImage &image, TGAColor const &color) {
    ligne(triangle.A, triangle.B, image, color);
    ligne(triangle.B, triangle.C, image, color);
    ligne(triangle.C, triangle.A, image, color);
}

void trianglePlein(Triangle const &triangle, double zbuffer[], TGAImage &image, IShader &shader) {
    // On parcours le rectangle englobant et on test si chaque pixel appartient au triangle

    // On récupềre les coordonées minX minY et maxX maxY du triangle
    Vecteur min{};
    Vecteur max{};

    min.x = MIN(triangle.A.x, triangle.B.x, triangle.C.x);
    min.y = MIN(triangle.A.y, triangle.B.y, triangle.C.y);

    max.x = MAX(triangle.A.x, triangle.B.x, triangle.C.x);
    max.y = MAX(triangle.A.y, triangle.B.y, triangle.C.y);

    // On parcours tous le rectangle englobant
    double x{min.x};
    double y{min.y};

    double ABC = ((triangle.A.x * (triangle.B.y - triangle.C.y) + triangle.B.x * (triangle.C.y - triangle.A.y) + triangle.C.x * (triangle.A.y - triangle.B.y)));

    while (y < max.y + 1) {
        x = min.x;
        while (x < max.x + 1) {
            // ABP
            double alpha = ((triangle.A.x * (triangle.B.y - y) + triangle.B.x * (y - triangle.A.y) + x * (triangle.A.y - triangle.B.y)));
            // BCP
            double beta = ((triangle.B.x * (triangle.C.y - y) + triangle.C.x * (y - triangle.B.y) + x * (triangle.B.y - triangle.C.y)));
            // CAP
            double gamma = ((triangle.C.x * (triangle.A.y - y) + triangle.A.x * (y - triangle.C.y) + x * (triangle.C.y - triangle.A.y)));

            if (alpha >= -0.01 && beta >= -0.01 && gamma >= -0.01) {
                alpha = alpha / ABC;
                beta = beta / ABC;
                gamma = gamma / ABC;
                // Utilisation du zbuffer
                double z = beta * triangle.A.z + gamma * triangle.B.z + alpha * triangle.C.z;

                if (x < WIDTH && y < HEIGHT) {
                    if (zbuffer[int(x) + int(y) * WIDTH] < z) {
                        zbuffer[int(x) + int(y) * WIDTH] = z;

                        TGAColor color;
                        bool discard = shader.fragment(Vecteur{x, y, z}, Vecteur{beta, gamma, alpha}, color);
                        if (!discard) {
                            image.set(x, y, color);
                        }
                    }
                }
            }

            x += 1;
        }
        y += 1;
    }
}

void Vecteur::normaliser() {
    double norme = this->norm();
    this->x = this->x / norme;
    this->y = this->y / norme;
    this->z = this->z / norme;
}

void Vecteur::set(std::vector<std::vector<double>> const &mat) {
    if (mat.size() == 3) {
        this->x = mat[0][0];
        this->y = mat[1][0];
        this->z = mat[2][0];
    } else if (mat.size() == 4) {
        this->x = round(mat[0][0] / mat[3][0]);
        this->y = round(mat[1][0] / mat[3][0]);
        this->z = round(mat[2][0] / mat[3][0]);
    }
}

Vecteur Vecteur::cross(Vecteur const &v1, Vecteur const &v2) {
    Vecteur v{};

    v.x = v1.y * v2.z - v1.z * v2.y;
    v.y = v1.z * v2.x - v1.x * v2.z;
    v.z = v1.x * v2.y - v1.y * v2.x;

    return v;
}

double Vecteur::norm() {
    return sqrt((this->x * this->x) + (this->y * this->y) + (this->z * this->z));
}

std::ostream &operator<<(std::ostream &os, const Vecteur &v) {
    return os << "{" << v.x << "," << v.y << "," << v.z << "}";
}