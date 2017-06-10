#ifndef IRONBALLS_GEOMETRY_H
#define IRONBALLS_GEOMETRY_H


#include <stdint.h>
#include "../../core/common.h"

class Geometry {
    friend class GeometryCache;

public:
    int getNumVertices();
    int getNumIndices();
    int getNumNormals();

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

    virtual void initBuffers() = 0;
    void deleteBuffers();

protected:
    int m_numVertices;
    int m_numIndices;
    int m_numNormals;

    Geometry();
    virtual ~Geometry();

private:
    GLenum m_primitive;
    glm::vec4 m_color;

    GLuint m_vbo;
    GLuint m_ibo;
    GLuint m_nbo;

    int m_verticesStride;
    int m_normalsStride;
};


#endif //IRONBALLS_GEOMETRY_H
