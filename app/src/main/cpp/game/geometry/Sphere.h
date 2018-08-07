#ifndef IRONBALLS_SPHERE_H
#define IRONBALLS_SPHERE_H

#include "Geometry.h"

class Sphere : public Geometry {
    friend class GeometryFactory;
public:
    virtual void initBuffers() override;
private:
    GLfloat* m_vertices = nullptr;
    GLfloat* m_normals = nullptr;
    GLushort* m_indices = nullptr;

    Sphere(float radius);
    virtual ~Sphere();

    void addVertex(int index, float x, float y, float z);
    void addNormal(int index, float x, float y, float z);
    void addIndex(int index, GLushort value);
};


#endif //IRONBALLS_SPHERE_H
