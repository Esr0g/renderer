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

void trianglePlein(Triangle const &triangle, double zbuffer[], TGAImage &image, TGAColor const &color) {
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
                // Utilisation du zbuffer
                double z = alpha * triangle.A.z + beta * triangle.B.z + gamma * triangle.C.z;

                if (zbuffer[int(x) + int(y) * WIDTH] < z) {
                    zbuffer[int(x) + int(y) * WIDTH] = z;
                    image.set(x, y, color);
                }
            }

            x += 1;
        }
        y += 1;
    }
}

void Vecteur::normaliser() {
    double norme = sqrt((this->x * this->x) + (this->y * this->y) + (this->z * this->z));
    this->x = this->x / norme;
    this->y = this->y / norme;
    this->z = this->z / norme;
}

// Vecteur bboxmin{std::numeric_limits<double>::infinity(), std::numeric_limits<double>::infinity()};
// Vecteur bboxmax{-std::numeric_limits<double>::infinity(), -std::numeric_limits<double>::infinity()};
// Vecteur clamp{WIDTH - 1, HEIGHT - 1};

// bboxmin.x = std::max(0., std::min(bboxmin.x, MIN(triangle.A.x, triangle.B.x, triangle.C.x)));
// bboxmin.y = std::max(0., std::min(bboxmin.x, MIN(triangle.A.y, triangle.B.y, triangle.C.y)));

// bboxmax.x = std::min(clamp.x, std::max(bboxmax.x, MAX(triangle.A.x, triangle.B.x, triangle.C.x)));
// bboxmax.y = std::min(clamp.y, std::max(bboxmax.y, MAX(triangle.A.y, triangle.B.y, triangle.C.y)));