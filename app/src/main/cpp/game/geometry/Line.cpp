#include "Line.h"

Line::Line(glm::vec3 start, glm::vec3 end) : Geometry::Geometry(),
               m_vertices(nullptr) {
    m_vertices = new GLfloat[2 * 3];
    m_vertices[0] = start.x;
    m_vertices[1] = start.y;
    m_vertices[2] = start.z;

    m_vertices[3] = end.x;
    m_vertices[4] = end.y;
    m_vertices[5] = end.z;
}

Line::~Line() {
    delete[] m_vertices;
}

GLenum Line::getPrimitive() {
    return GL_LINES;
}

void Line::initBuffers() {
    setVertices(m_vertices, 2, 3 * sizeof(GLfloat));
}