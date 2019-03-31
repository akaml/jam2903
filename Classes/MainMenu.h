#ifndef MAINMENU_H
#define MAINMENU_H

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "PlayScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

class MainMenu : public Scene
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    CREATE_FUNC(MainMenu);

private:
    ui::Button* startButton;
    Sprite* backGround;
};

#endif // MAINMENU_H
