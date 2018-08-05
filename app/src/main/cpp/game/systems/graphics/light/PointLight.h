#ifndef AWA_POINTLIGHT_H
#define AWA_POINTLIGHT_H

#include "BaseLight.h"

struct LightAttenuation {
    float constantFactor;
    float linearFactor;
    float exponentialFactor;
};

struct PointLight : public BaseLight {
    LightAttenuation attenuation;
    physx::PxVec3 position;

    PointLight() {
        // lightDistanceStrength = lightUnitDistanceStrength / (constantFactor + linearFactor * distance + exponentialFactor * distance * distance)
        attenuation.constantFactor = 1.0f;
        attenuation.linearFactor = 1.5f;
        attenuation.exponentialFactor = 0.0f;
        position = physx::PxVec3(0.0f, 0.0f, 0.0f);
    }
};

#endif //AWA_POINTLIGHT_H
