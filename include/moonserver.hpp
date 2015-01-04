/*!
-----------------------------------------------------------------------------
@file    moonserver.hpp
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
 ###    @@@@@               @date Jan. 3. 2014
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
@brief moonraker server main class
-----------------------------------------------------------------------------
*/
#pragma once
#include <chrono>
#include "motor.hpp"
#include "zmqserver.hpp"
using std::string;

/*!
 * @class MoonServer
 * @brief handle rover main routine
*/
class MoonServer
{
    ZMQServer zmq_; //! zmq handling class
    Motor motor_; //! serial communication for motor handling class
    void doRoutine();
public:
    MoonServer(const string& rover_address, const string& console_address);
    void run();
    void run(double seconds);
};
