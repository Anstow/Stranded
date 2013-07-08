#include "MainState.hpp"

#include <algorithm>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>
#include "Engine/System/Game.hpp"
#include "Engine/System/Entity.hpp"
#include "Engine/Graphics/BitmapText.hpp"
#include "Room.hpp"
#include "Player.hpp"
#include "Tile.hpp"

//TODO Put this in a global settings class or load from Tiled
#define MAP_WIDTH 4
#define MAP_HEIGHT 4

MainState::MainState()
   :currentRoom_(NULL),
    paused_(false)
{
    tank::Game::log << "Loading main state" << std::endl;

    /* Load graphics */
    font.load("res/Font.png");
    fontsmall.load("res/FontSmall.png");
    grass.load("res/GrassTiles.png");
    sand.load("res/SandTiles.png");
    sandwater.load("res/SandWaterTiles.png");
    water.load("res/WaterTiles.png");
    player.load("res/Player.png");
    smalltree.load("res/SmallTree.png");
    largetree.load("res/LargeTree.png");
    oceanrock.load("res/OceanRock.png");
    bamboo.load("res/Bamboo.png");
    palmtree.load("res/PalmTree.png");
    smallrock.load("res/SmallRock.png");
    largerock.load("res/LargeRock.png");

    tank::Game::log << "Loading World..." << std::endl;

    loadRooms();
    currentRoom_ = rooms_[0];
    const float playerPos = 8 * Tile::TILE_SIZE;
    player_ = currentRoom_->makeEntity<Player>(tank::Vectorf{playerPos, playerPos},
                                               this);

    makeEntity<tank::Entity>(tank::Vectorf{10.f, 10.f})->makeGraphic<tank::BitmapText>(font, tank::Vectoru{50, 50})->setText("HELLO");
}

MainState::~MainState()
{
    tank::Game::log << "Unloading World..." << std::endl;

    for (unsigned int i = 0; i < rooms_.size(); ++i)
    {
        delete(rooms_[i]);
    }
    rooms_.clear();

    entities_.clear();
}

void MainState::loadRooms()
{
    rooms_.reserve(MAP_WIDTH*MAP_HEIGHT);

    tank::Vectori coords({0,0});

    for(int i = 0; i < MAP_HEIGHT; i++)
    {
        coords.y = i;
        for(int j = 0; j < MAP_WIDTH; j++)
        {
            coords.x = j;
            //rooms_[i*MAP_WIDTH+j] = new Room(coords,player_);
            rooms_.push_back(new Room(coords));
        }
    }
}

void MainState::changeRoom(tank::Vectori coords)
{
    if(coords.x == MAP_WIDTH || coords.y == MAP_HEIGHT) return;

    Room* lastRoom = currentRoom_;
    currentRoom_ = rooms_[coords.y*MAP_WIDTH + coords.x];

    lastRoom->moveEntity(currentRoom_, player_);
}

Room const* MainState::currentRoom()
{
    return currentRoom_;
}

void MainState::pause()
{
    if(paused_)
    {
        paused_ = false;
    }
    else
    {
        paused_ = true;
    }
}

void MainState::handleEvents(sf::Keyboard::Key ke)
{
    bool pressed = sf::Keyboard::isKeyPressed(ke);
    if(pressed)
    {
        switch(ke)
        {
            case sf::Keyboard::W:
                player_->move(0, true);
                break;
            case sf::Keyboard::S:
                player_->move(2, true);
                break;
            case sf::Keyboard::A:
                player_->move(3, true);
                break;
            case sf::Keyboard::D:
                player_->move(1, true);
                break;
            case sf::Keyboard::P:
                pause();
                break;
            default:
                break;
        }
    }
    else
    {
        switch(ke)
        {
            case sf::Keyboard::W:
                player_->move(0, false);
                break;
            case sf::Keyboard::S:
                player_->move(2, false);
                break;
            case sf::Keyboard::A:
                player_->move(3, false);
                break;
            case sf::Keyboard::D:
                player_->move(1, false);
                break;
            default:
                break;
        }
    }
}

void MainState::update()
{
    if(!paused_)
    {
        currentRoom_->update();
    }
}

void MainState::draw()
{
    currentRoom_->draw();
    tank::State::draw();
}

tank::Image MainState::font;
tank::Image MainState::fontsmall;
tank::Image MainState::grass;
tank::Image MainState::sand;
tank::Image MainState::sandwater;
tank::Image MainState::water;
tank::Image MainState::player;
tank::Image MainState::smalltree;
tank::Image MainState::largetree;
tank::Image MainState::oceanrock;
tank::Image MainState::bamboo;
tank::Image MainState::palmtree;
tank::Image MainState::smallrock;
tank::Image MainState::largerock;
