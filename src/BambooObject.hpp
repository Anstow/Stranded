/*
 * File:   BambooObject.hpp
 * Author: jamie
 *
 * Created on 06 January 2012, 01:35
*/

#ifndef BAMBOOOBJECT_H
#define	BAMBOOOBJECT_H

#include "Engine/Graphics/Animation.hpp"
#include "Object.hpp"

class BambooObject : public Object
{
public:
    virtual void update();
    virtual void draw(tank::IRender*);

    BambooObject(tank::Vectorf const& pos);
private:
    tank::Animation anim_;
};

#endif	/* BAMBOOOBJECT_H */

