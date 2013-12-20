//
//  RoadTransfer.cpp
//  GunnyGame
//
//  Created by macbook_006 on 13/09/25.
//
//

#include "RoadTransfer.h"
USING_NS_CC;
RoadTransfer::RoadTransfer()
{
    
}

void RoadTransfer::createRoad(b2World* world, CCPoint point, int number)
{
    string str = static_cast<ostringstream*>( &(ostringstream() << number) )->str();
    str = "Floor/roadWall" + str + ".png";
    this->roadBar = CCSprite::create(str.c_str());
    this->roadBar->setPosition(point);
    this->initWithTexture(roadBar->getTexture());
    this->location = point;
    this->setPosition(point);
    b2BodyDef bodyDef1;
    bodyDef1.type = b2_staticBody;
    bodyDef1.position.Set(point.x/32,(point.y)/32);
    this->body = world->CreateBody(&bodyDef1);
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(roadBar->getContentSize().width/64, roadBar->getContentSize().height/70);
    b2FixtureDef fixtureDef1;
    fixtureDef1.shape = &dynamicBox;
    fixtureDef1.density = 10.0f;
    fixtureDef1.friction = 0.2f;
    fixtureDef1.restitution = 0;
    this->body->CreateFixture(&fixtureDef1);
    b2Filter filter;
    filter.groupIndex = -1;
    body->GetFixtureList()[0].SetFilterData(filter);
    this->setPhysicsBody(body);
    this->body->SetGravityScale(0);
}

void RoadTransfer::swivelBar(int degree)
{
    
}

void RoadTransfer::setLinearDamping(float count){
    this->body->SetLinearDamping(count);
}

void RoadTransfer::setScales(b2World* world)
{
    this->setScaleX(0.35);
    this->setScaleY(2.5);
    b2BodyDef bodyDef1;
    bodyDef1.type = b2_dynamicBody;
    bodyDef1.position.Set(location.x/32, location.y/32);
    this->body = world->CreateBody(&bodyDef1);
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(roadBar->getContentSize().width/3.5/64, roadBar->getContentSize().height/64);
    b2FixtureDef fixtureDef1;
    fixtureDef1.shape = &dynamicBox;
    fixtureDef1.density = 900.0f;
    fixtureDef1.friction = 0.0f;
    this->body->CreateFixture(&fixtureDef1);
    this->setPhysicsBody(body);
    this->body->SetGravityScale(0);
}

CCSprite* RoadTransfer::getSprite()
{
    return this->roadBar;
}
RoadTransfer::~RoadTransfer(){}