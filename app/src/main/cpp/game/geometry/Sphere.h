#ifndef IRONBALLS_SPHERE_H
#define IRONBALLS_SPHERE_H

#include "Geometry.h"

class Sphere : public Geometry {
    friend class GeometryCache;
public:
    virtual void initBuffers() override;
private:
    Sphere(float radius);
    virtual ~Sphere();

    GLfloat* m_vertices;
    GLfloat* m_normals;
    GLushort* m_indices;
};


#endif //IRONBALLS_SPHERE_H
