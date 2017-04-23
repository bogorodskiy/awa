#include "Geometry.h"

Geometry::Geometry():
        m_vbo(0),
        m_numVertices(0),
       // m_primitive(GL_TRIANGLES),
        m_primitive(GL_LINES),
        m_verticesStride(0),
        m_ibo(0),
        m_numIndices(0),
        m_numNormals(0),
        m_nbo(0),
        m_normalsStride(0)
{
}

uint Geometry::getNumVertices() {
    return m_numVertices;
}

uint Geometry::getNumIndices() {
    return m_numIndices;
}

uint Geometry::getNumNormals() {
    return m_numNormals;
}

int Geometry::getVerticesStride() {
    return m_verticesStride;
}

int Geometry::getNormalsStride() {
    return m_normalsStride;
}

GLenum Geometry::getPrimitive() {
    return m_primitive;
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

Geometry::~Geometry() {
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ibo);
    glDeleteBuffers(1, &m_nbo);
}