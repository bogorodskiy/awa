#include "Plane.h"

Plane::Plane(int size) : Geometry::Geometry(),
                 m_vertices(nullptr),
                 m_indices(nullptr),
                 m_normals(nullptr)
{
    LOGI("PLANE CTOR");
    m_numVertices = 4;
    m_vertices = new GLfloat[3 * m_numVertices];

    m_numIndices = 6;
    m_indices = new GLushort[m_numIndices];
    m_indices[0] = 0;
    m_indices[1] = 1;
    m_indices[2] = 2;
    m_indices[3] = 2;
    m_indices[4] = 3;
    m_indices[5] = 0;

    m_normals = new GLfloat[3];
    m_normals[0] = 1.0f;
    m_normals[1] = 0.0f;
    m_normals[2] = 0.0f;

    auto halfSize = size / 2.0f;

    m_vertices[0] = 0;
    m_vertices[1] = halfSize;
    m_vertices[2] = halfSize;

    m_vertices[3] = 0;
    m_vertices[4] = -halfSize;
    m_vertices[5] = halfSize;

    m_vertices[6] = 0;
    m_vertices[7] = -halfSize;
    m_vertices[8] = -halfSize;

    m_vertices[9] = 0;
    m_vertices[10] = halfSize;
    m_vertices[11] = -halfSize;
}

Plane::~Plane() {
    Geometry::~Geometry();

    LOGI("PLANE DTOR");

    if (m_vertices != nullptr) {
        delete[] m_vertices;
    }
    if (m_indices != nullptr) {
        delete [] m_indices;
    }
    if (m_normals != nullptr) {
        delete [] m_normals;
    }
}

void Plane::initBuffers() {
    setVertices(m_vertices, m_numVertices, 3 * sizeof(GLfloat));
    setIndices(m_indices, m_numIndices * 3 * sizeof(GLushort));
    setNormals(m_normals, m_numVertices, 3 * sizeof(GLfloat));
}

GLenum Plane::getPrimitive() {
    return GL_TRIANGLES;
}