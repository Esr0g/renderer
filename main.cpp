#include "geometrie.h"
#include "model.h"
#include "tgaimage.h"

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
            screenCoords[0] = {round((v1.x + 1.) * WIDTH / 2.), round((v1.y + 1.) * HEIGHT / 2.)};
            worldCoords[0] = v1;

            Vecteur v2 = t.B;
            screenCoords[1] = {round((v2.x + 1.) * WIDTH / 2.), round((v2.y + 1.) * HEIGHT / 2.)};
            worldCoords[1] = v2;

            Vecteur v3 = t.C;
            screenCoords[2] = {round((v3.x + 1.) * WIDTH / 2.), round((v3.y + 1.) * HEIGHT / 2.)};
            worldCoords[2] = v3;

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
                trianglePlein(t2, zbuffer, image, TGAColor(intensity * 255, intensity * 255, intensity * 255, 255));
            }
        }

        image.write_tga_file("outpout.tga");
    } catch (std::runtime_error const &exception) {
        std::cout << "Erreur : " << exception.what() << std::endl;
        return -1;
    }
    return 0;
}
