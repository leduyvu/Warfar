//
//  HelloWorldScene.h
//  Warfare
//
//  Created by macbook_006 on 12/19/13.
//  Copyright __MyCompanyName__ 2013. All rights reserved.
//
#ifndef __GAME_SCREEN_H__
#define __GAME_SCREEN_H__

// When you import this file, you import all the cocos2d classes
#include "cocos2d.h"
#include "Floor/RoadTransfer.h"
#include "PhysicsSprite.h"
#include "MyContactListener.h"
#include "Player.h"

//#include "Box2D.h"
//
//class PhysicsSprite : public cocos2d::CCSprite
//{
//public:
//    PhysicsSprite();
//    void setPhysicsBody(b2Body * body);
//    virtual bool isDirty(void);
//    virtual cocos2d::CCAffineTransform nodeToParentTransform(void);
//private:
//    b2Body* m_pBody;    // strong ref
//};

class GameScreen : public cocos2d::CCLayer {
public:
    ~GameScreen();
    GameScreen();
    
    // returns a Scene that contains the HelloWorld as the only child
    static cocos2d::CCScene* scene();
    
    void initPhysics();
    // adds a new sprite at a given coordinate
    void addNewSpriteAtPosition(cocos2d::CCPoint p);

    virtual void draw();
    virtual void ccTouchesEnded(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
    void update(float dt);
    
    virtual void ccTouchesBegan(CCSet* touches, CCEvent* event);
    virtual void ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
    void impactHandler();
    
    //joystick
    float heronsformula(float x1,float y1,float x2,float y2,float x3,float y3);
    bool triangleContainPoint(float x1,float y1,float x2,float y2,float x3,float y3,float px,float py);
    
    
private:
    void flying(float dt);
    
    cocos2d::CCSprite *joystick;
    //中心点O
    cocos2d::CCPoint O;
    //大圆半径
    float R;
    
    cocos2d::CCSprite *plane;
    //飞机飞行的速度分量值
    float speedX;
    float speedY;
    //是否飞行的标志
    bool isFlying;
    
    
    bool isBallSTRun = false;
    bool isBallNDRun = false;
    bool isBallST = false;
    bool isBallND = false;
    bool playerSTRun = false;
    bool playerNDRun = false;
    CCArray* arrBall;
    Player* playerST, *playerND;
    b2ContactListener* listener;
    b2World* world;
    cocos2d::CCTexture2D* m_pSpriteTexture; // weak ref
    CCArray* arrFloor;
    bool isTouchST, isTouchND;
    float timeTouchST;
    float timeTouchND;
    
    //shoot
    CCSprite* shootButtonST;
    CCSprite* shootButtonND;
    bool shootingST = false;
    bool shootingND = false;
    bool isShootST = false;
    bool isShootND = false;
};

#endif // __HELLO_WORLD_H__
