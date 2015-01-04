/*!
-----------------------------------------------------------------------------
@file    zmqdata.cpp
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
 ###    @@@@@               @date Jan. 2. 2015
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
@brief published/subscribe data manipulation class
-----------------------------------------------------------------------------
*/
#include "zmqdata.hpp"

/*!
 * @brief Constructor for ZMQData class
 * @param[in] raw bits array from serial port.
 */
ZMQData::ZMQData(const ZMQBytes& data)
{
    this->set(data);
    return;
}

/*!
 * @brief set internal variables
 * @param[in] raw bits array from serial port.
 */
void ZMQData::set(const ZMQBytes& data)
{
    this -> time  = static_cast<double>(be64toh(data.time));
    this -> left_rear_current  = static_cast<int16_t>(be16toh(data.left_rear_current));
    this -> left_front_current = static_cast<int16_t>(be16toh(data.left_front_current));
    this -> right_rear_current  = static_cast<int16_t>(be16toh(data.right_rear_current));
    this -> right_front_current = static_cast<int16_t>(be16toh(data.right_front_current));
    this -> left_rear_rpm  = static_cast<int16_t>(be16toh(data.left_rear_rpm));
    this -> left_front_rpm  = static_cast<int16_t>(be16toh(data.left_front_rpm));
    this -> right_rear_rpm  = static_cast<int16_t>(be16toh(data.right_rear_rpm));
    this -> right_front_rpm  = static_cast<int16_t>(be16toh(data.right_front_rpm));
    this -> battery_v  = static_cast<uint16_t>(be16toh(data.battery_v));

    return;
}

/*!
 * @brief set internal variables
 * @param[in] left motor data
 * @param[in] right motor data
 */
void ZMQData::set(const MotorData& left, const MotorData& right)
{
    double now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    this -> time  = now;
    this -> left_rear_current  = left.front_current;
    this -> left_front_current = left.rear_current;
    this -> right_rear_current  = right.front_current;
    this -> right_front_current = right.rear_current;
    this -> left_rear_rpm  = left.front_rpm;
    this -> left_front_rpm = left.rear_rpm;
    this -> right_rear_rpm  = right.front_rpm;
    this -> right_front_rpm = right.rear_rpm;
    this -> battery_v  = (left.battery_v + right.battery_v) / 2;

    return;
}

/*!
 * @brief Generate raw bits array from private variables
 * @return Data bits array
 */
ZMQBytes ZMQData::toByteArray() const
{
    ZMQBytes data;
    data.time = static_cast<double>(htobe64(this->time));
    data.left_rear_current = static_cast<int>(htobe16(this->left_rear_current));
    data.left_front_current = static_cast<int>(htobe16(this->left_front_current));
    data.right_rear_current = static_cast<int>(htobe16(this->right_rear_current));
    data.right_front_current = static_cast<int>(htobe16(this->right_front_current));
    data.left_rear_rpm = static_cast<int>(htobe16(this->left_rear_rpm));
    data.left_front_rpm = static_cast<int>(htobe16(this->left_front_rpm));
    data.right_rear_rpm = static_cast<int>(htobe16(this->right_rear_rpm));
    data.right_front_rpm = static_cast<int>(htobe16(this->right_front_rpm));
    data.battery_v = static_cast<int>(htobe16(this->battery_v));
    return data;
}
