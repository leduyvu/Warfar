//
//  PhysicSprite.h
//  Warfare
//
//  Created by macbook_006 on 12/19/13.
//
//

#ifndef __Warfare__PhysicsSprite__
#define __Warfare__PhysicsSprite__

#include <iostream>
#include "cocos2d.h"
#include "Box2D.h"

#define PTM_RATIO 32

class PhysicsSprite : public cocos2d::CCSprite
{
public:
    PhysicsSprite();
    void setPhysicsBody(b2Body * body);
    virtual bool isDirty(void);
    virtual cocos2d::CCAffineTransform nodeToParentTransform(void);
    b2Body* getBody();
private:
    b2Body* m_pBody;    // strong ref
};

#endif /* defined(__Warfare__PhysicsSprite__) */
