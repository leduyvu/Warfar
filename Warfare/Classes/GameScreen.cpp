//
//  HelloWorldScene.cpp
//  Warfare
//
//  Created by macbook_006 on 12/19/13.
//  Copyright __MyCompanyName__ 2013. All rights reserved.
//
#include "GameScreen.h"
#include "SimpleAudioEngine.h"
#include "RoadTransfer.h"
#include "MyContactListener.h"
#include "Ball.h"

using namespace cocos2d;
using namespace CocosDenshion;

enum {
    kTagParentNode = 1,
};

GameScreen::GameScreen()
{
    timeTouch = 0;
    isTouch = false;
    arrFloor = new CCArray();
    setTouchEnabled( true );
    setAccelerometerEnabled( true );

    CCSize s = CCDirector::sharedDirector()->getWinSize();
    // init physics
    this->initPhysics();

    CCSpriteBatchNode *parent = CCSpriteBatchNode::create("blocks.png", 100);
    m_pSpriteTexture = parent->getTexture();

    addChild(parent, 0, kTagParentNode);

    addNewSpriteAtPosition(ccp(s.width/2, s.height));

    CCLabelTTF *label = CCLabelTTF::create("Tap screen", "Marker Felt", 32);
    addChild(label, 0);
    label->setColor(ccc3(0,0,255));
    label->setPosition(ccp( s.width/2, s.height-50));
    
    //Player
    player = new Player();
    player->createPlayer(this->world, "Player/Player.png", ccp(200, 600));
    this->addChild(player);
    
    //Floor
    int type = 5;
    for(int i = 0; i <= 5; i ++){
        float pointXOld = 0;
        int max = rand() % 8;
        for(int j = 0; j <= max; j++){
            RoadTransfer* rf = new RoadTransfer();
            rf->setTag(1000);
            type = rand() % 7;
            pointXOld = rf->getContentSize().width/2 + pointXOld + (rand()% 3) * 100 + 50;
            rf->createRoad(this->world, ccp(pointXOld , 20  + i * 80), type);
            CCLOG("pos .x =  %f, pos.y = %f", rf->getPosition().x, rf->getPosition().y);
            pointXOld = rf->getContentSize().width + pointXOld;
            rf->setLinearDamping(i);
            this->addChild(rf, 9);
            arrFloor->addObject(rf);
        }
    }
    scheduleUpdate();
}

GameScreen::~GameScreen()
{
    delete arrFloor;
    arrFloor = NULL;
    delete world;
    world = NULL;
    
    //delete m_debugDraw;
}

void GameScreen::initPhysics()
{
    listener = new MyContactListener(this->world, this);
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    // Do we want to let bodies sleep?
    b2Vec2 gravity;
    gravity.Set(0.0f, -10.0f);
    world = new b2World(gravity);

    // Do we want to let bodies sleep?
    world->SetContactListener(listener);
    world->SetAllowSleeping(true);

    world->SetContinuousPhysics(true);

//     m_debugDraw = new GLESDebugDraw( PTM_RATIO );
//     world->SetDebugDraw(m_debugDraw);

    uint32 flags = 0;
    flags += b2Draw::e_shapeBit;
    //        flags += b2Draw::e_jointBit;
    //        flags += b2Draw::e_aabbBit;
    //        flags += b2Draw::e_pairBit;
    //        flags += b2Draw::e_centerOfMassBit;
    //m_debugDraw->SetFlags(flags);


    // Define the ground body.
    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0, 0); // bottom-left corner

    // Call the body factory which allocates memory for the ground body
    // from a pool and creates the ground box shape (also from a pool).
    // The body is also added to the world.
    b2Body* groundBody = world->CreateBody(&groundBodyDef);

    // Define the ground box shape.
    b2EdgeShape groundBox;

    // bottom

    groundBox.Set(b2Vec2(0,0), b2Vec2(s.width/PTM_RATIO,0));
    groundBody->CreateFixture(&groundBox,0);

    // top
    groundBox.Set(b2Vec2(0,s.height/PTM_RATIO), b2Vec2(s.width/PTM_RATIO,s.height/PTM_RATIO));
    groundBody->CreateFixture(&groundBox,0);

    // left
    groundBox.Set(b2Vec2(0,s.height/PTM_RATIO), b2Vec2(0,0));
    groundBody->CreateFixture(&groundBox,0);

    // right
    groundBox.Set(b2Vec2(s.width/PTM_RATIO,s.height/PTM_RATIO), b2Vec2(s.width/PTM_RATIO,0));
    groundBody->CreateFixture(&groundBox,0);
}

void GameScreen::draw()
{
    //
    // IMPORTANT:
    // This is only for debug purposes
    // It is recommend to disable it
    //
    CCLayer::draw();

    ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position );

    kmGLPushMatrix();

    world->DrawDebugData();

    kmGLPopMatrix();
}

void GameScreen::addNewSpriteAtPosition(CCPoint p)
{
    CCLOG("Add sprite %0.2f x %02.f",p.x,p.y);
    CCNode* parent = getChildByTag(kTagParentNode);
    
    //We have a 64x64 sprite sheet with 4 different 32x32 images.  The following code is
    //just randomly picking one of the images
    int idx = (CCRANDOM_0_1() > .5 ? 0:1);
    int idy = (CCRANDOM_0_1() > .5 ? 0:1);
    PhysicsSprite *sprite = new PhysicsSprite();
    sprite->initWithTexture(m_pSpriteTexture, CCRectMake(32 * idx,32 * idy,32,32));
    sprite->autorelease();
    sprite->setTag(111);
    
    parent->addChild(sprite);
    
    sprite->setPosition( CCPointMake( p.x, p.y) );
    
    // Define the dynamic body.
    //Set up a 1m squared box in the physics world
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(p.x/PTM_RATIO, p.y/PTM_RATIO);
    b2Body *body = world->CreateBody(&bodyDef);
    // Define another box shape for our dynamic body.
    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(.5f, .5f);//These are mid points for our 1m box
    
    // Define the dynamic body fixture.
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &dynamicBox;    
    fixtureDef.density = 11.0f;
    fixtureDef.friction = 0.3f;
    body->CreateFixture(&fixtureDef);
    
    sprite->setPhysicsBody(body);
}


void GameScreen::update(float dt)
{
    //It is recommended that a fixed time step is used with Box2D for stability
    //of the simulation, however, we are using a variable time step here.
    //You need to make an informed choice, the following URL is useful
    //http://gafferongames.com/game-physics/fix-your-timestep/
    
    int velocityIterations = 8;
    int positionIterations = 1;
    
    // Instruct the world to perform a single step of simulation. It is
    // generally best to keep the time step and iterations fixed.
    world->Step(dt, velocityIterations, positionIterations);
    
    //Iterate over the bodies in the physics world
    for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
    {
        if (b->GetUserData() != NULL) {
            //Synchronize the AtlasSprites position and rotation with the corresponding body
            CCSprite* myActor = (CCSprite*)b->GetUserData();
            myActor->setPosition( CCPointMake( b->GetPosition().x * PTM_RATIO, b->GetPosition().y * PTM_RATIO) );
            myActor->setRotation( -1 * CC_RADIANS_TO_DEGREES(b->GetAngle()) );
        }
        if(b->GetLinearDamping() == 21 || b->GetLinearDamping() == 222){
            world->DestroyBody(b);
            this->getChildByTag(kTagParentNode)->removeChildByTag(111);
        }
        
    }
    
    //impact handler
    if(dynamic_cast<MyContactListener*>(listener)->getA() == 1){
    }
    
    if(isTouch){
        timeTouch++;
    }
}

void GameScreen::ccTouchesBegan(CCSet* touches, CCEvent* event){
    isTouch = true;
    Ball* ball = new Ball();
//    ball->createBall(<#b2World *world#>, string str, <#cocos2d::CCPoint location#>)
    string str;
    ball->createBall(this->world, "Icon-Small.png", ccp(player->getBody()->GetPosition().x *32 , player->getBody()->GetPosition().y * 32 + 30));
    this->addChild(ball);
    ball->setTag(222);
}

void GameScreen::ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent){
}

void GameScreen::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
    
    //Add a new body/atlas sprite at the touched location
    CCSetIterator it;
    CCTouch* touch;
    CCPoint location;
    for( it = touches->begin(); it != touches->end(); it++) 
    {
        touch = (CCTouch*)(*it);
        
        if(!touch)
            break;
        
        location = touch->getLocationInView();
        
        location = CCDirector::sharedDirector()->convertToGL(location);
        
        addNewSpriteAtPosition( location );
    }
    
    float X = location.x - this->getPosition().x - this->player->getBody()->GetPosition().x * 32;
    float Y = location.y - this->player->getBody()->GetPosition().y * 32;
    float x = timeTouch/4 * X/(sqrt(X * X + Y * Y));
    float y = timeTouch/4 * Y/(sqrt(X * X + Y * Y));
    CCLOG("touches  * %f .. %f ", location.x, location.y);
    b2Vec2 ex = b2Vec2();
    ex.Set(x, y);
    dynamic_cast<PhysicsSprite*>(this->getChildByTag(222))->getBody()->SetLinearVelocity(ex);
//    player->getBody()->SetLinearVelocity(ex);
}

CCScene* GameScreen::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // add layer as a child to scene
    CCLayer* layer = new GameScreen();
    scene->addChild(layer);
    layer->release();
    
    return scene;
}
