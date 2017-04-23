#include "Sphere.h"

Sphere::Sphere() : Geometry::Geometry(),
                   m_pVertices(nullptr),
                   m_pNormals(nullptr),
                   m_pIndices(nullptr)
    {
    float radius = 0.5;
    uint latitudeBands = 30;
    uint longitudeBands = 30;
    int index = 0;
    uint numVertices = (latitudeBands + 1) * (longitudeBands + 1);
    m_pVertices = new GLfloat[numVertices * 3];

    uint numIndices = (latitudeBands + 1) * (longitudeBands + 1) * 6;
    m_pIndices = new GLushort[numIndices];

    m_pNormals = new GLfloat[numVertices * 3];

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

            m_pVertices[index] = x * radius;
            m_pNormals[index] = x;
            index++;

            m_pVertices[index] = y * radius;
            m_pNormals[index] = y;
            index++;

            m_pVertices[index] = z * radius;
            m_pNormals[index] = z;
            index++;
        }
    }

    index = 0;
    for (int latNumber = 0; latNumber <= latitudeBands; latNumber++) {
        for (int longNumber = 0; longNumber <= longitudeBands; longNumber++) {
            uint first = (uint)(latNumber * (longitudeBands + 1)) + longNumber;
            uint second = first + longitudeBands + 1;
            m_pIndices[index] = (GLushort)first;
            index++;
            m_pIndices[index] = (GLushort)second;
            index++;
            m_pIndices[index] = (GLushort)(first + 1);
            index++;

            m_pIndices[index] = (GLushort)second;
            index++;
            m_pIndices[index] = (GLushort)(second + 1);
            index++;
            m_pIndices[index] = (GLushort)(first + 1);
            index++;
        }
    }

    setVertices(m_pVertices, numVertices, 3 * sizeof(GLfloat));
    setIndices(m_pIndices, numIndices * 3 * sizeof(GLushort));
    setNormals(m_pNormals, numVertices, 3 * sizeof(GLfloat));
}

Sphere::~Sphere() {
    delete [] m_pVertices;
    delete [] m_pIndices;
    delete [] m_pNormals;
}