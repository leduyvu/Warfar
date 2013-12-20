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
    
private:
    Player* player;
    b2ContactListener* listener;
    b2World* world;
    cocos2d::CCTexture2D* m_pSpriteTexture; // weak ref
    CCArray* arrFloor;
    bool isTouch;
    float timeTouch;
};

#endif // __HELLO_WORLD_H__
