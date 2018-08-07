#include "Geometry.h"

Geometry::Geometry():
        m_specularIntensity(0.0f),
        m_specularPower(0.0f)
{
}

Geometry::~Geometry() {
    deleteBuffers();
}

int Geometry::getNumVertices() const {
    return m_numVertices;
}

int Geometry::getNumIndices() const {
    return m_numIndices;
}

int Geometry::getNumNormals() const {
    return m_numNormals;
}

int Geometry::getVerticesStride() const {
    return m_verticesStride;
}

int Geometry::getNormalsStride() const {
    return m_normalsStride;
}

GLenum Geometry::getPrimitive() const {
    return m_primitive;
}

Geometry::Type Geometry::getType() const {
    return m_type;
}

void Geometry::setVertices(GLfloat* data, int numVertices, int stride) {
    m_vbo = 0;
    m_verticesStride = stride;
    m_numVertices = numVertices;
    auto size = numVertices * stride;

    glGenBuffers(1, &m_vbo);
    bindVertexBuffer();
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    unbindVertexBuffer();
}

void Geometry::setIndices(GLushort* data, int size) {
    m_numIndices = size / sizeof(GLushort);
    glGenBuffers(1, &m_ibo);
    bindIndexBuffer();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    unbindIndexBuffer();
}

void Geometry::setNormals(GLfloat* data, int numNormals, int stride)
{
    m_nbo = 0;
    m_normalsStride = stride;
    m_numNormals = numNormals;
    auto size = numNormals * stride;

    glGenBuffers(1, &m_nbo);
    bindNormalBuffer();
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    unbindNormalBuffer();
}

float Geometry::getSpecularIntensity() const {
    return m_specularIntensity;
}

float Geometry::getSpecularPower() const {
    return m_specularPower;
}

void Geometry::bindVertexBuffer() {
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
}

void Geometry::bindIndexBuffer() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
}

void Geometry::bindNormalBuffer() {
    glBindBuffer(GL_ARRAY_BUFFER, m_nbo);
}

void Geometry::unbindVertexBuffer() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Geometry::unbindIndexBuffer() {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Geometry::unbindNormalBuffer() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Geometry::deleteBuffers() {
    if (m_numVertices > 0) {
        glDeleteBuffers(1, &m_vbo);
    }
    if (m_numIndices > 0) {
        glDeleteBuffers(1, &m_ibo);
    }
    if (m_numNormals > 0) {
        glDeleteBuffers(1, &m_nbo);
    }
}