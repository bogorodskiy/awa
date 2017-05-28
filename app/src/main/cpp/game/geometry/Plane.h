#ifndef AWA_PLANE_H
#define AWA_PLANE_H

#include "../../core/Geometry.h"
#include <vector>

class Plane : public Geometry {
public:
    Plane();
    ~Plane();
    void setSize(int value);
    virtual GLenum getPrimitive() override;
private:
    GLfloat* m_vertices;
};

#endif //AWA_PLANE_H
