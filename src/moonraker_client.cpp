"""
@note Before executing this program SSH into the Rover at IP Below.
IP: 192.168.11.10
username: rover
password: tohoku
cd into Rover/bin
roverApp (runs the motor interface)
"""

struct MotorData
{
  double time_{0};
  double rpm_{0};
  double torque_{0};
  double id_{0};
};

class MoonRakerCommand
{
  bool checkCommand_()
  {
    if(abs(left_front_rpm) > max_rpm_) return false;
    if(abs(left_rear_rpm) > max_rpm_) return false;
    if(abs(right_front_rpm) > max_rpm_) return false;
    if(abs(right_rear_rpm) > max_rpm_) return false;
    return true;
  }
  friend class MoonRakerClient;
public:
  short left_front_RPM{0};
  short left_rear{0};
  short right_front_RPM{0};
  short right_rear{0};
  short max_rpm_{4000};
  bool toByteArray(char* bytes)
  {
    return true;
    return false;
  }
};

class MoonRakerClient
{
  string cmd_url_{"tcp://192.168.11.10:5555"};
  string data_url_{"tcp://192.168.11.10:5556"};
public:
  MoonRakerClient()
  {
    #Open a socket to send movement commands tot he rover-side app
    #This is a subscription socket, so it can only be read from
    motor_data_socket = context.socket(zmq.SUB)
    #Open a socket to send movement commands to the rover-side app
    #The command socket is a pair-type socket. In our case it is only
    #set up for 1-way communication to the rover.
    motor_cmd_socket = context.socket(zmq.PAIR)

    #The high water mark for the message queue is set
    #here. When the HWM is passed, messages are dumped
    #The lower level tcp buffer limit is set here in bytes
    motor_cmd_socket.setsockopt(zmq.SNDBUF, 4)
    motor_cmd_socket.connect(url_motor_cmd)
    print("Motor Control Initialized")
    #The high water mark for the message queue is set
    #here. When the HWM is passed, messages are dumped
    #The lower level tcp buffer limit is set here in bytes
    motor_data_socket.setsockopt(zmq.RCVBUF, 256)

    #The subscription socket can only be subscribed to after the connect
    motor_data_socket.connect(url_motor_data)
    motor_data_socket.setsockopt(zmq.SUBSCRIBE, b"")

    print("Motor Data Subscribed")

  }
  ~MoonRakerClient()
  {
    this->setRPM();
    motor_cmd_socket.close()
    motor_data_socket.close()
  }
  setRPM();
  {
    motor_cmd_socket.send(bytes(raw_data));
  }
  setRPM(const MoonRakerCommand& command);
  {
  '''Sends RPM Command to Rover. Arguments are signed shorts'''
  #Set up a bytearray for a 4-byte
  #data packet to send to the rover
  raw_data = bytearray()

  #The first 2 bytes are the left motors speeds
  for i in (8, 0):
          #Separate the signed short into 2 bytes and append in big endian order
          raw_data.append(rpm_L >> i & 0xff)
  #Rinse and repeat for the right side
  for i in (8, 0):
          raw_data.append(rpm_R >> i & 0xff)
    motor_cmd_socket.send(bytes(raw_data));
  }
  getRPM()
  {
    motor_message = motor_data_socket.recv()
  }
  bool log(Logger& logger)
  {
    thread(logger.log);
    return true;
  }
};

class Logger
{
  string filename;
public:
  MotorLogger(const string& filename);
  ~MotorLogger();
private:
  void log();
};
