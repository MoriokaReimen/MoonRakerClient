/*!
-----------------------------------------------------------------------------
@file    main.cpp
----------------------------------------------------------------------------
         @@
       @@@@@@
      @```@@@@
     @`  `@@@@@@
   @@`   `@@@@@@@@
  @@`    `@@@@@@@@@           Tohoku University
  @` `   `@@@@@@@@@       SPACE ROBOTICS LABORATORY
  @`` ## `@@@@@@@@@    http://www.astro.mech.tohoku.ac.jp/
  @` #..#`@@@@@@@@@        Planetary Robotics Group
  @` #..#`@@@@@@@@@
  @` ### `@@@@@@@@@          Professor Kazuya Yoshida
  @` ###``@@@@@@@@@      Associate Professor Keiji Nagatani
   @### ``@@@@@@@@
   ###  ` @@@@@@@
  ###  @  @@@@@                 Creation Date:
 ###    @@@@@               @date Jan. 1. 2015
 /-\     @@
|   |      %%                      Authors:
 \-/##    %%%%%             @author Kei Nakata
   #### %%%                  menschenjager.mark.neun@gmail.com
     ###%%       *
      ##%%     *****
       #%%      ***
        %%     *   *
         %%
          %%%%%
           %%%
-----------------------------------------------------------------------------
@brief main function of rover side application
-----------------------------------------------------------------------------
*/
#include "moonclient.hpp"
#include "gamepad.h"
#include <ncurses.h>
using std::string;
using std::cout;
using std::endl;

int main ()
{
  int ch = 0;
  int line = 0;
  float lx, ly;
  float rx, ry;

	GamepadInit();

	initscr();
	cbreak();
	noecho();
	timeout(1);
  const string command_address {"tcp://192.168.11.99:5556"};
  const string data_address {"tcp://192.168.11.99:5555"};
  MoonClient client(command_address, data_address);
  ZMQData data;
  MotorCommand command(0, 0);
	while ((ch = getch()) != 'q') {
		GamepadUpdate();
    GamepadStickNormXY(GAMEPAD_0, STICK_LEFT, &lx, &ly);
    GamepadStickNormXY(GAMEPAD_0, STICK_RIGHT, &rx, &ry);
    command.set(3000 * ly, 3000 * ry);

    client.sendCommand(command);
    data = client.getData();
		move(0, 0);
		printw("press (q) to quit");
		move(1, 0);
		printw("R:%5f\tL:%5f", 3000 * ry, 3000 * ly);
    move(3, 0);
		printw("Left Front RPM\tRight Front RPM\tLeft Rear RPM\tRight Rear RPM");
    move(4 + line, 0);
		printw("%5f\t%5f\t%5f\t%5f",
        data.left_front_rpm, data.right_front_rpm,
        data.left_rear_rpm, data.right_rear_rpm);

    line++;
    if(line > 20) line = 0;
		refresh();
  }

	endwin();
  return 0;
}

