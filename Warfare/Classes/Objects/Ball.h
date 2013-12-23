//
//  Ball.h
//  GunnyGame
//
//  Created by macbook_006 on 13/09/30.
//
//

#ifndef __GunnyGame__Ball__
#define __GunnyGame__Ball__

#include <iostream>
#include "cocos2d.h"
#include <b2Math.h>
#include "PhysicsSprite.h"
#include "MyContactListener.h"
USING_NS_CC;
using namespace std;
//struct MyContact {
//    b2Fixture *fixtureA;
//    b2Fixture *fixtureB;
//    bool operator==(const MyContact& other) const
//    {
//        return (fixtureA == other.fixtureA) && (fixtureB == other.fixtureB);
//    }
//};
class Ball : public PhysicsSprite{
public:
    std::vector<MyContact> _contacts;
    Ball();
    ~Ball();
    void createBall(b2World * world, string str, CCPoint location);
    void throwBall(b2Vec2 vector);
    CCSprite* getSprite();
    
//    virtual void BeginContact(b2Contact* contact);
//    virtual void EndContact(b2Contact* contact);
//    virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
//    virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
private:
    CCSprite* sprBall;
//    CC_SYNTHESIZE(int, _numberBegin, NumberBegin);
//    CC_SYNTHESIZE(int, _numberEnd, NumberEnd);
};

#endif /* defined(__GunnyGame__Ball__) */
