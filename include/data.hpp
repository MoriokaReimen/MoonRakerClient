/*!
-----------------------------------------------------------------------------
@file    data.hpp
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
#pragma once
#include <cstdint>
#include <string>
#include <portable_endian.h>
using std::string;

/*!
 * @struct DataBytes
 * @brief wrapper for data bytes array
*/
#if __BYTE_ORDER == __BIG_ENDIAN
#pragma pack(1)
struct DataBytes {
    const uint16_t header = 0x75AA; //! Header bytes
    uint8_t device = 0x00; //! Source device (0x11 = left, 0x12 = right)
    int16_t rear_current = 0; //! rear Current
    int16_t front_current = 0; //! front Current
    int16_t rear_rpm = 0; //! rear Motor RPM
    int16_t front_rpm = 0; //! front Motor RPM
    uint16_t battery_v = 0;        //! Battery Level milliVolts
    uint32_t time = 0;         //! Time stamp Byte
    const uint16_t footter = 0x75FF; //! Footer byte
} __attribute__((__packed__));
#pragma pack()
#elif __BYTE_ORDER == __LITTLE_ENDIAN
#pragma pack(1)
struct DataBytes {
    const uint16_t header = 0xAA75; //! Header bytes
    uint8_t device = 0x00; //! Source device (0x11 = left, 0x12 = right)
    int16_t rear_current = 0; //! rear Current
    int16_t front_current = 0; //! front Current
    int16_t rear_rpm = 0; //! rear Motor RPM
    int16_t front_rpm = 0; //! front Motor RPM
    uint16_t battery_v = 0;        //! Battery Level milliVolts
    uint32_t time = 0;         //! Time stamp Byte
    const uint16_t footter = 0xFF75; //! Footer byte
} __attribute__((__packed__));
#pragma pack()
#endif

/*!
 * @class MotorData
 * @brief wrapper class for data bytes array
*/
class MotorData
{
public:
    string device {"O"}; //! device id L/R or O
    int rear_current {0}; //! rear raw sensor output
    int front_current {0}; //! fron raw sensor output
    int rear_rpm {0};
    int front_rpm {0};
    int battery_v {0};
    long time {0};
    MotorData() = default;
    MotorData& operator=(const MotorData&) = default;
    MotorData(const DataBytes& data);
    void set(const DataBytes& data);
    DataBytes toByteArray() const;
};

