#ifndef AWA_LINE_H
#define AWA_LINE_H


#include "../../core/Geometry.h"
#include <vector>

class Line : public Geometry {
public:
    Line();
    void setPoints(glm::vec3 start, glm::vec3 end);
    virtual GLenum getPrimitive() override;
private:
    GLfloat* m_vertices;
    std::vector<GLushort> m_indices;
};


#endif //AWA_LINE_H
