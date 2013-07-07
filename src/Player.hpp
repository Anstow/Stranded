#ifndef PLAYER_H
#define	PLAYER_H

#include "Engine/Graphics/Animation.hpp"
#include "Engine/System/EventHandler.hpp"
#include "Object.hpp"

class MainState;

class Player : public Object
{
public:
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();
	/*
    void move(int rotation, bool moving);
	*/
    virtual void update() override;

    Player(tank::Vectorf pos, MainState*);
private:
    void handleCollisions();
    tank::Animation* anim_;
    unsigned int rotation_;
    tank::Vectori mapPos_;
    MainState* mState_;

    tank::Vectorf vel_;
    tank::Vectorf lastPos_;
    float speed_;
    void checkSides();
};
#endif
