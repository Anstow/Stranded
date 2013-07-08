#include "Room.hpp"

#include <sstream>
#include <fstream>
#include <string>
#include <algorithm>
#include <memory>
#include <stdexcept>
#include "Object.hpp"
#include "Player.hpp"
#include "LargeTreeObject.hpp"
#include "SmallTreeObject.hpp"
#include "PalmTreeObject.hpp"
#include "BambooObject.hpp"
#include "LargeRockObject.hpp"
#include "SmallRockObject.hpp"

//TODO Replace this
enum EntityType
{
    NULL_ENTITY = 0,
    ENT_TEXT_BIG = 1,
    ENT_TEXT_SMALL = 2,
    ENT_PLAYER = 3,
    ENT_TREE_SMALL = 10,
    ENT_TREE_LARGE = 11,
    ENT_TREE_PALM = 12,
    ENT_ROCK_SMALL = 13,
    ENT_ROCK_LARGE = 14,
    ENT_BOULDER = 15,
    ENT_OCEAN_ROCK = 16,
    ENT_HUT = 17,
    ENT_SHIPWRECK = 18,
    ENT_CRATE = 19,
    ENT_STONE_TABLE = 20,
    ENT_BAMBOO = 21,
    TILE_GRASS = 100,
    TILE_WOODLAND = 200,
    TILE_SAND = 300,
    TILE_SAND_WATER = 400,
    TILE_WATER = 500,
};

Room::Room(tank::Vectori coords)
{
    std::vector<std::unique_ptr<Tile>> tiles;
    std::vector<std::unique_ptr<tank::Entity>> entities;
    tank::Vectorf tilePos = {0,0};
    int tileID;
    int rotation;
    int objectID;

    std::stringstream ss;

    ss << "res/Room_" << coords.x << "-" << coords.y << ".roo";
    std::string path = ss.str();

    std::fstream roomFile(path.c_str());

    if(not roomFile)
    {
        roomFile.close();
        throw std::runtime_error("Room file missing");
    }

    float float1;

    tiles.reserve( ROOM_SIZE*ROOM_SIZE );

    //TODO this could probably be unique with obj.reset used each time
    Object* obj = NULL;

    for(int i = 0; i < ROOM_SIZE*ROOM_SIZE; ++i)
    {
        float1    = i + 1; //WHAT THE FUCK
        tilePos.x = i % ROOM_SIZE;
        tilePos.y = ceil(float1/ROOM_SIZE) - 1;

        roomFile >> tileID  ; roomFile.ignore(1,':');
        roomFile >> rotation; roomFile.ignore(1,'(');
        roomFile >> objectID; roomFile.ignore(1,')');

        if(roomFile.fail())
        {
            roomFile.close();
            throw std::runtime_error("Room file flawed");
        }
        tiles.emplace_back(new Tile(tilePos, tileID, rotation));

        tank::Vectorf objPos = { tilePos.x*Tile::TILE_SIZE,tilePos.y*Tile::TILE_SIZE };

        switch(objectID)
        {
            case NULL_ENTITY:
                obj = NULL;
                break;
            case ENT_BAMBOO:
                obj = new BambooObject( {} );
                break;
            case ENT_TREE_PALM:
                obj = new PalmTreeObject( {} );
                break;
            case ENT_TREE_SMALL:
                obj = new SmallTreeObject( {} );
                break;
            case ENT_TREE_LARGE:
                obj = new LargeTreeObject( {} );
                break;
            case ENT_ROCK_LARGE:
                obj = new LargeRockObject( {} );
                break;
            case ENT_ROCK_SMALL:
                obj = new SmallRockObject( {} );
                break;
            default:
                //obj = new Object( {}, "" );
                obj = nullptr;
                break;
        }

        if(obj)
        {
            obj->setPos(objPos);
            obj->setState(this);
            entities.emplace_back(obj);
        }
    }

    roomFile.close();

    for (auto& tile : tiles)
    {
        entities_.push_back(std::move(tile));
    }
    for (auto& ent : entities)
    {
        entities_.push_back(std::move(ent));
    }
}

Room::~Room() { }
