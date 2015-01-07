/*!
-----------------------------------------------------------------------------
@file    moonclient.cpp
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
@brief console side command/data manipulation class
-----------------------------------------------------------------------------
*/
#include "moonclient.hpp"
using std::string;

/*!
 * @brief Constructor of MoonClient class
 * @param[in] address of rover data socket
 * @param[in] address of rover command socket
*/
MoonClient::MoonClient(const string& data_address, const string& command_address) :
  zmq_(data_address, command_address), worker_(&MoonClient::thredFunc, this)
{
}


/*!
 * @brief Destructor of MoonClient class
*/
MoonClient::~MoonClient()
{
  if(this->worker_.joinable()) this->worker_.join();
}

/*!
 * @brief routine done by other thread
*/
void MoonClient::thredFunc()
{
  MotorCommand old_command;
  ZMQData data;

  while(true)
  {
    /* send Command */
    {
      std::unique_lock<std::mutex> command_lock(this->command_mtx_, std::defer_lock);
      if(command_lock.try_lock())
      {
        old_command = this-> command_;
        this->zmq_.sendCommand(this->command_);
      }
      else this->zmq_.sendCommand(old_command);
    }
    /* getData Command */
    {
      std::unique_lock<std::mutex> data_lock(this->command_mtx_);
      this->data_ = zmq_.getData();
    }
  }
  return;
}

/*!
 * @brief send command to rover
 * @param[in] command
 * @return Data from rover
*/
void MoonClient::sendCommand(const MotorCommand& command)
{
  std::unique_lock<std::mutex> lock(this->command_mtx_);
  this->command_ = command;
  return;
}

/*!
 * @brief get Data from rover
 * @return Data from rover
*/
ZMQData MoonClient::getData()
{
  static ZMQData old_data;
  ZMQData data;
  std::unique_lock<std::mutex> lock(this->data_mtx_, std::defer_lock);
  if(lock.try_lock())
  {
    old_data = this-> data_;
    data = this-> data_;
  }
  else
  {
    data = old_data;
  }

  return data;
}
