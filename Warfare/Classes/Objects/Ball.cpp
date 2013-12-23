//
//  Ball.cpp
//  GunnyGame
//
//  Created by macbook_006 on 13/09/30.
//
//

#include "Ball.h"
#include "cocos2d.h"
USING_NS_CC;
Ball::Ball(){
}
Ball::~Ball(){
}
void Ball::createBall(b2World * world, string str, CCPoint location)
{
    sprBall = CCSprite::create("Icon-Small.png");
    sprBall->setPosition(location);
    this->initWithTexture(sprBall->getTexture());
    this->setPosition(location);
    b2BodyDef bodyDef1;
    bodyDef1.type = b2_dynamicBody;
    bodyDef1.position.Set(location.x/32, location.y/32);
    b2Body* body = world->CreateBody(&bodyDef1);
    b2CircleShape dynamicBox;
    dynamicBox.m_radius = 0.3f;
    b2FixtureDef fixtureDef1;
    fixtureDef1.shape = &dynamicBox;
    fixtureDef1.density = 3;
    fixtureDef1.friction = 0.0f;
    fixtureDef1.restitution = 1;
    body->CreateFixture(&fixtureDef1);
    b2Filter filter;
    filter.groupIndex = -1;
    body->GetFixtureList()[0].SetFilterData(filter);
    body->SetLinearDamping(0.01);
    this->setPhysicsBody(body);
}
void Ball::throwBall(b2Vec2 vectorV){
//    this->getBody()->SetLinearVelocity(vectorV);
}

CCSprite* Ball::getSprite(){
    return this->sprBall;
}

//void Ball::BeginContact(b2Contact* contact){
//    MyContact myContact = { contact->GetFixtureA(), contact->GetFixtureB() };
//    _contacts.push_back(myContact);
//    {
//        if(myContact.fixtureA->GetDensity() == 10)
//        {
//            myContact.fixtureA->GetBody()->GetFixtureList()[0].SetSensor(true);
//            myContact.fixtureA->GetBody()->SetLinearDamping(2222);
//            CCLOG("AAAA");
//        }
//        if(myContact.fixtureB->GetDensity() == 3){
//            myContact.fixtureB->GetBody()->ResetMassData();
//            myContact.fixtureB->GetBody()->GetFixtureList()[0].SetSensor(true);
//            myContact.fixtureB->GetBody()->SetLinearDamping(2225);
//            CCLOG("AAAA");
//        }
//    }
//}