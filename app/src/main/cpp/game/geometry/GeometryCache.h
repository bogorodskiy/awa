#ifndef AWA_GEOMETRYCACHE_H
#define AWA_GEOMETRYCACHE_H

#include <map>
#include <vector>
#include <memory>
#include "Geometry.h"

class GeometryCache {
public:
    static GeometryCache* getInstance();
    Geometry* getSphere();
    Geometry* getPlane(float width, float height);
    Geometry* getLine(glm::vec3 start, glm::vec3 end);
    void connect();
    void disconnect();
private:
    enum class GeometryType{
        SPHERE, PLANE
    };
    std::map<GeometryType, Geometry*> m_geometries;
    std::vector<Geometry*> m_uniqueGeometries;
    bool m_graphicsReady;

    GeometryCache();
    ~GeometryCache();
};

#endif //AWA_GEOMETRYCACHE_H
