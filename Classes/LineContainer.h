#ifndef LINECONTAINER_H
#define LINECONTAINER_H

#include "cocos2d.h"

USING_NS_CC;

typedef enum LineType : int {
  LINE_NONE,
  LINE_TEMP,
  LINE_DASHED,
} LineType;

class LineContainer : public cocos2d::DrawNode {
 public:
  LineContainer();
  static LineContainer* create();

  virtual void update(float delta);
  virtual void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;

  void setEnergyDecrement(float energyDecrement);
  void reset(void);

  CC_SYNTHESIZE(float, _energy, Energy);
  CC_SYNTHESIZE(Point, _pivot, Pivot);
  CC_SYNTHESIZE(Point, _tip, Tip);
  CC_SYNTHESIZE(float, _lineLentgh, LineLength);
  CC_SYNTHESIZE(LineType, _lineType, LineType);

 protected:
 private:
  int _dash;
  int _dashSpace;
  Size _screenSize;
  float _energyLineX;
  float _energyHeight;

  float _energyDecrement;
};

#endif  // LINECONTAINER_H
