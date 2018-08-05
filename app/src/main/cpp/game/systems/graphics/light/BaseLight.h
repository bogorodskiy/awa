#ifndef AWA_BASELIGHT_H
#define AWA_BASELIGHT_H

struct BaseLight
{
    float ambientIntensity;
    float diffuseIntensity;
    physx::PxVec3 color;

    BaseLight()
    {
        ambientIntensity = 0.0f;
        diffuseIntensity = 0.0f;
        color = physx::PxVec3(0.0f, 0.0f, 0.0f);
    }
};

#endif //AWA_BASELIGHT_H
