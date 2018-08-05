#ifndef IRONBALLS_GEOMETRY_H
#define IRONBALLS_GEOMETRY_H


#include <stdint.h>
#include "../../core/common.h"

class Geometry {
    friend class GeometryFactory;

public:
    int getNumVertices();
    int getNumIndices();
    int getNumNormals();

    int getVerticesStride();
    int getNormalsStride();

    virtual GLenum getPrimitive();
    void setPrimitive(GLenum primitive);
    void setVertices(GLfloat* data, int numVertices, int stride);
    void setIndices(GLushort* data, int size);
    void setNormals(GLfloat* data, int numNormals, int stride);

    float getSpecularIntensity();
    float getSpecularPower();

    void bindVertexBuffer();
    void bindIndexBuffer();
    void bindNormalBuffer();
    void unbindVertexBuffer();
    void unbindIndexBuffer();
    void unbindNormalBuffer();

    virtual void initBuffers() = 0;
    void deleteBuffers();

protected:
    int m_numVertices = 0;
    int m_numIndices = 0;
    int m_numNormals = 0;

    float m_specularIntensity;
    float m_specularPower;

    Geometry();
    virtual ~Geometry();

private:
    GLenum m_primitive;
    GLuint m_vbo = 0;
    GLuint m_ibo = 0;
    GLuint m_nbo = 0;

    int m_verticesStride = 0;
    int m_normalsStride = 0;
};


#endif //IRONBALLS_GEOMETRY_H
