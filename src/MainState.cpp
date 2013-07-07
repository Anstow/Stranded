#include "MainState.hpp"

#include <algorithm>
#include <functional>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>

#include "Engine/System/Game.hpp"
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
	initEventHandler();
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

void MainState::initEventHandler()
{
	tank::Game::log << "Initialise the event handler" << std::endl;
	tank::EventHandler& evtHandler = getEventHandler();
	evtHandler.registerEvent("PlayerLeft", tank::EventType::button_held);

	evtHandler.registerEventListener("PlayerLeft", std::bind(&MainState::playerLeft, this, std::placeholders::_1));
	evtHandler.registerKey(sf::Keyboard::Key::A, "PlayerLeft");

	evtHandler.registerEvent("PlayerRight", tank::EventType::button_held);
	evtHandler.registerEventListener("PlayerRight", std::bind(&MainState::playerRight, this, std::placeholders::_1));
	evtHandler.registerKey(sf::Keyboard::Key::D, "PlayerRight");

	evtHandler.registerEvent("PlayerUp", tank::EventType::button_held);
	evtHandler.registerEventListener("PlayerUp", std::bind(&MainState::playerUp, this, std::placeholders::_1));
	evtHandler.registerKey(sf::Keyboard::Key::W, "PlayerUp");

	evtHandler.registerEvent("PlayerDown", tank::EventType::button_held);
	evtHandler.registerEventListener("PlayerDown", std::bind(&MainState::playerDown, this, std::placeholders::_1));
	evtHandler.registerKey(sf::Keyboard::Key::S, "PlayerDown");
}

bool MainState::playerLeft(const tank::EventArgs& args)
{
	player_->moveLeft();
	return false;
}

bool MainState::playerRight(const tank::EventArgs& args)
{
	player_->moveRight();
	return false;
}

bool MainState::playerUp(const tank::EventArgs& args)
{
	player_->moveUp();
	return false;
}

bool MainState::playerDown(const tank::EventArgs& args)
{
	player_->moveDown();
	return false;
}

/*
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
*/

void MainState::update()
{
    if(!paused_)
    {
        currentRoom_->update();
    }
	getEventHandler().callListeners();
}

void MainState::draw()
{
    currentRoom_->draw();
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
