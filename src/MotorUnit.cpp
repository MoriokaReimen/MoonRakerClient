
/*
-----------------------------------------------------------------------------
Filename:    MotorUnit.cpp
-----------------------------------------------------------------------------
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
 ###    @@@@@                    Spring 2013
 /-\     @@
|   |      %%                      Authors:
 \-/##    %%%%%               Nathan John Britton
   #### %%%                 nathan@astro.mech.tohoku.ac.jp
     ###%%       *
      ##%%     *****
       #%%      ***
        %%     *   *
         %%
          %%%%%
           %%%
-----------------------------------------------------------------------------

-----------------------------------------------------------------------------
*/


#include "MotorUnit.hpp"


/*CONSTRUCTOR
MotorUnit:
   Outsources the socket initialization to the Subsystem parent class, and then
connects each of the sockets.
*/
MotorUnit::MotorUnit(zmq::context_t& context_arg, std::string robot_address,
                     std::string data_address, std::string inproc_address)
    :Subsystem(context_arg), motor_publisher(context, ZMQ_PUB)
{
    std::cout<< "Initializing Motor Unit" <<std::endl;



    //Set the queue to a length of 1
    uint64_t hwm_value = 10;
    this->data_subscription_socket.setsockopt(ZMQ_HWM, &hwm_value, sizeof(hwm_value));
//  Use this setting to drop excess data
//    uint64_t buf_value = 4000000;
//    this->data_subscription_socket.setsockopt(ZMQ_RCVBUF, &buf_value, sizeof(buf_value));

    //Connect and subscribe to the image stream
    this->data_subscription_socket.connect(data_address.c_str());
    this->data_subscription_socket.setsockopt(ZMQ_SUBSCRIBE, NULL, 0);
    //this->data_subscription_socket.setsockopt(ZMQ_SUBSCRIBE, "R", 1);
    //this->data_subscription_socket.setsockopt(ZMQ_SUBSCRIBE, "L", 1);

    //Connect the command sockets
    this->subsystem_socket.connect(robot_address.c_str());
    this->command_socket.connect(inproc_address.c_str());



    //The motor publisher socket is unique to the motorUnit object
    //(as opposed to the general subsystem sockets above)
    //Set the socket not to linger when it's closed, and bind it
    int linger_value = 0;
    this->motor_publisher.setsockopt(ZMQ_LINGER, &linger_value, sizeof(linger_value));
    //Bind to the inproc address used for motor data publishing
    this->motor_publisher.bind("inproc://power_data");


    return;
}


/*DESTRUCTOR
~MotorUnit:
    Clean up memory by deleting all objects in memory that were
assigned through a pointer.
*/
MotorUnit::~MotorUnit()
{

    return;
}

void MotorUnit::join()
{
    this->main_thread->join();
    delete this->main_thread;

    this->subsystem_socket.close();
    this->data_subscription_socket.close();
    this->command_socket.close();
    this->motor_publisher.close();

    return;
}

/*MOTOR THREAD ROUTINE
mainExecutionLoop:
   This is the function that will be executed by the thread associated with
each MotorUnit object. Any subsequent threads used by the motor object must be
created and managed here.
*/
void MotorUnit::mainExecutionLoop()
{

    std::cout<< "motor loop" <<std::endl;
    //Set a flag to control the loop of this thread
    this->loop = true;

    int number_of_sockets = 3;
    int wait_time = -1;
    //0: don't wait
    //-1: wait forever
    zmq::pollitem_t incoming_list[] = { { this->subsystem_socket,         0, ZMQ_POLLIN, 0 },
        { this->data_subscription_socket, 0, ZMQ_POLLIN, 0 },
        { this->command_socket,           0, ZMQ_POLLIN, 0 }
    };


    while(this->loop) {
        zmq::poll(incoming_list, number_of_sockets, wait_time);

        if(incoming_list[0].revents && ZMQ_POLLIN) {
            //this->subsystem_socket.recv(&this->incoming_message);
        }

        if(incoming_list[1].revents && ZMQ_POLLIN) {
            this->handleMotorData();
        }

        if(incoming_list[2].revents && ZMQ_POLLIN) {
            this->handleUICommand();
        }
    }
    std::cout << "Motor SHUTDOWN" << std::endl;
    return;
}


void MotorUnit::handleMotorMessage()
{
    return;
}


/*HARDWARE CONTROL
handleMotorData:
     This function is called by the Motor thread's main execution loop
 when a new incoming data message is detected.
*/
void MotorUnit::handleMotorData()
{

    //record the time
    gettimeofday(&this->recv_TS, NULL);



    /*
        //Receive the data
        this->data_subscription_socket.recv(&tag);

        int messages = 0;

        std::cout << "FIRST MESSAGE " << messages << ":" << tag.size() << std::endl;
        std::string tester(static_cast<char*>(tag.data()), tag.size());
        std::cout << tester << std::endl;

            messages++;


        bool more = 0;
        size_t option_size = sizeof(int64_t);
        //Check if there are more messages
        data_subscription_socket.getsockopt(ZMQ_RCVMORE, &more, &option_size);
        while(more)
        {
            std::cout << "Message " << messages << ":" << tag.size() << std::endl;
            messages++;
            //std::string tester(static_cast<char*>(tag.data()), tag.size());
            //std::cout << tester << std::endl;
            this->data_subscription_socket.recv(&tag);
            data_subscription_socket.getsockopt(ZMQ_RCVMORE, &more, &option_size);
        }
    */






    //std::string label(static_cast<char*>(tag.data()), tag.size());
    /*if (label=="B")
    {
        zmq::message_t power;
        this->data_subscription_socket.recv(&power);
        float voltage = *static_cast<float*>(power.data());
        //std::cout << "Battery voltage: " << voltage << "V" << std::endl;

        motor_publisher.send(tag, ZMQ_SNDMORE);
        motor_publisher.send(power);
    }*/


    //Prepare messages to be filled with motor data
    zmq::message_t tag;
    zmq::message_t timestamp;
    zmq::message_t controller_clock;
    zmq::message_t target;
    zmq::message_t back_rpm;
    zmq::message_t front_rpm;
    zmq::message_t back_torque;
    zmq::message_t front_torque;
    zmq::message_t back_power;
    zmq::message_t front_power;
    zmq::message_t battery;


    this->data_subscription_socket.recv(&tag);
    this->data_subscription_socket.recv(&timestamp);
    this->data_subscription_socket.recv(&controller_clock);
    this->data_subscription_socket.recv(&target);
    this->data_subscription_socket.recv(&back_rpm);
    this->data_subscription_socket.recv(&front_rpm);
    this->data_subscription_socket.recv(&back_torque);
    this->data_subscription_socket.recv(&front_torque);
    this->data_subscription_socket.recv(&back_power);
    this->data_subscription_socket.recv(&front_power);
    this->data_subscription_socket.recv(&battery);

    struct timeval rover_TS;
    unsigned long motor_clock;
    signed short target_speed, back_speed, front_speed;
    float back_motor_torque, front_motor_torque, back_motor_power, front_motor_power, battery_voltage;

    std::string motor_tag(static_cast<char*>(tag.data()), tag.size());
    rover_TS = *static_cast<struct timeval*>(timestamp.data());
    motor_clock = *static_cast<unsigned long*>(controller_clock.data());
    target_speed = *static_cast<signed short*>(target.data());
    back_speed = *static_cast<signed short*>(back_rpm.data());
    front_speed = *static_cast<signed short*>(front_rpm.data());
    back_motor_torque = *static_cast<float*>(back_torque.data());
    front_motor_torque = *static_cast<float*>(front_torque.data());
    back_motor_power = *static_cast<float*>(back_power.data());
    front_motor_power = *static_cast<float*>(front_power.data());
    battery_voltage = *static_cast<float*>(battery.data());


    motor_publisher.send(tag, ZMQ_SNDMORE);
    motor_publisher.send(back_torque, ZMQ_SNDMORE);
    motor_publisher.send(front_torque, ZMQ_SNDMORE);
    motor_publisher.send(back_power, ZMQ_SNDMORE);
    motor_publisher.send(front_power);



    if(this->record && this->data_file.is_open()) {
        this->data_file << motor_tag << ":"
                        << recv_TS.tv_sec << "," << recv_TS.tv_usec << ";"
                        << rover_TS.tv_sec << "," << rover_TS.tv_usec << ";"
                        << motor_clock << ";"
                        << target_speed << "," << back_speed << "," << front_speed << ";"
                        << back_motor_torque << ","<< front_motor_torque << ";"
                        << back_motor_power << ","<< front_motor_power << ";"
                        << battery_voltage << std::endl;
    }







    /*
        std::cout << motor_tag << " MOTOR @ " << rover_TS << ":" <<std::endl
                  << "target speed: " << target_speed << "; F: " <<front_speed << "; B: " << back_speed << std::endl
                  << "torque: " << motor_torque << "; power: " << motor_power << "W" << std::endl<< std::endl;

        int messages = 0;
        bool more = 0;
        size_t option_size = sizeof(int64_t);
        //Check if there are more messages
        data_subscription_socket.getsockopt(ZMQ_RCVMORE, &more, &option_size);
        while(more)
        {
            messages++;
        }
    */






    return;
}


/*COMMANDS FROM USER
handleUICommand:
    All commands expected from the Rover class should be handled here

    q: quit/shutdown
    r1,r2,r4: change resolution (1-low,2-mid,4-high)
    ...
*/
void MotorUnit::handleUICommand()
{

    //The zmq::poll function has detected an incoming message,
    //first it needs to be received.
    this->command_socket.recv(&this->command);

    std::string command_string(static_cast<char*>(this->command.data()), this->command.size());

    //Move the message into a char buffer
    //unsigned char message_buffer[this->command.size()];
    //memcpy(message_buffer, this->command.data(), this->command.size());

    //--------------------------PROCESS COMMANDS------------------------
    //QUIT
    if(command_string == "shutdown") {
        this->loop = false;
    } else if(command_string == "motor_speed") {
        //std::cout << "speed control" << std::endl;

        zmq::message_t speed_L, speed_R;
        this->command_socket.recv(&speed_L);
        this->command_socket.recv(&speed_R);


        int spd_L, spd_R;
        spd_L = *(int*)(speed_L.data());
        spd_R = *(int*)(speed_R.data());

        //std::cout << "L: " << spd_L << ", R: " << spd_R << std::endl;

        unsigned char data_packet[4];
        data_packet[0] = (spd_L >> 8) & 0xFF;
        data_packet[1] = spd_L & 0xFF;
        data_packet[2] = (spd_R >> 8) & 0xFF;
        data_packet[3] = spd_R & 0xFF;

        zmq::message_t mot_cmd(4);
        memcpy(mot_cmd.data(), data_packet, 4);
        this->subsystem_socket.send(mot_cmd);

    } else if(command_string == "") {

        std::cout << "..." << std::endl;
    } else if(command_string == "start_recording") {

        zmq::message_t foldername;
        this->command_socket.recv(&foldername);
        std::string filename(static_cast<char*>(foldername.data()), foldername.size());
        filename.append("/motor.txt");

        Subsystem::startRecording(filename);
    } else if(command_string == "stop_recording") {
        Subsystem::stopRecording();
    } else {
        std::cout << "Unrecognized Camera Command: " << command_string << std::endl;
    }

    return;
}
