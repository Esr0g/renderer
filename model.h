#ifndef _H_PARSER
#define _H_PARSER

#include "geometrie.h"

struct Model {
    std::vector<Triangle> faces{};
    std::vector<Triangle> texturesCoord{};
    std::vector<Triangle> normales{};

   public:
    void parseObj(std::string const &path);
    int nbFaces();
};

#endif