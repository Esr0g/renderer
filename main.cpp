#include <iostream>

#include "geometrie.h"
#include "matrice.h"
#include "model.h"
#include "tgaimage.h"

Vecteur eye{1, 1, 3};
Vecteur center{0, 0, 0};
Vecteur lightDir{0, 0, -1};
Vecteur up{0, 1, 0};

Model monModel{};

std::vector<std::vector<double>> modelView{};
std::vector<std::vector<double>> viewPort{};
std::vector<std::vector<double>> projection = Matrice::createMatrice(4, 4);

struct GouraudShader : public IShader {
    Vecteur varying_intensity{};
    virtual std::vector<std::vector<double>> vertex(int iface, char coord) override {
        Vecteur v2{};
        Vecteur v{};
        switch (coord) {
            case 'A':
                v = monModel.normales[iface].A;
                varying_intensity.x = std::max(0., v.x * -lightDir.x + v.y * -lightDir.y + v.z * -lightDir.z);
                v2 = monModel.faces[iface].A;
                break;
            case 'B':
                v = monModel.normales[iface].B;
                varying_intensity.y = std::max(0., v.x * -lightDir.x + v.y * -lightDir.y + v.z * -lightDir.z);
                v2 = monModel.faces[iface].B;
                break;
            case 'C':
                v = monModel.normales[iface].C;
                varying_intensity.z = std::max(0., v.x * -lightDir.x + v.y * -lightDir.y + v.z * -lightDir.z);
                v2 = monModel.faces[iface].C;
                break;
        }

        auto glVertex = Matrice::createMatrice(4, 1);
        glVertex[0][0] = v2.x;
        glVertex[1][0] = v2.y;
        glVertex[2][0] = v2.z;
        glVertex[3][0] = 1;

        return Matrice::mult(viewPort, Matrice::mult(projection, Matrice::mult(modelView, glVertex)));
    }

    virtual bool fragment(Vecteur pt, Vecteur bar, TGAColor &color) override {
        float intensity = varying_intensity.x * bar.x + varying_intensity.y * bar.y + varying_intensity.z * bar.z;
        //      std::cerr << bar.x << " " << bar.y << " " << bar.z << " " << std::endl;

        color = TGAColor(255 * intensity, 255 * intensity, 255 * intensity);
        return false;
    }
};

std::vector<std::vector<double>>
lookat(Vecteur eye, Vecteur center, Vecteur up) {
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

    auto res = Matrice::createMatrice(4, 4);

    for (int i = 0; i < 4; i++) {
        res[i][i] = 1;
    }

    res[0][0] = x.x;
    res[0][1] = x.y;
    res[0][2] = x.z;
    res[1][0] = y.x;
    res[1][1] = y.y;
    res[1][2] = y.z;
    res[2][0] = z.x;
    res[2][1] = z.y;
    res[2][2] = z.z;
    res[0][3] = -center.x;
    res[1][3] = -center.y;
    res[2][3] = -center.z;

    return res;
}

std::vector<std::vector<double>> viewport(int x, int y, int w, int h) {
    auto m = Matrice::createMatrice(4, 4);
    m[3][3] = 1;
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
    monModel.parseObj("../obj/african_head/african_head.obj");

    // On récupère les textures diffuses pour les afficher sur le model
    TGAImage texture{};
    texture.read_tga_file("../obj/african_head/african_head_diffuse.tga");

    modelView = lookat(eye, center, up);

    viewPort = viewport(WIDTH / 8, HEIGHT / 8, WIDTH * 3 / 4, HEIGHT * 3 / 4);
    auto tmpEye = Matrice::createMatFromVec(eye);
    auto tmpCenter = Matrice::createMatFromVec(center);
    auto m1 = Matrice::minus(tmpEye, tmpCenter);
    Vecteur tmp{};
    tmp.set(m1);

    for (int i = 0; i < 4; i++) {
        projection[i][i] = 1;
    }
    projection[3][2] = -1. / tmp.norm();

    // Matrice::printMat(projection);
    // Matrice::printMat(modelView);
    // Matrice::printMat(viewPort);
    // Matrice::printMat(Matrice::mult(Matrice::mult(viewPort, projection), modelView));

    try {
        double zbuffer[WIDTH * HEIGHT];
        for (int i = 0; i < WIDTH * HEIGHT; i++) {
            zbuffer[i] = -std::numeric_limits<double>::infinity();
        }

        GouraudShader shader{};
        for (int i = 0; i < monModel.nbFaces(); i++) {
            Triangle const &t = monModel.faces[i];
            Vecteur screenCoords[3];
            Vecteur worldCoords[3];

            Vecteur v1 = t.A;
            worldCoords[0] = v1;
            auto tmpv1 = Matrice::createMatrice(4, 1);
            tmpv1[0][0] = v1.x;
            tmpv1[1][0] = v1.y;
            tmpv1[2][0] = v1.z;
            tmpv1[3][0] = 1;
            screenCoords[0].set(shader.vertex(i, 'A'));

            Vecteur v2 = t.B;
            worldCoords[1] = v2;
            auto tmpv2 = Matrice::createMatrice(4, 1);
            tmpv2[0][0] = v2.x;
            tmpv2[1][0] = v2.y;
            tmpv2[2][0] = v2.z;
            tmpv2[3][0] = 1;
            screenCoords[1].set(shader.vertex(i, 'B'));

            Vecteur v3 = t.C;
            worldCoords[2] = v3;
            auto tmpv3 = Matrice::createMatrice(4, 1);
            tmpv3[0][0] = v3.x;
            tmpv3[1][0] = v3.y;
            tmpv3[2][0] = v3.z;
            tmpv3[3][0] = 1;
            screenCoords[2].set(shader.vertex(i, 'C'));

            // Vecteur v4{
            //     worldCoords[2].x - worldCoords[0].x,
            //     worldCoords[2].y - worldCoords[0].y,
            //     worldCoords[2].z - worldCoords[0].z};

            // Vecteur v5{
            //     worldCoords[1].x - worldCoords[0].x,
            //     worldCoords[1].y - worldCoords[0].y,
            //     worldCoords[1].z - worldCoords[0].z};

            // // v4 x v5
            // Vecteur n{
            //     v4.y * v5.z - v4.z * v5.y,
            //     v4.z * v5.x - v4.x * v5.z,
            //     v4.x * v5.y - v4.y * v5.x};

            // n.normaliser();

            // double intensity = n.x * lightDir.x + n.y * lightDir.y + n.z * lightDir.z;
            // if (intensity > 0) {
            Triangle const t2{screenCoords[0], screenCoords[1], screenCoords[2]};
            trianglePlein(t2, monModel.texturesCoord[i], zbuffer, image, texture, shader);
            //}
        }

        image.write_tga_file("outpout.tga");
    } catch (std::runtime_error const &exception) {
        std::cout << "Erreur : " << exception.what() << std::endl;
        return -1;
    }
    return 0;
}
