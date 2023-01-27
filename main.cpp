#include "geometrie.h"
#include "matrice.h"
#include "model.h"
#include "tgaimage.h"

void lookat(Vecteur eye, Vecteur center, Vecteur up) {
    auto matEye = Matrice::createMatFromVec(eye);
    auto matCenter = Matrice::createMatFromVec(center);

    auto tempz = Matrice::minus(matEye, matCenter);
    Vecteur z{};
    z.set(tempz);
    z.normaliser();

    Vecteur x = Vecteur::cross(up, z);
    x.normaliser();

    Vecteur y = Vecteur::cross(z, x);
    y.normaliser();

    auto minv = Matrice::createMatrice(4, 4);
    auto tr = Matrice::createMatrice(4, 4);

    for (int i = 0; i < 4; i++) {
        minv[i][i] = 1;
        tr[i][i] = 1;
    }

    minv[0][1] = x.x;
    minv[0][2] = x.y;
    minv[0][3] = x.z;
    minv[1][0] = y.x;
    minv[1][1] = y.y;
    minv[1][2] = y.z;
    minv[2][0] = z.x;
    minv[2][1] = z.y;
    minv[2][2] = z.z;

    tr[0][3] = -eye.x;
    tr[1][3] = -eye.y;
    tr[2][3] = -eye.z;

    auto ModelView = Matrice::mult(minv, tr);
}

std::vector<std::vector<double>> viewport(int x, int y, int w, int h) {
    auto m = Matrice::createMatrice(4, 4);
    m[0][3] = x + w / 2.;
    m[1][3] = y + h / 2.;
    m[2][3] = DEPTH / 2.;

    m[0][0] = w / 2.;
    m[1][1] = h / 2.;
    m[2][2] = DEPTH / 2.;

    return m;
}

int main() {
    TGAImage image(WIDTH, HEIGHT, TGAImage::RGB);

    // On récupère les textures diffuses pour les afficher sur le model
    TGAImage texture{};
    texture.read_tga_file("../obj/african_head/african_head_diffuse.tga");

    try {
        Model monModel{};
        monModel.parseObj("../obj/african_head/african_head.obj");

        double zbuffer[WIDTH * HEIGHT];
        for (int i = 0; i < WIDTH * HEIGHT; i++) {
            zbuffer[i] = -std::numeric_limits<double>::infinity();
        }

        Vecteur lightDir{0, 0, -1};

        for (int i = 0; i < monModel.nbFaces(); i++) {
            Triangle const &t = monModel.faces[i];
            Vecteur screenCoords[3];
            Vecteur worldCoords[3];

            Vecteur v1 = t.A;
            worldCoords[0] = v1;
            double c = 5.;
            v1.x = v1.x / (1 - (v1.z / c));
            v1.y = v1.y / (1 - (v1.z / c));
            screenCoords[0] = {round((v1.x + 1.) * WIDTH / 2.), round((v1.y + 1.) * HEIGHT / 2.), v1.z};

            Vecteur v2 = t.B;
            worldCoords[1] = v2;
            v2.x = v2.x / (1 - (v2.z / c));
            v2.y = v2.y / (1 - (v2.z / c));
            screenCoords[1] = {round((v2.x + 1.) * WIDTH / 2.), round((v2.y + 1.) * HEIGHT / 2.), v2.z};

            Vecteur v3 = t.C;
            worldCoords[2] = v3;
            v3.x = v3.x / (1 - (v3.z / c));
            v3.y = v3.y / (1 - (v3.z / c));
            screenCoords[2] = {round((v3.x + 1.) * WIDTH / 2.), round((v3.y + 1.) * HEIGHT / 2.), v3.z};

            Vecteur v4{
                worldCoords[2].x - worldCoords[0].x,
                worldCoords[2].y - worldCoords[0].y,
                worldCoords[2].z - worldCoords[0].z};

            Vecteur v5{
                worldCoords[1].x - worldCoords[0].x,
                worldCoords[1].y - worldCoords[0].y,
                worldCoords[1].z - worldCoords[0].z};

            // v4 x v5
            Vecteur n{
                v4.y * v5.z - v4.z * v5.y,
                v4.z * v5.x - v4.x * v5.z,
                v4.x * v5.y - v4.y * v5.x};

            n.normaliser();

            double intensity = n.x * lightDir.x + n.y * lightDir.y + n.z * lightDir.z;
            if (intensity > 0) {
                Triangle const t2{screenCoords[0], screenCoords[1], screenCoords[2]};
                trianglePlein(t2, monModel.texturesCoord[i], zbuffer, image, texture, intensity);
            }
        }

        image.write_tga_file("outpout.tga");
    } catch (std::runtime_error const &exception) {
        std::cout << "Erreur : " << exception.what() << std::endl;
        return -1;
    }
    return 0;
}
