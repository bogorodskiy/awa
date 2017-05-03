#ifndef IRONBALLS_GEOMETRY_H
#define IRONBALLS_GEOMETRY_H


#include <stdint.h>
#include "common.h"

class Geometry {
public:
    Geometry();
    virtual ~Geometry();

    uint getNumVertices();
    uint getNumIndices();
    uint getNumNormals();

    int getVerticesStride();
    int getNormalsStride();

    virtual GLenum getPrimitive();
    void setPrimitive(GLenum primitive);
    glm::vec4* getColor();
    void setColor(float r, float g, float b, float a);
    void setVertices(GLfloat* data, int numVertices, int stride);
    void setIndices(GLushort* data, int size);
    void setNormals(GLfloat* data, int numNormals, int stride);

    void bindVertexBuffer();
    void bindIndexBuffer();
    void bindNormalBuffer();
    void unbindVertexBuffer();
    void unbindIndexBuffer();
    void unbindNormalBuffer();

private:
    GLuint m_vbo;
    glm::vec4 m_color;

    uint m_numVertices;
    GLenum m_primitive;
    int m_verticesStride;

    GLuint m_ibo;
    uint m_numIndices;

    uint m_numNormals;
    GLuint m_nbo;
    int m_normalsStride;
};


#endif //IRONBALLS_GEOMETRY_H
