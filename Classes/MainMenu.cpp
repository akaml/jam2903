#include "MainMenu.h"
#include "PlayScene.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

Scene* MainMenu::createScene() {
  return MainMenu::create();
}

bool MainMenu::init() {
  auto visibleSize = Director::getInstance()->getVisibleSize();
  Vec2 origin = Director::getInstance()->getVisibleOrigin();

  backGround = Sprite::create("MenuBg.png");
  backGround->setAnchorPoint({0, 0});
  backGround->setPosition({0, 0});
  backGround->setContentSize(visibleSize);

  startButton = ui::Button::create("grey_button10.png", "grey_button09.png");
  startButton->setTitleFontName("fonts/arial.ttf");
  startButton->setTitleColor({0, 0, 0});
  startButton->setTitleFontSize(20);
  startButton->setTitleText("Play");
  startButton->addTouchEventListener(
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
  startButton->setPosition({visibleSize.width / 2, visibleSize.height / 2});
  auto label = Label::createWithTTF("Rocket Jam", "fonts/Marker Felt.ttf", 56);
  label->setColor({244, 244, 244});
  label->setPosition(
      Vec2(origin.x + visibleSize.width / 2,
           origin.y + visibleSize.height / 2 + label->getContentSize().height));

  auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
  audio->playBackgroundMusic("Factory-On-Mercury_Looping.mp3", true);
  audio->setBackgroundMusicVolume(0.5f);
  this->addChild(backGround);
  this->addChild(label);
  this->addChild(startButton);
  return true;
}
