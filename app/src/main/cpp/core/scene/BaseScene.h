#ifndef AWA_BASESCENE_H
#define AWA_BASESCENE_H

#include <string>

class BaseScene {
public:
    BaseScene();

    virtual const std::string& getId() const = 0;

    virtual void awake() = 0;
    virtual void update(float dt) = 0;
    virtual void sleep() = 0;
    virtual void finalize() = 0;
};


#endif //AWA_BASESCENE_H
