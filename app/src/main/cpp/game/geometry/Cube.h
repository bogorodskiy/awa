#ifndef IRON_BALLS_CUBE_H
#define IRON_BALLS_CUBE_H
#include "../../core/Geometry.h"
#include <vector>

class Cube : public Geometry {
public:
    Cube();
private:
    std::vector<GLfloat> m_vertices;
    std::vector<GLushort> m_indices;
    std::vector<GLfloat> m_normals;

    void initVertices();
    void initIndices();
    void initNormals();
};


#endif //IRON_BALLS_CUBE_H
