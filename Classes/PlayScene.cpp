#include "PlayScene.h"
#include "LineContainer.h"
#include "Rocket.h"
#include "SimpleAudioEngine.h"
#include "ui/CocosGUI.h"
#include <string>

USING_NS_CC;

Scene* PlayScene::createScene() {
  return PlayScene::create();
}

bool PlayScene::init() {
  if (!Scene::init()) {
    return false;
  }

  auto visibleSize = Director::getInstance()->getVisibleSize();
  minimumLineLength = visibleSize.width * .07;

  Vec2 origin = Director::getInstance()->getVisibleOrigin();
  auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
  audio->playBackgroundMusic("Steamtech-Mayhem_Looping_v001.mp3", true);
  rocket = Rocket::create();

  isAnchored = false;

  backGround = Sprite::create("gameBg.png");
  backGround->setAnchorPoint({0, 0});
  backGround->setPosition({0, 0});
  backGround->setContentSize(visibleSize);

  lineContainer = LineContainer::create();
  lineContainer->reset();
  lineContainer->setVisible(true);

  comet = ParticleSystemQuad::create("comet.plist");
  comet->stopSystem();
  comet->setPosition(0, visibleSize.height * .6);
  comet->setVisible(false);

  this->addChild(backGround, kBackground);
  this->addChild(lineContainer, kBackground);
  this->addChild(rocket, kForeground);
  this->addChild(comet, kForeground);

  EventListenerTouchAllAtOnce* touches = EventListenerTouchAllAtOnce::create();
  touches->onTouchesBegan = CC_CALLBACK_2(PlayScene::onTouchesBegan, this);
  touches->onTouchesMoved = CC_CALLBACK_2(PlayScene::onTouchesMoved, this);
  touches->onTouchesEnded = CC_CALLBACK_2(PlayScene::onTouchesEnded, this);

  this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touches,
                                                                     this);
  reset();
  this->scheduleUpdate();

  return true;
}

Point PlayScene::touchToPoint(Touch* touch) {
  return CCDirector::getInstance()->convertToGL(touch->getLocationInView());
}

void PlayScene::showRestartDialog() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    restarButton = ui::Button::create("grey_button10.png", "grey_button09.png");
    restarButton->setTitleFontName("fonts/arial.ttf");
    restarButton->setTitleColor({0, 0, 0});
    restarButton->setTitleFontSize(12);
    restarButton->setTitleText("Restat");
    restarButton->addTouchEventListener(
        [&](Ref* sender, ui::Widget::TouchEventType type) {
          switch (type) {
            case ui::Widget::TouchEventType::BEGAN:
              break;
            case ui::Widget::TouchEventType::ENDED: {
              auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
              audio->stopBackgroundMusic();
              audio->playEffect("GUI Sound Effects_072.mp3");
              Director::getInstance()->replaceScene(TransitionFade::create(
                  0.5, PlayScene::createScene(), Color3B(255, 255, 255)));
            } break;
            default:
              break;
          }
        });
    restarButton->setPosition({visibleSize.width / 2, visibleSize.height / 2});
    std::string scoreStr = "You collect: " + std::to_string(score) + " research unit(s)";
    auto label = Label::createWithTTF(scoreStr.c_str(), "fonts/Marker Felt.ttf", 56);
    label->setColor({244,244,244});
    label->setPosition(
        Vec2(origin.x + visibleSize.width / 2,
             origin.y + visibleSize.height/2 + label->getContentSize().height));
    this->addChild(restarButton);
    this->addChild(label);
}

void PlayScene::update(float dt) {
  auto visibleSize = Director::getInstance()->getVisibleSize();
  if (lineContainer->getLineType() != LINE_NONE) {
    lineContainer->setTip(rocket->getPosition());
  }

  if(gameOver){
      return;
  }

  if (rocket->isCollidedWithSides()) {
    auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
    lineContainer->setLineType(LINE_NONE);
    rocket->setVisible(false);
    auto explosion = ParticleExplosion::create();
    explosion->setPosition(rocket->getPosition());
    explosion->setDuration(1.f);
    audio->playEffect("Explosion5.mp3");
    comet->stopSystem();
    audio->stopBackgroundMusic();
    this->addChild(explosion);
    audio->playBackgroundMusic("Factory-On-Mercury_Looping.mp3", true);
    gameOver = true;
    showRestartDialog();
  }

  rocket->update(dt);
  rocket->setOpacity(lineContainer->getEnergy() * 255);

  cometTimer += dt;
  float newY;

  if (cometTimer > cometInterval) {
    cometTimer = 0;
    if (!comet->isVisible()) {
      comet->setPositionX(0);
      newY = visibleSize.height * .2 +
             (float)rand() / ((float)RAND_MAX / visibleSize.height * .7);
      if (newY > visibleSize.height * .9) {
        newY = visibleSize.height * .9;
      }
      comet->setPositionY(newY);
      comet->setVisible(true);
      comet->resetSystem();
    }
  }

  if (comet->isVisible()) {
    if (rocket->getPosition().getDistance(comet->getPosition()) <
        rocket->getRadius()) {
      if (rocket->isVisible()) {
          comet->setVisible(false);
          auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
          audio->playEffect("GUI Sound Effects_078.mp3");
          rocket->setSpeed(rocket->getSpeed() + 35);
          lineContainer->setLineType(LINE_NONE);
          rocket->setRotationOrientation(RotationOrientation::NONE);
          comet->resetSystem();
          score++;
      }
    }
    comet->setPositionX(comet->getPositionX() + 50 * dt);
    if (comet->getPositionX() > visibleSize.width * 1.5) {
      comet->stopSystem();
      comet->setVisible(false);
    }
  }

  lineContainer->update(dt);
}

void PlayScene::onTouchesBegan(const std::vector<Touch*>& touches,
                               Event* unused_event) {
  Touch* touch = touches.at(0);
  if (!touch) {
    return;
  }
  Point tap = touch->getLocation();

  if (rocket->getPosition().getDistance(tap) < rocket->getRadius() * 1.2) {
    lineContainer->setLineType(LINE_NONE);
    rocket->setRotationOrientation(RotationOrientation::NONE);
  }
}
void PlayScene::onTouchesMoved(const std::vector<Touch*>& touches,
                               Event* unused_event) {
  Touch* touch = touches.at(0);
  if (!touch)
    return;

  Point tap = touch->getLocation();
  if (rocket->getPosition().getDistance(tap) > minimumLineLength) {
    lineContainer->setPivot(tap);
    lineContainer->setLineType(LINE_TEMP);
  } else {
    lineContainer->setLineType(LINE_NONE);
  }
}

void PlayScene::onTouchesEnded(const std::vector<Touch*>& touches,
                               Event* unused_event) {
  Touch* touch = touches.at(0);
  if (!touch) {
    return;
  }
  Point tap = touch->getLocation();

  if (lineContainer->getLineType() == LINE_TEMP) {
    lineContainer->setPivot(tap);
    lineContainer->setLineLength(rocket->getPosition().getDistance(tap));
    rocket->setPivot(tap);

    float circle_length = lineContainer->getLineLength() * 2 * M_PI;
    int iterations = floor(circle_length / rocket->getSpeed());
    rocket->setAngularSpeed(2 * M_PI / iterations);

    Point clockwise = (rocket->getPosition() - rocket->getPivot()).getRPerp();
    float dot = clockwise.dot(rocket->getVector());

    if (dot > 0) {
      rocket->setAngularSpeed(rocket->getAngularSpeed() * -1);
      rocket->setRotationOrientation(RotationOrientation::CLOCKWIZE);
      rocket->setTargetRotation(
          CC_RADIANS_TO_DEGREES(atan2(clockwise.y, clockwise.x)));
    } else {
      rocket->setRotationOrientation(RotationOrientation::COUNTER_CLOCKWIZE);
      rocket->setTargetRotation(
          CC_RADIANS_TO_DEGREES(atan2(-clockwise.y, -clockwise.x)));
    }
  }
  lineContainer->setLineType(LINE_DASHED);
}

void PlayScene::reset() {
  gameOver = false;
  score = 0;
  auto visibleSize = Director::getInstance()->getVisibleSize();
  cometTimer = 0;
  cometInterval = 2;
  rocket->setSpeed(120);
  rocket->setPosition(visibleSize.width / 2, visibleSize.height * .1);
}
