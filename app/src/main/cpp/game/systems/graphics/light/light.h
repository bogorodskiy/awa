#ifndef IRON_BALLS_LIGHT_H
#define IRON_BALLS_LIGHT_H

// TODO: delete
struct DirectionalLight
{
    physx::PxVec3 color;
    physx::PxVec3 direction;
    float diffuseIntensity;
};

struct DirectionalLightLocation {
    GLint color;
    GLint direction;
    GLint diffuseIntensity;
};

#endif //IRON_BALLS_LIGHT_H
