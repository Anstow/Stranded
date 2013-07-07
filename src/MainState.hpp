#ifndef MAINSTATE_H
#define	MAINSTATE_H

#include "Engine/System/State.hpp"
#include "Engine/Utility/Vector.hpp"
#include "Engine/Graphics/Image.hpp"
#include "Engine/System/EventHandler.hpp"

class Player;
class Room;

class MainState : public tank::State
{
public:
    //Images
    static tank::Image font;
    static tank::Image fontsmall;
    static tank::Image grass;
    static tank::Image sand;
    static tank::Image sandwater;
    static tank::Image water;
    static tank::Image player;
    static tank::Image smalltree;
    static tank::Image largetree;
    static tank::Image oceanrock;
    static tank::Image bamboo;
    static tank::Image palmtree;
    static tank::Image smallrock;
    static tank::Image largerock;

    void changeRoom(tank::Vectori coords);

    Room const* currentRoom();

	void initEventHandler();

	bool playerLeft(const tank::EventArgs& args);
	bool playerRight(const tank::EventArgs& args);
	bool playerUp(const tank::EventArgs& args);
	bool playerDown(const tank::EventArgs& args);
	/*
    virtual void handleEvents(sf::Keyboard::Key) override;
	*/
    virtual void update() override;
    virtual void draw() override;

    MainState();
    virtual ~MainState();
private:
    std::vector<Room*> rooms_;

    Room*   currentRoom_;
    Player* player_;
    bool    paused_;

    void loadRooms();
    void pause();

    MainState           (MainState const&);
    MainState& operator=(MainState const&);
};
#endif
