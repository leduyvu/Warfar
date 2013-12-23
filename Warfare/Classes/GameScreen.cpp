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
    arrBall = new CCArray();
    timeTouchST = 0;
    timeTouchND = 0;
    isTouchST = false;
    isTouchND = false;
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
    playerST = new Player();
    playerST->createPlayer(this->world, "Player/Player.png", ccp(200, 600));
    this->addChild(playerST);
    
    playerND = new Player();
    playerND->createPlayer(this->world, "Player/Player.png", ccp(600, 600));
    this->addChild(playerND);
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
    //shoot sprite
    shootButtonND = CCSprite::create("Icon.png");
    shootButtonST = CCSprite::create("Icon.png");
    shootButtonST->setPosition(ccp(50, 50));
    shootButtonND->setPosition(ccp(s.width - 50, 50));
    this->addChild(shootButtonND);
    this->addChild(shootButtonST);
    
    this->schedule(schedule_selector(GameScreen::flying));
    
    isFlying=false;
    speedX=speedY=0;
    
    scheduleUpdate();
}

GameScreen::~GameScreen()
{
    delete arrFloor;
    arrFloor = NULL;
    delete world;
    world = NULL;
    delete arrBall;
    arrBall = NULL;
    
    //delete m_debugDraw;
}

void GameScreen::initPhysics()
{
    CCSize s = CCDirector::sharedDirector()->getWinSize();
    // Do we want to let bodies sleep?
    b2Vec2 gravity;
    gravity.Set(0.0f, -10.0f);
    world = new b2World(gravity);

    // Do we want to let bodies sleep?
//    world->SetContactListener(listener);
    world->SetAllowSleeping(true);

    world->SetContinuousPhysics(true);

//     m_debugDraw = new GLESDebugDraw( PTM_RATIO );
//     world->SetDebugDraw(m_debugDraw);

    uint32 flags = 0;
    flags += b2Draw::e_shapeBit;
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
//    groundBox.Set(b2Vec2(0,s.height/PTM_RATIO), b2Vec2(s.width/PTM_RATIO,s.height/PTM_RATIO));
//    groundBody->CreateFixture(&groundBox,0);

    // left
    groundBox.Set(b2Vec2(0,s.height/PTM_RATIO), b2Vec2(0,0));
    groundBody->CreateFixture(&groundBox,0);

    // right
    groundBox.Set(b2Vec2(s.width/PTM_RATIO,s.height/PTM_RATIO), b2Vec2(s.width/PTM_RATIO,0));
    groundBody->CreateFixture(&groundBox,0);
}

void GameScreen::draw()
{
    CCLayer::draw();

    ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position );

    kmGLPushMatrix();

    world->DrawDebugData();

    kmGLPopMatrix();
}

void GameScreen::addNewSpriteAtPosition(CCPoint p){
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
//        if(b->GetLinearDamping() == 0.2){
//            world->DestroyBody(b);
////            this->getChildByTag(kTagParentNode)->removeChildByTag(111);
//            this->removeChildByTag(222);
//            isBall = false;
//        }
//        if(b->GetLinearDamping() == 0.5){
//            world->DestroyBody(b);
//            //            this->getChildByTag(kTagParentNode)->removeChildByTag(111);
////            this->removeChildByTag(222);
//            isBall = false;
//        }
    }
    
    //impact handler
    if(isTouchST){
        timeTouchST++;
    }
    if(isTouchND){
        timeTouchND++;
    }
    impactHandler();
    
    if(fabs(playerST->getBody()->GetLinearVelocity().x) < 0.01 && fabs(playerST->getBody()->GetLinearVelocity().y) < 0.01){
        playerSTRun = false;
    }
}

void GameScreen::ccTouchesBegan(CCSet* touches, CCEvent* event){
    CCSetIterator it;
//    CCTouch* touch;
//    for( it = touches->begin(); it != touches->end(); it++)
//    {
//        touch = (CCTouch*)(*it);
//
//        if(!touch)
//            break;
    CCTouch *touch = (CCTouch*)touches->anyObject();
    CCPoint location = this->getParent()->convertTouchToNodeSpace(touch);

//        location = touch->getLocationInView();
//        location = CCDirector::sharedDirector()->convertToGL(location);
    
        
        /************shoot************/
    
    CCRect rectSTPlayer=playerST->boundingBox();
    CCRect rectNDPlayer=playerND->boundingBox();
    
    CCRect rectST=shootButtonST->boundingBox();
    CCRect rectND=shootButtonND->boundingBox();
    CCRect touchRect = CCRectMake(location.x, location.y, 50, 50);
    
    if(touchRect.intersectsRect(rectST)){
        if(isBallST)
            goto tt;
        isTouchST = true;
        isShootST = true;
        timeTouchST = 0;
    }
tt:
    if(touchRect.intersectsRect(rectND)){
        if(isBallND)
//            return;
        isTouchND = true;
        isShootND = true;
        timeTouchND = 0;
    }

    /*************end shoot *******/
    if(location.x < CCDirector::sharedDirector()->getWinSize().width/2){
        if(!isShootST && !touchRect.intersectsRect(rectNDPlayer)){
            b2Vec2 vecto = b2Vec2();
            float s = location.x - playerST->getBody()->GetPosition().x * 32;
            float Vx = sqrt(fabs(s) * 0.01 * 10 * 2) * fabs(s)/s ;
            vecto.Set(Vx, 0);
            playerST->getBody()->SetLinearVelocity(vecto);
            return;
        }
    }
    else if(location.x > CCDirector::sharedDirector()->getWinSize().width/2){
        if(!isShootND && !touchRect.intersectsRect(rectSTPlayer)){
            b2Vec2 vecto = b2Vec2();
            float s = location.x - playerND->getBody()->GetPosition().x * 32;
            float Vx = sqrt(fabs(s) * 0.01 * 10 * 2) * fabs(s)/s ;
            vecto.Set(Vx, 0);
            playerND->getBody()->SetLinearVelocity(vecto);
            return;
        }
    }
}

void GameScreen::ccTouchesMoved(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent){
    CCTouch *touch = (CCTouch*)pTouches->anyObject();
    CCPoint location = this->getParent()->convertTouchToNodeSpace(touch);
    CCRect touchRect = CCRectMake(location.x, location.y, 50, 50);
    
    CCRect rectSTLeft= CCRectMake(playerST->getBody()->GetPosition().x * 32 - 50 , playerST->getBody()->GetPosition().y * 32 + 80, 60, 90);
    CCRect rectSTRight = CCRectMake(playerST->getBody()->GetPosition().x * 32 + 50 , playerST->getBody()->GetPosition().y * 32 + 80, 60, 90);
    CCRect rectNDLeft= CCRectMake(playerND->getBody()->GetPosition().x * 32 - 50 , playerND->getBody()->GetPosition().y * 32 + 80, 60, 90);
    CCRect rectNDRight = CCRectMake(playerND->getBody()->GetPosition().x * 32 + 50 , playerND->getBody()->GetPosition().y * 32 + 80, 60, 90);
    if(touchRect.intersectsRect(rectNDLeft)){
        playerND->getBody()->SetLinearVelocity(b2Vec2(-4, 5));
    }
    if(touchRect.intersectsRect(rectNDRight)){
        playerND->getBody()->SetLinearVelocity(b2Vec2(4, 5));
    }
    if(touchRect.intersectsRect(rectSTLeft)){
        playerST->getBody()->SetLinearVelocity(b2Vec2(-4, 5));
    }
    if(touchRect.intersectsRect(rectSTRight)){
        playerST->getBody()->SetLinearVelocity(b2Vec2(4, 5));
    }
}

void GameScreen::ccTouchesEnded(CCSet* touches, CCEvent* event){
    CCSetIterator it;
    CCTouch* touch;
       for( it = touches->begin(); it != touches->end(); it++)
    {
        touch = (CCTouch*)(*it);
        
        if(!touch)
            break;
        CCPoint location;
        location = touch->getLocationInView();
        location = CCDirector::sharedDirector()->convertToGL(location);
        if(location.x < CCDirector::sharedDirector()->getWinSize().width/2){
            //create Ball
            if(!isBallST && isShootST){
                Ball* ball = new Ball();
                ball->createBall(this->world, "Icon-Small.png", ccp(playerST->getBody()->GetPosition().x *32 + 50 , playerST->getBody()->GetPosition().y * 32 + 30));
                this->addChild(ball);
                isBallST = true;
                ball->setTag(222);
                arrBall->addObject(ball);                
                float X = 50;
                float Y = 50;
                float x = timeTouchST/4 * X/(sqrt(X * X + Y * Y));
                float y = timeTouchST/4 * Y/(sqrt(X * X + Y * Y));
                CCLOG("touches  * %f .. %f ", location.x, location.y);
                b2Vec2 ex = b2Vec2();
                ex.Set(x, y);
                if(isBallST == true && !isBallSTRun){
                    dynamic_cast<PhysicsSprite*>(this->getChildByTag(222))->getBody()->SetLinearVelocity(ex);
                    isBallSTRun = true;
                }
                timeTouchST = 0;
//                return;
            }
        }
        else if (location.x > CCDirector::sharedDirector()->getWinSize().width/2){
            //create
            if(!isBallND && isShootND){
                Ball* ball = new Ball();
                ball->createBall(this->world, "Icon-Small.png", ccp(playerND->getBody()->GetPosition().x *32 + 50 , playerND->getBody()->GetPosition().y * 32 + 30));
                this->addChild(ball);
                isBallND = true;
                ball->setTag(223);
                arrBall->addObject(ball);
                float X = -50;
                float Y = 50;
                float x = timeTouchND/4 * X/(sqrt(X * X + Y * Y));
                float y = timeTouchND/4 * Y/(sqrt(X * X + Y * Y));
                CCLOG("touches  * %f .. %f ", location.x, location.y);
                b2Vec2 ex = b2Vec2();
                ex.Set(x, y);
                if(isBallND == true && !isBallNDRun){
                    dynamic_cast<PhysicsSprite*>(this->getChildByTag(223))->getBody()->SetLinearVelocity(ex);
                    isBallNDRun = true;
                }
                timeTouchND = 0;
//                return;
            }
        }
    }
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

void GameScreen::impactHandler(){
    CCObject* obj;
    CCARRAY_FOREACH(arrBall, obj){
        CCObject* objFloor;
        Ball* ball = dynamic_cast<Ball*>(obj);
        CCARRAY_FOREACH(arrFloor, objFloor){
            RoadTransfer* rf = dynamic_cast<RoadTransfer*>(objFloor);
            CCRect rectRF = CCRect(rf->getBody()->GetPosition().x * 32, rf->getBody()->GetPosition().y * 32, rf->getContentSize().width, rf->getContentSize().height);
            CCRect rectBall = CCRectMake(ball->getBody()->GetPosition().x * 32, ball->getBody()->GetPosition().y * 32, 105, 105);
            if(rectRF.intersectsRect(rectBall)){
                if(ball->getTag() == 222){
                
                    world->DestroyBody(rf->getBody());
                    this->removeChild(rf);
                    world->DestroyBody(ball->getBody());
                    this->removeChild(ball);
                    isBallST = false;
                    arrBall->removeObject(ball);
                    arrFloor->removeObject(rf);
                    timeTouchST = 0;
                    isBallSTRun = false;
                    isShootST = false;
                    return;
                }
                if(ball->getTag() == 223){
                    
                    world->DestroyBody(rf->getBody());
                    this->removeChild(rf);
                    world->DestroyBody(ball->getBody());
                    this->removeChild(ball);
                    isBallND = false;
                    arrBall->removeObject(ball);
                    arrFloor->removeObject(rf);
                    timeTouchND = 0;
                    isBallNDRun = false;
                    isShootND = false;
                    return;
                }
            }
        }
        if(ball->getBody()->GetPosition().y * 32 < 50){
            if(ball->getTag() == 222){
                world->DestroyBody(ball->getBody());
                this->removeChild(ball);
                isBallST = false;
                arrBall->removeObject(ball);
                timeTouchST = 0;
                isBallSTRun = false;
                isShootST = false;
                return;
            }
            if(ball->getTag() == 223){
                world->DestroyBody(ball->getBody());
                this->removeChild(ball);
                isBallND = false;
                arrBall->removeObject(ball);
                timeTouchND = 0;
                isBallNDRun = false;
                isShootND = false;
                return;
            }
        }
    }
}

void GameScreen::flying(float dt)
{
    if (isFlying&&(speedX!=0||speedY!=0)){
        CCPoint position=ccp(plane->getPosition().x+speedX,plane->getPosition().y+speedY);
        
        CCSize size=CCDirector::sharedDirector()->getWinSize();
        CCRect rect=CCRectMake(0,0,size.width,size.height);
        
        if(rect.containsPoint(position) && !playerSTRun){
            playerSTRun = true;
//            plane->setPosition(position);
            b2Vec2 ex = b2Vec2();
            ex.Set(speedX * 5, speedY * 5);
//            playerST->getBody()->SetLinearVelocity(ex);
        }
    }  
}

float GameScreen::heronsformula(float x1,float y1,float x2,float y2,float x3,float y3)
{
    float a=sqrt(pow(x1-x2,2)+pow(y1-y2,2));
    float b=sqrt(pow(x2-x3,2)+pow(y2-y3,2));
    float c=sqrt(pow(x3-x1,2)+pow(y3-y1,2));
    float s=(a+b+c)/2;    
    return sqrt(s*(s-a)*(s-b)*(s-c));
}

bool GameScreen::triangleContainPoint(float x1,float y1,float x2,float y2,float x3,float y3,float px,float py)
{
    float s1=heronsformula(x1,y1,x2,y2,px,py);
    float s2=heronsformula(x2,y2,x3,y3,px,py);
    float s3=heronsformula(x3,y3,x1,y1,px,py);
    float s=heronsformula(x1,y1,x2,y2,x3,y3);
    return abs(s-(s1+s2+s3))<0.001f;
}
