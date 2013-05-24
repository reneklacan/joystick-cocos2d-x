
#include "Joystick.h" 

#define JOYSTICK_OFFSET_X 5.0f
#define JOYSTICK_OFFSET_Y 5.0f

#define JOYSTICK_RADIUS 64.0f

#define THUMB_RADIUS 26.0f

static CCPoint convertCoordinate(CCPoint point){
    return CCDirector::sharedDirector()->convertToGL(point);
}

static bool isPointInCircle(CCPoint point, CCPoint center, float radius){
    float dx = (point.x - center.x);
    float dy = (point.y - center.y);
    return (radius >= sqrt((dx*dx)+(dy*dy)));
}

bool Joystick::init()
{
    bool bRet = false;
    do 
    {
        kCenter=CCPoint(JOYSTICK_RADIUS + JOYSTICK_OFFSET_X,
                JOYSTICK_RADIUS + JOYSTICK_OFFSET_Y);
        //////////////////////////////////////////////////////////////////////////
        // super init first
        //////////////////////////////////////////////////////////////////////////

        CC_BREAK_IF(!CCLayer::init());

        //////////////////////////////////////////////////////////////////////////
        // add your codes below...
        //////////////////////////////////////////////////////////////////////////

		this->setTouchEnabled(true);
        velocity = CCPointZero;

        CCSprite *bg = CCSprite::create("joystick_background.png");
        bg->setPosition(kCenter);
        this->addChild(bg,0);

        thumb = CCSprite::create("joystick_thumb.png");
        thumb->setPosition(kCenter);
        this->addChild(thumb,1);

        bRet=true;

    } while(0);

    return bRet;
}

void Joystick::updateVelocity(CCPoint point)
{
    // calculate Angle and length
    float dx = point.x - kCenter.x;
    float dy = point.y - kCenter.y;

    float distance = sqrt(dx*dx + dy*dy);
    float angle = atan2(dy,dx); // in radians

    if (distance > JOYSTICK_RADIUS)
    {
        dx = cos(angle) * JOYSTICK_RADIUS;
        dy = sin(angle) * JOYSTICK_RADIUS;
    }

    velocity = CCPointMake(dx/JOYSTICK_RADIUS, dy/JOYSTICK_RADIUS);

    if(distance>THUMB_RADIUS)
    {
        point.x = kCenter.x + cos(angle) * THUMB_RADIUS;
        point.y = kCenter.y + sin(angle) * THUMB_RADIUS;
    }

    thumb->setPosition(point);
}

void Joystick::resetJoystick()
{
    this->updateVelocity(kCenter);
}

bool Joystick::handleLastTouch()
{
    bool wasPressed = isPressed;
    if (wasPressed)
    {
        this->resetJoystick();
        isPressed = false;
    }
    return (wasPressed ? true : false);
}

void Joystick::ccTouchesBegan( CCSet *pTouches, CCEvent *pEvent )
{
    CCTouch *touch = (CCTouch*)pTouches->anyObject();
    CCPoint point = touch->getLocationInView();
    point = convertCoordinate(point);

    if (isPointInCircle(point,kCenter,JOYSTICK_RADIUS))
    {
        isPressed = true;
        this->updateVelocity(point);
    }
}

void Joystick::ccTouchesMoved( CCSet *pTouches, CCEvent *pEvent )
{
    if (isPressed)
    {
        CCTouch *touch = (CCTouch*)pTouches->anyObject();
        CCPoint point = touch->getLocationInView();
        point = convertCoordinate(point);
        this->updateVelocity(point);
    }
}

void Joystick::ccTouchCancelled( CCTouch *pTouch, CCEvent *pEvent )
{
    this->handleLastTouch();
}

void Joystick::ccTouchesEnded( CCSet *pTouches, CCEvent *pEvent )
{
    this->handleLastTouch();
}
