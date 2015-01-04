/*!
-----------------------------------------------------------------------------
@file    zmqclient.cpp
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
#include "zmqclient.hpp"
using std::string;

/*!
 * @brief Constructor for ZMQClient class
 * @param[in] address to publish data like ""
 * @param[in] address to subscribe command like ""
*/
ZMQClient::ZMQClient(const string& data_address, const string& command_address) :
    publish_socket_(publish_context_, publish_type_), subscribe_socket_(subscribe_context_, subscribe_type_)
{
    /* Initialize socket for receiving data */
    this -> subscribe_socket_.connect(data_address);

    /* Initialize socket for publishing motor command */
    this -> publish_socket_.connect(command_address);

    return;
}

/*!
 * @brief publish command through zmq socket.
 * @param[in] motor command
*/
void ZMQClient::sendCommand(const MotorCommand& command)
{
    CommandBytes  bytes = command.toByteArray();
    zmqpp::message message;
    message.add_raw(&bytes, sizeof(bytes));
    this->publish_socket_.send(message);
    return;

}

/*!
 * @brief subscribe rover data through zmq socket
 * @return Motor command
*/
ZMQData ZMQClient::getData()
{
    ZMQBytes bytes;
    zmqpp::message message;
    this->subscribe_socket_.receive(message);
    memcpy(&bytes, message.raw_data(0), sizeof(bytes));
    return ZMQData(bytes);
}
