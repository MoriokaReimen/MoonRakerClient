/*!
-----------------------------------------------------------------------------
@file    command.cpp
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
@brief command manipulation class
-----------------------------------------------------------------------------
*/
#include "command.hpp"
using std::abs;

/*!
 * @brief Constructor for MotorCommand class
 * @param[in] left left motr rotation velocity in rpm
 * @param[in] right left motr rotation velocity in rpm
 */
MotorCommand::MotorCommand(const signed short& left, const signed short& right)
{
    if(abs(left_rpm) > max_rpm_ || abs(right_rpm) > max_rpm_)
        throw std::range_error("rpm is over max rpm");

    this->left_rpm  = left;
    this->right_rpm = right;
    return;
}

/*!
 * @brief Constructor for MotorCommand class
 * @param[in] bytes array of command
 */
MotorCommand::MotorCommand(const CommandBytes& command)
{
    this->set(command);
    return;
}

/*!
 * @brief set internal variables
 * @param[in] left left motr rotation velocity in rpm
 * @param[in] right left motr rotation velocity in rpm
 */
void MotorCommand::set(const signed short& left, const signed short& right)
{
    if(abs(left_rpm) > max_rpm_ || abs(right_rpm) > max_rpm_)
        throw std::range_error("rpm is over max rpm");

    this->left_rpm  = left;
    this->right_rpm = right;
    return;
}

/*!
 * @brief set internal variables
 * @param[in] bytes array of command
 */
void MotorCommand::set(const CommandBytes& command)
{
    signed short left = static_cast<int16_t>(be16toh(command.left_rpm));
    signed short right = static_cast<int16_t>(be16toh(command.right_rpm));
    this->set(left, right);
    return;
}

/*!
 * @brief generate Bytes array of command
 * @return bytes array of command
 */
CommandBytes MotorCommand::toByteArray() const
{
    CommandBytes command;
    command.left_rpm = htobe16(this->left_rpm);
    command.right_rpm = htobe16(this->right_rpm);
    return command;
}
