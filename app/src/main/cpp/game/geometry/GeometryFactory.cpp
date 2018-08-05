#include "GeometryFactory.h"
#include "Sphere.h"
#include "Plane.h"
#include "Line.h"

GeometryFactory::GeometryFactory():
        m_graphicsReady(false){
}

GeometryFactory::~GeometryFactory() {
    for (const auto& pair : m_geometries) {
        delete pair.second;
    }
    for (const auto& geometry: m_uniqueGeometries) {
        delete geometry;
    }
    m_geometries.clear();
    m_uniqueGeometries.clear();
}

GeometryFactory* GeometryFactory::getInstance() {
    static GeometryFactory instance;
    return &instance;
}

Geometry* GeometryFactory::getSphere(float radius) {
    if (m_geometries.find(GeometryType::SPHERE) != m_geometries.end()) {
        return m_geometries[GeometryType::SPHERE];
    }

    auto sphere = new Sphere(radius);
    if (m_graphicsReady) {
        sphere->initBuffers();
    }
    m_geometries[GeometryType::SPHERE] = sphere;
    return sphere;
}

Geometry* GeometryFactory::getPlane(float width, float height) {
    auto plane = new Plane(width, height);
    if (m_graphicsReady) {
        plane->initBuffers();
    }
    m_uniqueGeometries.emplace_back(plane);
    return plane;
}

Geometry* GeometryFactory::getLine(glm::vec3 start, glm::vec3 end) {
    auto line = new Line(start, end);
    if (m_graphicsReady) {
        line->initBuffers();
    }
    m_uniqueGeometries.emplace_back(line);
    return line;
}

void GeometryFactory::connect() {
    if (m_graphicsReady) {
        return;
    }

    m_graphicsReady = true;
    for (const auto& pair : m_geometries) {
        pair.second->initBuffers();
    }
    for (const auto& geometry: m_uniqueGeometries) {
        geometry->initBuffers();
    }
}

void GeometryFactory::disconnect() {
    if (!m_graphicsReady) {
        return;
    }

    m_graphicsReady = false;
    for (const auto& pair : m_geometries) {
        pair.second->deleteBuffers();
    }
    for (const auto& geometry: m_uniqueGeometries) {
        geometry->deleteBuffers();
    }
}