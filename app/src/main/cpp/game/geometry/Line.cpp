#include "Line.h"

Line::Line() : Geometry::Geometry(),
               m_vertices(nullptr) {
    m_vertices = new GLfloat[2 * 3];
}

void Line::setPoints(glm::vec3 start, glm::vec3 end) {
    m_vertices[0] = start.x;
    m_vertices[1] = start.y;
    m_vertices[2] = start.z;

    m_vertices[3] = end.x;
    m_vertices[4] = end.y;
    m_vertices[5] = end.z;

    setVertices(&m_vertices[0], 6, 3 * sizeof(GLfloat));

    m_indices = {0, 1};
    setIndices(&m_indices[0], 36 * sizeof(GLushort));
}

GLenum Line::getPrimitive() {
    return GL_LINES;
}