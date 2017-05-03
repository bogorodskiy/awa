#include "Sphere.h"

Sphere::Sphere() : Geometry::Geometry(),
                   m_vertices(nullptr),
                   m_normals(nullptr),
                   m_indices(nullptr)
    {
    float radius = 0.5;
    uint latitudeBands = 30;
    uint longitudeBands = 30;
    int index = 0;
    uint numVertices = (latitudeBands + 1) * (longitudeBands + 1);
    m_vertices = new GLfloat[numVertices * 3];

    uint numIndices = (latitudeBands + 1) * (longitudeBands + 1) * 6;
    m_indices = new GLushort[numIndices];

    m_normals = new GLfloat[numVertices * 3];

    for (int latNumber = 0; latNumber <= latitudeBands; latNumber++) {
        double theta = latNumber * M_PI / (double)latitudeBands;
        double sinTheta = glm::sin(theta);
        double cosTheta = glm::cos(theta);

        for (int longNumber = 0; longNumber <= longitudeBands; longNumber++) {
            double phi = longNumber * 2 * M_PI / (double) longitudeBands;
            double sinPhi = glm::sin(phi);
            double cosPhi = glm::cos(phi);
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
    for (int latNumber = 0; latNumber <= latitudeBands; latNumber++) {
        for (int longNumber = 0; longNumber <= longitudeBands; longNumber++) {
            uint first = (uint)(latNumber * (longitudeBands + 1)) + longNumber;
            uint second = first + longitudeBands + 1;
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

    setVertices(m_vertices, numVertices, 3 * sizeof(GLfloat));
    setIndices(m_indices, numIndices * 3 * sizeof(GLushort));
    setNormals(m_normals, numVertices, 3 * sizeof(GLfloat));
}

Sphere::~Sphere() {
    delete [] m_vertices;
    delete [] m_indices;
    delete [] m_normals;
}