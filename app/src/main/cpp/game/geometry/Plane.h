#ifndef AWA_PLANE_H
#define AWA_PLANE_H

#include "Geometry.h"
#include <vector>

//The Plane lies on the YZ plane with "above" pointing towards positive X
class Plane : public Geometry {
    friend class GeometryFactory;

public:
    virtual void initBuffers() override;

private:
    Plane(float width, float height);
    ~Plane();

    GLfloat* m_vertices = nullptr;
    GLushort* m_indices = nullptr;
    GLfloat* m_normals = nullptr;
};

#endif //AWA_PLANE_H
