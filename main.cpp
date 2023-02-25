#include <iostream>

#include "geometrie.h"
#include "matrice.h"
#include "model.h"
#include "tgaimage.h"

using Mat = std::vector<std::vector<double>>;

Vecteur eye{1, 1, 3};
Vecteur center{0, 0, 0};
Vecteur lightDir{1, 1, 1};
Vecteur up{0, 1, 0};
Vecteur uniform_l{};

Model monModel{};
TGAImage texture{};
TGAImage normalMap{};
TGAImage specularMap{};

Mat modelView{};
Mat viewPort{};
Mat projection = Matrice::createMatrice(4, 4);

struct GouraudShader : public IShader {
    Mat varying_uv = Matrice::createMatrice(2, 3);
    Mat uniform_M = Matrice::createMatrice(4, 4);
    Mat uniform_MIT = Matrice::createMatrice(4, 4);

    virtual Mat vertex(int iface, char coord) override {
        Vecteur v2{};
        Vecteur t{};
        switch (coord) {
            case 'A':
                t = monModel.texturesCoord[iface].A;
                varying_uv[0][0] = t.x;
                varying_uv[1][0] = t.y;
                v2 = monModel.faces[iface].A;
                break;
            case 'B':
                t = monModel.texturesCoord[iface].B;
                varying_uv[0][1] = t.x;
                varying_uv[1][1] = t.y;
                v2 = monModel.faces[iface].B;
                break;
            case 'C':
                t = monModel.texturesCoord[iface].C;
                varying_uv[0][2] = t.x;
                varying_uv[1][2] = t.y;
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
        Vecteur uv{};
        uv.x = varying_uv[0][0] * bar.x + varying_uv[0][1] * bar.y + varying_uv[0][2] * bar.z;
        uv.y = varying_uv[1][0] * bar.x + varying_uv[1][1] * bar.y + varying_uv[1][2] * bar.z;

        Mat numit = Matrice::createMatrice(4, 1);
        TGAColor tmpnormal = normalMap.get(round(uv.x * normalMap.width()), normalMap.height() - 1 - (uv.y * normalMap.height()));
        Vecteur normal{};
        normal.x = (double)tmpnormal.bgra[2] / 255. * 2. - 1.;
        normal.y = (double)tmpnormal.bgra[1] / 255. * 2. - 1.;
        normal.z = (double)tmpnormal.bgra[0] / 255. * 2. - 1.;
        normal.normaliser();

        numit[0][0] = normal.x;
        numit[1][0] = normal.y;
        numit[2][0] = normal.z;
        numit[3][0] = 1;
        Mat mtemp1 = Matrice::mult(uniform_MIT, numit);
        Vecteur n{mtemp1[0][0], mtemp1[1][0], mtemp1[2][0]};
        n.normaliser();

        Mat lum = Matrice::createMatrice(4, 1);
        lum[0][0] = lightDir.x;
        lum[1][0] = lightDir.y;
        lum[2][0] = lightDir.z;
        lum[3][0] = 1;
        Mat mtemp2 = Matrice::mult(uniform_M, lum);
        Vecteur l{mtemp2[0][0], mtemp2[1][0], mtemp2[2][0]};
        l.normaliser();

        Vecteur r = n;
        double t = (n.x * l.x + n.y * l.y + n.z * l.z) * 2.;
        r.x = r.x * t - l.x;
        r.y = r.y * t - l.y;
        r.z = r.z * t - l.z;
        r.normaliser();

        double spec = pow(std::max(r.z, 0.), specularMap.get(uv.x * specularMap.width(), specularMap.height() - 1 - (uv.y * specularMap.height())).bgra[0]);
        double diff = std::max(0., n.x * l.x + n.y * l.y + n.z * l.z);

        color = texture.get(uv.x * texture.width(), texture.height() - 1 - (uv.y * texture.height()));

        color.bgra[0] = std::min(5 + color.bgra[0] * (diff + 0.5 * spec), 255.);
        color.bgra[1] = std::min(5 + color.bgra[1] * (diff + 0.5 * spec), 255.);
        color.bgra[2] = std::min(5 + color.bgra[2] * (diff + 0.5 * spec), 255.);
        return false;
    }
};

Mat lookat(Vecteur eye, Vecteur center, Vecteur up) {
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

Mat viewport(int x, int y, int w, int h) {
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
    texture.read_tga_file("../obj/african_head/african_head_diffuse.tga");
    normalMap.read_tga_file("../obj/african_head/african_head_nm.tga");
    specularMap.read_tga_file("../obj/african_head/african_head_spec.tga");

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

    Mat tmpl = Matrice::createMatrice(4, 1);
    tmpl[0][0] = lightDir.x;
    tmpl[1][0] = lightDir.y;
    tmpl[2][0] = lightDir.z;
    tmpl[3][0] = 1;
    tmpl = Matrice::mult(modelView, tmpl);
    uniform_l.x = tmpl[0][0];
    uniform_l.y = tmpl[1][0];
    uniform_l.z = tmpl[2][0];
    uniform_l.normaliser();

    lightDir.normaliser();

    // Matrice::printMat(projection);
    // Matrice::printMat(modelView);
    // Matrice::printMat(viewPort);
    // Matrice::printMat(Matrice::invert(viewPort));
    // Matrice::printMat(Matrice::mult(Matrice::mult(viewPort, projection), modelView));

    try {
        double zbuffer[WIDTH * HEIGHT];
        for (int i = 0; i < WIDTH * HEIGHT; i++) {
            zbuffer[i] = -std::numeric_limits<double>::infinity();
        }

        GouraudShader shader{};
        shader.uniform_M = Matrice::mult(projection, modelView);
        shader.uniform_MIT = Matrice::invert_transpose(Matrice::mult(projection, modelView));

        for (int i = 0; i < monModel.nbFaces(); i++) {
            Triangle const &t = monModel.faces[i];
            Vecteur screenCoords[3];
            Vecteur worldCoords[3];

            Vecteur v1 = t.A;
            screenCoords[0].set(shader.vertex(i, 'A'));

            Vecteur v2 = t.B;
            screenCoords[1].set(shader.vertex(i, 'B'));

            Vecteur v3 = t.C;
            screenCoords[2].set(shader.vertex(i, 'C'));

            Triangle const t2{screenCoords[0], screenCoords[1], screenCoords[2]};
            trianglePlein(t2, zbuffer, image, shader);
        }

        image.write_tga_file("outpout.tga");
    } catch (std::runtime_error const &exception) {
        std::cout << "Erreur : " << exception.what() << std::endl;
        return -1;
    }
    return 0;
}
