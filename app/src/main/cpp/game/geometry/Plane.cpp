#include "Plane.h"

Plane::Plane() : Geometry::Geometry(),
               m_vertices(nullptr) {
}

Plane::~Plane() {
    delete[] m_vertices;
}

void Plane::setSize(int value) {
    auto numLinesPerOne = 4;
    auto numVertices = 4 + value * numLinesPerOne * 2; // 4 for bounds
    m_vertices = new GLfloat[3 * numVertices];

    auto offset = value / 2.0f;

    m_vertices[0] = 0;
    m_vertices[1] = -offset;
    m_vertices[2] = -offset;

    m_vertices[3] = 0;
    m_vertices[4] = value - offset;
    m_vertices[5] = -offset;

    m_vertices[6] = 0;
    m_vertices[7] = value - offset;
    m_vertices[8] = value - offset;

    m_vertices[9] = 0;
    m_vertices[10] = -offset;
    m_vertices[11] = value - offset;

    auto step = 1.0f / numLinesPerOne;
    auto startIndex = 0;
    for (auto i = 0; i < numLinesPerOne * value; ++i) {
        startIndex = 12 + i * 2 * 3; //index * num vertices per line * vertex size

        m_vertices[startIndex] = 0;
        m_vertices[startIndex + 1] = i * step - offset;
        m_vertices[startIndex + 2] = -offset;

        m_vertices[startIndex + 3] = 0;
        m_vertices[startIndex + 4] = m_vertices[startIndex + 1] + step;
        m_vertices[startIndex + 5] = value - offset;
    }

    setVertices(&m_vertices[0], numVertices, 3 * sizeof(GLfloat));
}

GLenum Plane::getPrimitive() {
    return GL_LINE_STRIP;
}