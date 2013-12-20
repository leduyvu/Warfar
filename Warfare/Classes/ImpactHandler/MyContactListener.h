//
//  MyContactListener.h
//  Warfare
//
//  Created by macbook_006 on 12/19/13.
//
//

#ifndef __Warfare__MyContactListener__
#define __Warfare__MyContactListener__

#include <iostream>
#include <Box2D/Box2D.h>
#include <vector>
#include "GameScreen.h"

struct MyContact {
    b2Fixture *fixtureA;
    b2Fixture *fixtureB;
    bool operator==(const MyContact& other) const
    {
        return (fixtureA == other.fixtureA) && (fixtureB == other.fixtureB);
    }
    };
    class MyContactListener : public b2ContactListener {
    public:
        std::vector<MyContact> _contacts;
        MyContactListener(b2World* world, CCLayer* layer);
        ~MyContactListener();
        
        virtual void BeginContact(b2Contact* contact);
        virtual void EndContact(b2Contact* contact);
        virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
        virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
        bool getResult();
        void setResult(bool result);
        int getA();
        int getB();
    private:
        int resultA;
        int resultB;
        CCLayer* layer;
        b2World* world;
        bool result;
        CC_SYNTHESIZE(int, _numberBegin, NumberBegin);
        CC_SYNTHESIZE(int, _numberEnd, NumberEnd);
    };

#endif /* defined(__Warfare__MyContactListener__) */
