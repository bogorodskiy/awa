#include "GeometryCache.h"
#include "Sphere.h"
#include "Plane.h"
#include "Line.h"

GeometryCache::GeometryCache():
        m_graphicsReady(false){
}

GeometryCache::~GeometryCache() {
    for (const auto& pair : m_geometries) {
        delete pair.second;
    }
    for (const auto& geometry: m_debugGeometries) {
        delete geometry;
    }
    m_geometries.clear();
    m_debugGeometries.clear();
}

GeometryCache* GeometryCache::getInstance() {
    static GeometryCache instance;
    return &instance;
}

Geometry* GeometryCache::getSphere() {
    if (m_geometries.find(GeometryType::SPHERE) != m_geometries.end()) {
        return m_geometries[GeometryType::SPHERE];
    }

    auto sphere = new Sphere(0.5f);
    if (m_graphicsReady) {
        sphere->initBuffers();
    }
    m_geometries[GeometryType::SPHERE] = sphere;
    return sphere;
}

Geometry* GeometryCache::getPlane() {
//    if (m_geometries.find(GeometryType::PLANE) != m_geometries.end()) {
//        return m_geometries[GeometryType::PLANE];
//    }

    auto plane = new Plane(10);
    if (m_graphicsReady) {
        plane->initBuffers();
    }
//    m_geometries[GeometryType::PLANE] = plane;
    m_debugGeometries.emplace_back(plane);
    return plane;
}

Geometry* GeometryCache::getLine(glm::vec3 start, glm::vec3 end) {
    auto line = new Line(start, end);
    if (m_graphicsReady) {
        line->initBuffers();
    }
    m_debugGeometries.emplace_back(line);
    return line;
}

void GeometryCache::connect() {
    if (m_graphicsReady) {
        return;
    }

    m_graphicsReady = true;
    for (const auto& pair : m_geometries) {
        pair.second->initBuffers();
    }
    for (const auto& geometry: m_debugGeometries) {
        geometry->initBuffers();
    }
}

void GeometryCache::disconnect() {
    if (!m_graphicsReady) {
        return;
    }

    m_graphicsReady = false;
    for (const auto& pair : m_geometries) {
        pair.second->deleteBuffers();
    }
    for (const auto& geometry: m_debugGeometries) {
        geometry->deleteBuffers();
    }
}