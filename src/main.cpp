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
      client.sendCommand(command);
      data = client.getData();
      cout << data.time << "," << endl;
    }

    return 0;
}
