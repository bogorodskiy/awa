#ifndef AWA_PLANE_H
#define AWA_PLANE_H

#include "Geometry.h"
#include <vector>

//The Plane lies on the YZ plane with "above" pointing towards positive X
class Plane : public Geometry {
    friend class GeometryCache;

public:
    virtual GLenum getPrimitive() override;
    virtual void initBuffers() override;

private:
    Plane(float width, float height);
    ~Plane();

    GLfloat* m_vertices;
    GLushort* m_indices;
    GLfloat* m_normals;
};

#endif //AWA_PLANE_H
