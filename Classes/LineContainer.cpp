#include "LineContainer.h"

USING_NS_CC;
using namespace DrawPrimitives;

LineContainer::LineContainer() {
  _dash = 10;
  _dashSpace = 10;
  _lineType = LINE_NONE;
  _screenSize = Director::getInstance()->getVisibleSize();
  this->reset();
}

void LineContainer::reset() {}

LineContainer* LineContainer::create() {
  LineContainer* lineContainer = new LineContainer();
  if (lineContainer && lineContainer->DrawNode::init()) {
    lineContainer->autorelease();
    return lineContainer;
  }
  CC_SAFE_DELETE(lineContainer);
  return NULL;
}

void LineContainer::update(float delta) {}

void LineContainer::draw(Renderer* renderer,
                         const Mat4& transform,
                         uint32_t flags) {
  this->clear();
  switch (_lineType) {
    case LINE_NONE:
      break;
    case LINE_TEMP:
      drawSegment(_tip, _pivot, 3, Color4F::WHITE);
      drawDot(_pivot, 10, Color4F::WHITE);
      break;

    case LINE_DASHED:
      drawDot(_pivot, 10, Color4F::WHITE);

      int segments = _lineLentgh / (_dash + _dashSpace);
      float t = 0;
      float x_, y_;
      for (int i = 0; i < segments + 1; i++) {
        x_ = _pivot.x + t * (_tip.x - _pivot.x);
        y_ = _pivot.y + t * (_tip.y - _pivot.y);
        drawDot(Point(x_, y_), 4, Color4F::WHITE);
        t += (float)1 / segments;
      }
      break;
  }
  this->DrawNode::draw(renderer, transform, flags);
}

void LineContainer::setEnergyDecrement(float energyDecrement) {}
