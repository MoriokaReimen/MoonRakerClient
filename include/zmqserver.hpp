/*!
-----------------------------------------------------------------------------
@file    zmqserver.hpp
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
 ###    @@@@@               @date Dec. 29. 2014
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
@brief zmq manipulation class for moonraker server
-----------------------------------------------------------------------------
*/
#pragma once
#include <cstdint>
#include <cstdlib>
#include <zmqpp/zmqpp.hpp>
#include <string>

#include "command.hpp"
#include "data.hpp"
#include "zmqdata.hpp"
using std::string;

/*!
 * @class ZMQServer
 * @brief wrapper class for zmq manupulation
*/
class ZMQServer
{
    /*! private variables for sending data */
    zmqpp::context publish_context_; //! zmq socket context
    zmqpp::socket_type publish_type_ {zmqpp::socket_type::push}; //! zmq socket type
    zmqpp::socket publish_socket_; //! zmq socket

    /*! private variables for receiving command */
    zmqpp::context subscribe_context_; //! zmq socket context
    zmqpp::socket_type subscribe_type_ {zmqpp::socket_type::pull}; //! zmq socket type
    zmqpp::socket subscribe_socket_; //! zmq socket
public:
    ZMQServer(const string& data_address, const string& command_address);
    MotorCommand getCommand();
    void sendData(const ZMQData& data);
};
