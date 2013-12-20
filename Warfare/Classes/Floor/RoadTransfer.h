//
//  RoadTransfer.h
//  Warfare
//
//  Created by macbook_006 on 12/19/13.
//
//

#ifndef __Warfare__RoadTransfer__
#define __Warfare__RoadTransfer__

#include <iostream>
#include "cocos2d.h"
#include "PhysicsSprite.h"
USING_NS_CC;
class RoadTransfer : public PhysicsSprite {
public:
    ~RoadTransfer();
    RoadTransfer();
    void createRoad(b2World* world, CCPoint point, int number);
    void swivelBar(int degree);
    CCSprite* getSprite();
    void setScales(b2World* world);
    void setLinearDamping(float count);
private:
    int number;
    CCSprite* roadBar;
    CCPoint location;
    b2Body* body;
    
};


#endif /* defined(__Warfare__RoadTransfer__) */
