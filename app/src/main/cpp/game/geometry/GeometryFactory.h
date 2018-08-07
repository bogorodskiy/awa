#ifndef AWA_GEOMETRYCACHE_H
#define AWA_GEOMETRYCACHE_H

#include <map>
#include <vector>
#include <memory>
#include "Geometry.h"

class GeometryFactory {
public:
    static GeometryFactory* getInstance();
    Geometry* getSphere(float radius);
    Geometry* getPlane(float width, float height);
    void connect();
    void disconnect();
private:
    std::map<Geometry::Type, Geometry*> m_geometries;
    std::vector<Geometry*> m_uniqueGeometries;
    bool m_graphicsReady;

    GeometryFactory();
    ~GeometryFactory();
};

#endif //AWA_GEOMETRYCACHE_H
