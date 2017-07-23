#include "Sphere.h"

Sphere::Sphere(float radius) : Geometry::Geometry(),
                   m_vertices(nullptr),
                   m_normals(nullptr),
                   m_indices(nullptr)
    {
    auto latitudeBands = 30;
    auto longitudeBands = 30;
    auto index = 0;
    m_numVertices = (latitudeBands + 1) * (longitudeBands + 1);
    m_vertices = new GLfloat[m_numVertices * 3];

    m_numIndices = (latitudeBands + 1) * (longitudeBands + 1) * 6;
    m_indices = new GLushort[m_numIndices];

    m_normals = new GLfloat[m_numVertices * 3];

    for (auto latNumber = 0; latNumber <= latitudeBands; latNumber++) {
        auto theta = latNumber * M_PI / (double)latitudeBands;
        auto sinTheta = glm::sin(theta);
        auto cosTheta = glm::cos(theta);

        for (auto longNumber = 0; longNumber <= longitudeBands; longNumber++) {
            auto phi = longNumber * 2 * M_PI / (double) longitudeBands;
            auto sinPhi = glm::sin(phi);
            auto cosPhi = glm::cos(phi);
            GLfloat x = (GLfloat)(cosPhi * sinTheta);
            GLfloat y = (GLfloat)(cosTheta);
            GLfloat z = (GLfloat)(sinPhi * sinTheta);

            m_vertices[index] = x * radius;
            m_normals[index] = x;
            index++;

            m_vertices[index] = y * radius;
            m_normals[index] = y;
            index++;

            m_vertices[index] = z * radius;
            m_normals[index] = z;
            index++;
        }
    }

    index = 0;
    for (auto latNumber = 0; latNumber <= latitudeBands; latNumber++) {
        for (auto longNumber = 0; longNumber <= longitudeBands; longNumber++) {
            auto first = (uint)(latNumber * (longitudeBands + 1)) + longNumber;
            auto second = first + longitudeBands + 1;
            m_indices[index] = (GLushort)first;
            index++;
            m_indices[index] = (GLushort)second;
            index++;
            m_indices[index] = (GLushort)(first + 1);
            index++;

            m_indices[index] = (GLushort)second;
            index++;
            m_indices[index] = (GLushort)(second + 1);
            index++;
            m_indices[index] = (GLushort)(first + 1);
            index++;
        }
    }
}

void Sphere::initBuffers() {
    setVertices(m_vertices, m_numVertices, 3 * sizeof(GLfloat));
    setIndices(m_indices, m_numIndices * 3 * sizeof(GLushort));
    setNormals(m_normals, m_numVertices, 3 * sizeof(GLfloat));
}

Sphere::~Sphere() {
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