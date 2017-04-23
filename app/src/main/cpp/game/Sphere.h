#ifndef IRONBALLS_SPHERE_H
#define IRONBALLS_SPHERE_H

#include "../core/Geometry.h"

class Sphere : public Geometry {
public:
    Sphere();
    virtual ~Sphere();
private:
    GLfloat* m_pVertices;
    GLfloat* m_pNormals;
    GLushort* m_pIndices;
};


#endif //IRONBALLS_SPHERE_H
