#include "Cube.h"

Cube::Cube() : Geometry::Geometry()
{
    initVertices();
    initIndices();
    initNormals();

    setVertices(&m_vertices[0], 72, 3 * sizeof(GLfloat));
    setIndices(&m_indices[0], 36 * sizeof(GLushort));
    setNormals(&m_normals[0], 72, 3 * sizeof(GLfloat));
}

void Cube::initVertices() {
    m_vertices = {
            // front
            -1.0f, -1.0f,  1.0f,
             1.0f, -1.0f,  1.0f,
             1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            // top
            -1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
             1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            // back
             1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
             1.0f,  1.0f, -1.0f,
            // bottom
            -1.0f, -1.0f, -1.0f,
             1.0f, -1.0f, -1.0f,
             1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,
            // left
            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,
            // right
            1.0f, -1.0f,  1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f,  1.0f,  1.0f,
    };
}

void Cube::initIndices() {
    m_indices = {
            // front
            0,  1,  2,
            2,  3,  0,
            // top
            4,  5,  6,
            6,  7,  4,
            // back
            8,  9, 10,
            10, 11,  8,
            // bottom
            12, 13, 14,
            14, 15, 12,
            // left
            16, 17, 18,
            18, 19, 16,
            // right
            20, 21, 22,
            22, 23, 20,
    };
}

void Cube::initNormals(){
    m_normals = {
            // front
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            // top
            -1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            // back
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            // bottom
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,
            // left
            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,
            // right
            1.0f, -1.0f,  1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f,  1.0f,  1.0f,
    };
}
