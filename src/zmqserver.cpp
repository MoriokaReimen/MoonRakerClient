/*!
-----------------------------------------------------------------------------
@file    zmqserver.cpp
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
 ###    @@@@@               @date Jan. 2. 2014
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
@brief published/subscribe contents manipulation class
-----------------------------------------------------------------------------
*/
#include "zmqserver.hpp"
using std::string;

/*!
 * @brief Constructor for ZMQServer class
 * @param[in] address to publish data like ""
 * @param[in] address to subscribe command like ""
*/
ZMQServer::ZMQServer(const string& data_address, const string& command_address) :
    publish_socket_(publish_context_, publish_type_), subscribe_socket_(subscribe_context_, subscribe_type_)
{
    /* Initialize socket for receiving command */
    this -> subscribe_socket_.bind(command_address);

    /* Initialize socket for publishing motor data */
    this -> publish_socket_.bind(data_address);

    return;
}

/*!
 * @brief subscribe command through zmq socket.
 * @return Motor command
*/
MotorCommand ZMQServer::getCommand()
{
    CommandBytes  bytes;
    zmqpp::message message;
    this->subscribe_socket_.receive(message);
    memcpy(&bytes, message.raw_data(0), sizeof(bytes));
    return MotorCommand(bytes);
}

/*!
 * @brief publish Motor data through zmq socket
 * @param[in] motor data
*/
void ZMQServer::sendData(const ZMQData& data)
{
    ZMQBytes bytes = data.toByteArray();
    zmqpp::message message;
    message.add_raw(&bytes, sizeof(bytes));
    this->publish_socket_.send(message);
    return;
}
