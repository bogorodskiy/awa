#ifndef IRONBALLS_SPHERE_H
#define IRONBALLS_SPHERE_H

#include "../../core/Geometry.h"

class Sphere : public Geometry {
public:
    Sphere(float radius);
    virtual ~Sphere();
private:
    GLfloat* m_vertices;
    GLfloat* m_normals;
    GLushort* m_indices;
};


#endif //IRONBALLS_SPHERE_H
