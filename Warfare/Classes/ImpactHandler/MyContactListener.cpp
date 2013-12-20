//
//  MyContactListener.cpp
//  Warfare
//
//  Created by macbook_006 on 12/19/13.
//
//

#include "MyContactListener.h"
#include "SimpleAudioEngine.h"
#include "cocos2d.h"
MyContactListener::MyContactListener(b2World* world, CCLayer* layer){
    result = false;
    this->world = world;
    this->layer = layer;
    
}
MyContactListener::~MyContactListener(){}
void MyContactListener::BeginContact(b2Contact* contact){
    MyContact myContact = { contact->GetFixtureA(), contact->GetFixtureB() };
    _contacts.push_back(myContact);
    {
        if(myContact.fixtureA->GetDensity() == 11)
        {
            resultA = 1;
            myContact.fixtureA->GetBody()->SetLinearDamping(21);
            myContact.fixtureA->GetBody()->GetFixtureList()[0].SetSensor(true);
            myContact.fixtureB->GetBody()->SetLinearDamping(21);
            CCLOG("AAAA");
        }
        if(myContact.fixtureB->GetDensity() == 10 && myContact.fixtureB->GetBody() != NULL)
        {
//            myContact.fixtureB->GetBody()->SetLinearDamping(21);
            resultB = 1;
            // myContact.fixtureA->GetBody()->GetFixtureList()[0].SetSensor(true);
            CCLOG("BBBB");
        }
    }
}
void MyContactListener::EndContact(b2Contact* contact){
}
void MyContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold){}
void MyContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse){}
bool MyContactListener::getResult()
{
    return result;
}
void MyContactListener::setResult(bool result)
{
    this->result = result;
}

int MyContactListener::getA(){
    return this->resultA;
}

int MyContactListener::getB(){
    return this->resultB;
}
