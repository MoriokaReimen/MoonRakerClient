/*!
-----------------------------------------------------------------------------
@file    motor.cpp
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
@brief motor serial communication manipulation class
-----------------------------------------------------------------------------
*/
#include "motor.hpp"
using std::runtime_error;
using bit8 = unsigned char;


/*!
 * @brief Constructor for MotorData class
 */
Motor::Motor()
{
    OpenComport(0, 115200);
}

/*!
 * @brief Destructor for MotorData class
 */
Motor::~Motor()
{
    CloseComport(0);
}

/*!
 * @brief send command to the motor
 * @param command[in] command for the motor
 */
int Motor::sendCommand(const MotorCommand& command) const
{
    CommandBytes bytes {command.toByteArray()};
    bit8 buffer[sizeof(bytes)];
    memcpy(buffer, &bytes, sizeof(bytes));
    return SendBuf(0, buffer, sizeof(bytes));
}

/*!
 * @brief get data from the motor
 * @return data as MotorData class
 * @exception runtime_error when no data
 * @exception runtime_error when broken data
 */
MotorData Motor::getData() const
{
    DataBytes bytes;
    bit8 buffer[40];
    int read_bytes = PollComport(0, buffer, 40);
    if (read_bytes == 0) {
        throw runtime_error("No data");//! There is no data in the buffer
    }
    //! Detect Headers and footers
    for (int i = 0; i < read_bytes; ++i) {
        if ((buffer[i] == 0x75) && (buffer[i + 1] == 0xAA) &&
            (buffer[i + 17] == 0x75) && (buffer[i + 18] == 0xFF)) {
            //! motor data array
            memcpy(&bytes, &buffer[i], sizeof(bytes));
            return MotorData(bytes);
        }
    }
    throw runtime_error("Broken data"); //! data is broken
}
