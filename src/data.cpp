/*!
-----------------------------------------------------------------------------
@file    data.cpp
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
@brief data manipulation class
-----------------------------------------------------------------------------
*/
#include "data.hpp"

/*!
 * @brief Constructor for MotorData class
 * @param[in] raw bits array from serial port.
 */
MotorData::MotorData(const DataBytes& data)
{
    this->set(data);

    return;
}

/*!
 * @brief set internal variables
 * @param[in] raw bits array from serial port.
 */
void MotorData::set(const DataBytes& data)
{
    if(data.device == 0x11) this->device = "L";
    if(data.device == 0x12) this->device = "R";
    this -> rear_current  = static_cast<int16_t>(be16toh(data.rear_current));
    this -> front_current = static_cast<int16_t>(be16toh(data.front_current));
    this -> rear_rpm  = static_cast<int16_t>(be16toh(data.rear_rpm));
    this -> front_rpm  = static_cast<int16_t>(be16toh(data.front_rpm));
    this -> battery_v  = static_cast<uint16_t>(be16toh(data.battery_v));
    this -> time  = static_cast<uint32_t>(be32toh(data.time));

    return;
}

/*!
 * @brief Generate raw bits array from private variables
 * @return Data bits array
 */
DataBytes MotorData::toByteArray() const
{
    DataBytes data;
    if(this->device == "L") data.device = 0x11;
    if(this->device == "R") data.device = 0x12;
    data.rear_current = static_cast<int>(htobe16(this->rear_current));
    data.front_current = static_cast<int>(htobe16(this->front_current));
    data.rear_rpm = static_cast<int>(htobe16(this->rear_rpm));
    data.front_rpm = static_cast<int>(htobe16(this->front_rpm));
    data.battery_v = static_cast<int>(htobe16(this->battery_v));
    data.time = static_cast<long>(htobe32(this->time));
    return data;
}
