#include "Rocket.h"
#include "cocos2d.h"

USING_NS_CC;

Rocket::Rocket()
    : _targetRotation(0),
      _dr(0),
      _ar(0),
      _vr(0),
      _rotationSpring(.2),
      _rotationDamping(.5),
      _pivot(Point(-1, -1)),
      _rotationOrientation(RotationOrientation::NONE) {
  this->reset();
}

bool Rocket::init() {
  sprite = Sprite::create("spaceRockets_003.png");
  sprite->setFlippedY(true);
  sprite->setScale(0.3f);
  sprite->setRotation(-90);
  setRadius(sprite->getBoundingBox().size.width / 2);
  this->addChild(sprite);
  trail = ParticleFire::create();
  trail->setDuration(ParticleSystem::DURATION_INFINITY);
  trail->setEmitterMode(ParticleSystem::Mode::RADIUS);
  trail->setStartRadius(200);
  trail->setStartRadiusVar(0);
  trail->setLife(1.f);
  sprite->addChild(trail);
  trail->setPosition({70, 500});
  return true;
}

void Rocket::reset() {
  _speed = 120;
  _pivot = Point(-1, 1);
  _rotationOrientation = RotationOrientation::NONE;
  this->setRotation(90);
  _targetRotation = -90;
  float angle = CC_DEGREES_TO_RADIANS(this->getRotation());
  _vector = Point(_speed * cos(angle), _speed * sin(angle));
}

void Rocket::update(float dt) {
  Point position = this->getPosition();

  if (_rotationOrientation == RotationOrientation::NONE) {
    position += _vector * dt;

  } else {
    Point rotatedPoint = position.rotateByAngle(_pivot, _angularSpeed * dt);
    position = rotatedPoint;

    float rotatedAngle = 0;
    Point clockwise = (position - _pivot).getRPerp();

    if (_rotationOrientation == RotationOrientation::COUNTER_CLOCKWIZE) {
      rotatedAngle = atan2(-clockwise.y, -clockwise.x);
    } else {
      rotatedAngle = atan2(clockwise.y, clockwise.x);
    }

    _vector.x = _speed * cos(rotatedAngle);
    _vector.y = _speed * sin(rotatedAngle);

    this->setRotationFromVector();

    if (this->getRotation() > 0) {
      this->setRotation(fmodf(this->getRotation(), 360.0));
    } else {
      this->setRotation(fmodf(this->getRotation(), -360.0));
    }
  }

  if (_targetRotation > this->getRotation() + 180) {
    _targetRotation -= 360;
  }
  if (_targetRotation < this->getRotation() - 180) {
    _targetRotation += 360;
  }

  this->setPosition(position);

  _dr = _targetRotation - this->getRotation();
  _ar = _dr * _rotationSpring;
  _vr += _ar;
  _vr *= _rotationDamping;
  this->setRotation(this->getRotation() + _vr);
}

bool Rocket::isCollidedWithSides() {
  Size screenSize = CCDirector::getInstance()->getWinSize();

  if (_position.x > screenSize.width - _radius) {
    _position.x = screenSize.width - _radius;
    _rotationOrientation = RotationOrientation::NONE;
    _vector = Point(this->getVector().x * -1, this->getVector().y);
    this->setRotationFromVector();
    return true;
  }

  if (_position.x < _radius) {
    _position.x = _radius;
    _rotationOrientation = RotationOrientation::NONE;
    _vector = Point(this->getVector().x * -1, this->getVector().y);
    this->setRotationFromVector();
    return true;
  }

  if (_position.y < _radius) {
    _position.y = _radius;
    _rotationOrientation = RotationOrientation::NONE;
    _vector = Point(this->getVector().x, this->getVector().y * -1);
    this->setRotationFromVector();
    return true;
  }

  if (_position.y > screenSize.height - _radius) {
    _position.y = screenSize.height - _radius;
    _rotationOrientation = RotationOrientation::NONE;
    _vector = Point(this->getVector().x, this->getVector().y * -1);
    this->setRotationFromVector();
    return true;
  }

  return false;
}
