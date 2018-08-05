#include "Plane.h"

Plane::Plane(float width, float height) : Geometry::Geometry(),
                 m_vertices(nullptr),
                 m_indices(nullptr),
                 m_normals(nullptr)
{
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

    auto halfWidth = width / 2.0f;
    auto halfHeight = height / 2.0f;

    m_vertices[0] = 0;
    m_vertices[1] = halfHeight;
    m_vertices[2] = halfWidth;

    m_vertices[3] = 0;
    m_vertices[4] = -halfHeight;
    m_vertices[5] = halfWidth;

    m_vertices[6] = 0;
    m_vertices[7] = -halfHeight;
    m_vertices[8] = -halfWidth;

    m_vertices[9] = 0;
    m_vertices[10] = halfHeight;
    m_vertices[11] = -halfWidth;

    m_numNormals = m_numVertices;
    m_normals = new GLfloat[3 * m_numNormals];
    m_normals[0] = 1.0f;
    m_normals[1] = halfHeight;
    m_normals[2] = halfWidth;

    m_normals[3] = 1.0f;
    m_normals[4] = -halfHeight;
    m_normals[5] = halfWidth;

    m_normals[6] = 1.0f;
    m_normals[7] = -halfHeight;
    m_normals[8] = -halfWidth;

    m_normals[9] = 1.0f;
    m_normals[10] = halfHeight;
    m_normals[11] = -halfWidth;

    m_specularPower = 1;
    m_specularIntensity = 1.0f;
}

Plane::~Plane() {
    Geometry::~Geometry();
    delete [] m_vertices;
    delete [] m_indices;
    delete [] m_normals;
}

void Plane::initBuffers() {
    setVertices(m_vertices, m_numVertices, 3 * sizeof(GLfloat));
    setIndices(m_indices, m_numIndices * sizeof(GLushort));
    setNormals(m_normals, m_numNormals, 3 * sizeof(GLfloat));
}

GLenum Plane::getPrimitive() {
    return GL_TRIANGLES;
}