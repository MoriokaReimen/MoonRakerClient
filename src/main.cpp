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
#include <ncurses.h>
using std::string;
using std::cout;
using std::endl;

int main ()
{
    const string command_address {
        ""
    };
    const string data_address {
        ""
    };
    MoonClient client(command_address, data_address);
    ZMQData data;
    MotorCommand command(0, 0);
    while(true)
    {
      auto input = getch();
      if(input == 'q') break;
      if(input == 'k') command.set(3000, 3000);
      if(input == 'k') command.set(0, 0);
      client.sendCommand(command);
      data = client.getData();
      cout << data.time << ",";
      cout << data.left_rear_rpm << ",";
      cout << data.left_front_rpm << ",";
      cout << data.right_rear_rpm << ",";
      cout << data.right_front_rpm << ",";
      cout << data.left_rear_current << ",";
      cout << data.left_front_current << ",";
      cout << data.right_rear_current << ",";
      cout << data.right_front_current << ",";
      cout << data.battery_v << endl;
    }

    return 0;
}
