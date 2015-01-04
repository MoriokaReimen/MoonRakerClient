/*!
-----------------------------------------------------------------------------
@file    moonserver.cpp
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
#include "moonserver.hpp"
using std::string;

/*!
 * @brief rover main routine
*/
void MoonServer::doRoutine()
{
    auto start_time = std::chrono::high_resolution_clock::now();
    MotorCommand command;
    MotorData r_motor_data; //! wrapper class for right motor data
    MotorData l_motor_data; //! wrapper class for left motor data
    ZMQData zmq_data;
    while(true) {
        try {
            command = this->zmq_.getCommand();
            break;
        } catch(...) {}
    }
    this->motor_.sendCommand(command);
    while(true) {
        try {
            l_motor_data = this->motor_.getData();
            if(l_motor_data.device == "L") break;
        } catch(...) {}
    }
    while(true) {
        try {
            r_motor_data = this->motor_.getData();
            if(r_motor_data.device == "R") break;
        } catch(...) {}
    }
    auto finish_time = std::chrono::high_resolution_clock::now();
    if(std::chrono::duration_cast<std::chrono::milliseconds>(finish_time - start_time).count() > 50) return;
    this->zmq_.sendData(zmq_data);
    return;
}

/*!
 * @brief Constructor for MoonServer class
 * @param[in] address to publish data like ""
 * @param[in] address to subscribe command like ""
*/
MoonServer::MoonServer(const string& rover_address, const string& console_address)
    : zmq_(rover_address, console_address)
{}

/*!
 * @brief do routine
*/
void MoonServer::run()
{
    while(true) this->doRoutine();
}

/*!
 * @brief do routine for specified seconds
 * @param[in] time for activate rover
*/
void MoonServer::run(double seconds)
{
    auto start_time = std::chrono::high_resolution_clock::now();
    while(true) {
        this->doRoutine();
        auto finish_time = std::chrono::high_resolution_clock::now();
        if(std::chrono::duration_cast<std::chrono::seconds>(finish_time - start_time).count() > seconds) return;
    }
}
