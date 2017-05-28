#ifndef AWA_LINE_H
#define AWA_LINE_H


#include "../../core/Geometry.h"

class Line : public Geometry {
public:
    Line();
    ~Line();
    void setPoints(glm::vec3 start, glm::vec3 end);
    virtual GLenum getPrimitive() override;
private:
    GLfloat* m_vertices;
};


#endif //AWA_LINE_H
