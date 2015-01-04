/*!
-----------------------------------------------------------------------------
@file    command.hpp
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
#pragma once
#include <stdexcept>
#include <cmath>
#include <portable_endian.h>

/*!
 * @struct CommandBytes
 * @brief wrapper for bytes array of command
*/
#if __BYTE_ORDER == __BIG_ENDIAN
#pragma pack(1)
struct CommandBytes {
    const uint16_t header = 0x75aa; //! Header byte
    const uint8_t device = 0x13; //! Source device 0x13 = PCBoard
    //(0x11 = left, 0x12 = right)
    const int16_t boost_flag = htole16(0x00);
    int16_t left_rpm = 0;        //! Left Motor RPM Byte
    int16_t right_rpm = 0;        //! Right Motor RPM Byte
    const uint16_t footer = 0x75ff; //! Footer byte
} __attribute__((__packed__));
#pragma pack()
#elif __BYTE_ORDER == __LITTLE_ENDIAN
#pragma pack(1)
struct CommandBytes {
    const uint16_t header = 0xaa75; //! Header byte
    const uint8_t device = 0x13; //! Source device 0x13 = PCBoard
    //(0x11 = left, 0x12 = right)
    const int16_t boost_flag = htole16(0x00);
    int16_t left_rpm = 0;        //! Left Motor RPM Byte
    int16_t right_rpm = 0;        //! Right Motor RPM Byte
    const uint16_t footer = 0xff75; //! Footer byte
} __attribute__((__packed__));
#pragma pack()
#endif

/*!
 * @class MotorCommand
 * @brief wrapper class for bytes array of command
*/
class MotorCommand
{
public:
    signed short left_rpm {0}; //! left motor rotation speed
    signed short right_rpm {0}; //! right motor rotation speed
    signed short max_rpm_ {4000}; //! max motor rotation speed
    MotorCommand() = default;
    MotorCommand& operator=(const MotorCommand& command) = default;

    MotorCommand(const signed short& left, const signed short& right);
    MotorCommand(const CommandBytes& command);
    void set(const signed short& left, const signed short& right);
    void set(const CommandBytes& command);
    CommandBytes toByteArray() const;
};
