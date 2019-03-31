#ifndef ROCKET_H
#define ROCKET_H

#include "cocos2d.h"
USING_NS_CC;

enum class RotationOrientation : int { NONE, CLOCKWIZE, COUNTER_CLOCKWIZE };

class Rocket : public Layer {
 public:
  Rocket();
  virtual bool init();
  void update(float dt);
  void reset(void);
  bool isCollidedWithSides(void);

  CREATE_FUNC(Rocket)
  CC_SYNTHESIZE(float, _radius, Radius);
  CC_SYNTHESIZE(Point, _vector, Vector);
  CC_SYNTHESIZE(Point, _pivot, Pivot);
  CC_SYNTHESIZE(float, _speed, Speed);
  CC_SYNTHESIZE(float, _angularSpeed, AngularSpeed);
  CC_SYNTHESIZE(RotationOrientation, _rotationOrientation, RotationOrientation);

  inline void setTargetRotation(float value) { _targetRotation = value; }

  inline void setRotationFromVector() {
    this->setTargetRotation(
        CC_RADIANS_TO_DEGREES(atan2(-_vector.y, _vector.x)));
  }

 private:
  Sprite* sprite;
  ParticleSystem* trail;
  float _targetRotation;
  float _dr;
  float _ar;
  float _vr;
  float _rotationSpring;
  float _rotationDamping;
};

#endif  // ROCKET_H
