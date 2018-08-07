#ifndef IRONBALLS_GEOMETRY_H
#define IRONBALLS_GEOMETRY_H


#include <stdint.h>
#include "../../core/common.h"

class Geometry {
    friend class GeometryFactory;

public:
    enum class Type{
        NONE, SPHERE, PLANE
    };

    int getNumVertices() const;
    int getNumIndices() const;
    int getNumNormals() const;

    int getVerticesStride() const;
    int getNormalsStride() const;

    virtual GLenum getPrimitive() const;
    Type getType() const;
    void setVertices(GLfloat* data, int numVertices, int stride);
    void setIndices(GLushort* data, int size);
    void setNormals(GLfloat* data, int numNormals, int stride);

    float getSpecularIntensity() const;
    float getSpecularPower() const;

    void bindVertexBuffer();
    void bindIndexBuffer();
    void bindNormalBuffer();
    void unbindVertexBuffer();
    void unbindIndexBuffer();
    void unbindNormalBuffer();

    virtual void initBuffers() = 0;
    void deleteBuffers();

protected:
    Type m_type = Type::NONE;

    int m_numVertices = 0;
    int m_numIndices = 0;
    int m_numNormals = 0;

    float m_specularIntensity;
    float m_specularPower;

    Geometry();
    virtual ~Geometry();

private:
    GLenum m_primitive = GL_TRIANGLES;
    GLuint m_vbo = 0;
    GLuint m_ibo = 0;
    GLuint m_nbo = 0;

    int m_verticesStride = 0;
    int m_normalsStride = 0;
};


#endif //IRONBALLS_GEOMETRY_H
