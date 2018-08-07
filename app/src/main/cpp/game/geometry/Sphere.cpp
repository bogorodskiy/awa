#include <cmath>
#include "Sphere.h"

Sphere::Sphere(float radius) : Geometry::Geometry()
{
    m_type = Geometry::Type::SPHERE;

    auto latitudeBands = 31;
    auto longitudeBands = 31;
    auto index = 0;
    m_numVertices = (latitudeBands + 1) * (longitudeBands + 1);
    m_vertices = new GLfloat[m_numVertices * 3];

    m_numIndices = latitudeBands * longitudeBands * 6;
    m_indices = new GLushort[m_numIndices];

    m_numNormals = m_numVertices;
    m_normals = new GLfloat[m_numNormals * 3];

    auto vertexCount = 0;
    for (auto latNumber = 0; latNumber <= latitudeBands; ++latNumber) {
        auto theta = latNumber * M_PI / (double)latitudeBands;
        auto sinTheta = std::sin(theta);
        auto cosTheta = std::cos(theta);

        for (auto longNumber = 0; longNumber <= longitudeBands; ++longNumber) {
            auto phi = longNumber * 2 * M_PI / (double) longitudeBands;
            auto sinPhi = std::sin(phi);
            auto cosPhi = std::cos(phi);
            GLfloat x = static_cast<GLfloat>(cosPhi * sinTheta);
            GLfloat y = static_cast<GLfloat>(cosTheta);
            GLfloat z = static_cast<GLfloat>(sinPhi * sinTheta);

            vertexCount++;

            addVertex(index, x * radius, y * radius, z * radius);
            addNormal(index, x, y, z);
            index += 3;
        }
    }

    index = 0;
    for (auto latNumber = 0; latNumber < latitudeBands; ++latNumber) {
        for (auto longNumber = 0; longNumber < longitudeBands; ++longNumber) {
            auto first = (latNumber * (longitudeBands + 1)) + longNumber;
            auto second = first + longitudeBands + 1;

            addIndex(index, static_cast<GLushort>(first));
            index++;
            addIndex(index, static_cast<GLushort>(second));
            index++;
            addIndex(index, static_cast<GLushort>(first + 1));
            index++;

            addIndex(index, static_cast<GLushort>(second));
            index++;
            addIndex(index, static_cast<GLushort>(second + 1));
            index++;
            addIndex(index, static_cast<GLushort>(first + 1));
            index++;
        }
    }

    m_specularPower = 32;
    m_specularIntensity = 1.0f;
}

void Sphere::initBuffers() {
    setVertices(m_vertices, m_numVertices, 3 * sizeof(GLfloat));
    setIndices(m_indices, m_numIndices * sizeof(GLushort));
    setNormals(m_normals, m_numNormals, 3 * sizeof(GLfloat));
}

void Sphere::addVertex(int index, float x, float y, float z) {
    if (index + 2 >= m_numVertices * 3) {
        LOGE("Vertex index out of bounds");
        return;
    }
    m_vertices[index] = x;
    m_vertices[index + 1] = y;
    m_vertices[index + 2] = z;
}

void Sphere::addNormal(int index, float x, float y, float z) {
    if (index + 2 >= m_numNormals * 3) {
        LOGE("Normal index out of bounds");
        return;
    }
    m_normals[index] = x;
    m_normals[index + 1] = y;
    m_normals[index + 2] = z;
}

void Sphere::addIndex(int index, GLushort value) {
    if (index >= m_numIndices) {
        LOGE("Index out of bounds");
        return;
    }
    m_indices[index] = value;
}

Sphere::~Sphere() {
    delete[] m_vertices;
    delete [] m_indices;
    delete [] m_normals;
}