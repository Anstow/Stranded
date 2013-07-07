#include "Player.hpp"

#include "Engine/Graphics/Animation.hpp"
#include "MainState.hpp"
#include "Room.hpp"

Player::Player(tank::Vectorf pos, MainState* mState)
    : Object(pos)
    , rotation_(1)
    , mapPos_({0, 0})
    , mState_(mState)
{
    anim_ = makeGraphic<tank::Animation>(MainState::player,
                                        tank::Vector<unsigned int>{ 64, 64 });
    setHitbox({ 21, 58, 21, 5 });
    setType("player");
    speed_ = 5;

    anim_->add("up",    {  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10 }, 100);
    anim_->add("right", { 11, 13, 14, 15, 16, 17, 18, 19, 20 },         100);
    anim_->add("down",  { 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31 }, 100);
    anim_->add("left",  { 32, 33, 34, 35, 36, 37, 38, 39, 40, 41 },     100);
    anim_->select("up");
}

void Player::update()
{
    bool moving = false;
    if (vel_.x > 0)
    {
        rotation_ = 1;
        moving = true;
    }
    else if (vel_.x < 0)
    {
        rotation_ = 3;
        moving = true;
    }
    if (vel_.y > 0)
    {
        rotation_ = 2;
        moving = true;
    }
    else if (vel_.y < 0)
    {
        rotation_ = 0;
        moving = true;
    }
    if (moving)
    {
        switch( rotation_ )
        {
            case 0: anim_->select("up");    break;
            case 1: anim_->select("right"); break;
            case 2: anim_->select("down");  break;
            case 3: anim_->select("left");  break;
            default: break;
        }

        anim_->play();
    }
    else
    {
        anim_->stop();
    }

    lastPos_ = getPos();

    setPos(lastPos_ + vel_);
    setLayer(getHitbox().y + getPos().y);
	vel_ = {0,0};

    handleCollisions();
    checkSides();
}

void Player::moveLeft()
{
	vel_.x -= speed_;
}
void Player::moveRight()
{
	vel_.x += speed_;
}
void Player::moveUp()
{
	vel_.y -= speed_;
}
void Player::moveDown()
{
	vel_.y += speed_;
}

/* done differently now
void Player::move(int rotation, bool moving)
{
    if (moving)
    {
        switch(rotation)
        {
            case 0:
                vel_.y -= speed_;
                break;
            case 1:
                vel_.x += speed_;
                break;
            case 2:
                vel_.y += speed_;
                break;
            case 3:
                vel_.x -= speed_;
                break;
            default:
                break;
        }
    }
    else
    {
        switch(rotation)
        {
            case 0:
                vel_.y += speed_;
                break;
            case 1:
                vel_.x -= speed_;
                break;
            case 2:
                vel_.y -= speed_;
                break;
            case 3:
                vel_.x += speed_;
                break;
            default:
                break;
        }
    }
}
*/

//TODO Remove magic numbers
void Player::checkSides()
{
    tank::Vectorf pos = getPos();
    tank::Rectd const& hitbox = getHitbox();

    if (pos.x + hitbox.x < 0)
    {
        if (mapPos_.x > 0)
        {
            mapPos_.x = mapPos_.x - 1; //Move player to next room
            pos.x = 576 + 21;
            lastPos_.x = 700;

            mState_->changeRoom(mapPos_);
        }
        else
        {
            pos.x = lastPos_.x;
        }
    }
    else if (pos.x + hitbox.x + hitbox.w >= 640)
    {
        if (mapPos_.x < 3)
        {
            mapPos_.x = mapPos_.x + 1;//Move player to next room
            pos.x = 0 - 21;
            lastPos_.x = -100;

            mState_->changeRoom(mapPos_);
        }
        else
        {
            pos.x = lastPos_.x;
        }
    }
    else if (pos.y + hitbox.y < 0)
    {
        if (mapPos_.y > 0)
        {
            mapPos_.y = mapPos_.y - 1; //Move player to next room
            pos.y = 640 - 64;
            lastPos_.y = 700;

            mState_->changeRoom(mapPos_);
        }
        else
        {
            pos.y = lastPos_.y;
        }
    }
    else if (pos.y + hitbox.y + hitbox.h >= 640)
    {
        if (mapPos_.y < 3)
        {
            mapPos_.y = mapPos_.y + 1; //Move player to next room
            pos.y = 0 - 58;
            lastPos_.y = -100;

            mState_->changeRoom(mapPos_);
        }
        else
        {
            pos.y = lastPos_.y;
        }
    }

    setPos(pos);
}

void Player::handleCollisions()
{
    if (!collide("bamboo").empty())
    {
        //TODO something
    }

    std::vector<Entity*> collisionList = collide();

    for(auto ent : collisionList)
    {
        if (ent->isSolid())
        {
            setPos(lastPos_);

            anim_->stop();
        }
    }
}
