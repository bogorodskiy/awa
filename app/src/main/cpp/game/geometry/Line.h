#ifndef AWA_LINE_H
#define AWA_LINE_H


#include "Geometry.h"

class Line : public Geometry {
    friend class GeometryFactory;

public:
    virtual GLenum getPrimitive() override;
    virtual void initBuffers() override;

private:
    Line(glm::vec3 start, glm::vec3 end);
    ~Line();
    GLfloat* m_vertices;
};


#endif //AWA_LINE_H
