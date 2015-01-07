/*!
-----------------------------------------------------------------------------
@file    moonclient.hpp
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
 ###    @@@@@               @date Jan. 7. 2014
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
@brief console side command/data manipulation class
-----------------------------------------------------------------------------
*/
#pragma once
#include <thread>
#include <mutex>
#include <string>
#include "zmqclient.hpp"
#include "motor.hpp"
#include "command.hpp"
using std::string;

/*!
 * @class MoonClient
 * @brief console side command/data manipulation class
*/
class MoonClient
{
  ZMQClient zmq_;
  MotorCommand command_;
  ZMQData data_;
  std::mutex command_mtx_;
  std::mutex data_mtx_;
  std::thread worker_;
  void thredFunc();
public:
  MoonClient(const string& data_address, const string& command_address);
  ~MoonClient();
  void sendCommand(const MotorCommand& command);
  ZMQData getData();
};
