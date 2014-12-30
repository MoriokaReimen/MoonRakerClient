"""
SSH into the Rover at IP Below.
username: rover
password: tohoku
cd into Rover/bin
roverApp (runs the motor interface)

Requires PyZMQ 2.2.x
"""


import zmq
import time
import os
import struct



'''----Define Parameters -----'''
#Max Allowable RPM Motor Command
max_rpm = 4000


#Create global context
context = zmq.Context()

'''--------------ADDRESSES-------------'''
target_IP = "192.168.11.10"
#target_IP = "127.0.0.1"

#Address to send commands to the motor handling thread on the rover-side app
url_motor_cmd = "tcp://" + target_IP + ":5555"

#Address of the motor data publisher of the motor thread on the rover-side app
url_motor_data = "tcp://" + target_IP + ":5556"
'''------------------------------------'''




'''-----------Processing Received Bytes----------'''
def processdata(motor_message):
        """Unpacks motor message into list"""
        return struct.unpack('>HhhBBLHhhBBLhh', motor_message)


'''-------------------------------'''



'''-------Load Command Script from File------------------------'''

def loadcommandscript (inputfile):

        #Loads Command Script from file
        print("\n*-----------*\nLoading Command Script")
        f = open(inputfile, "r")
        commandscriptlines = f.read().splitlines()
        f.close()
        print("Script Loaded \nVerifying Script")
        commandscript = []
        for line in commandscriptlines:
                commandscript.append(line.split(","))

        #Converts Command Script to Proper Formats and checks it is valid

        if commandscript[0] != ['1', '0', '0', '0']: #Checks initiation line
                print("The first line of the command script is not properly formatted \n Exectuing null script")
                return [[1,0.0,0,0],['END']]
        else:
                commandscript[0] = [1, 0.0, 0, 0]


        if commandscript[-1][0] != 'END': #Checks the last line
                print("The last line of the command script is not END \n Exectuing null script")
                return [[1,0.0,0,0],['END']]

        for i in range(1, len(commandscript)-1): # Checks other lines
                commandscript[i][0] = int(commandscript[i][0])
                if commandscript[i][0] != i+1: #Checks numbering
                        print("Command number " + str(i+1) + " is not properly labeled. Exectuing null script")
                        return [[1,0.0,0,0],['END']]
                commandscript[i][1] = float(commandscript[i][1])
                if commandscript[i][1] <= commandscript[i-1][1]: #Checks positive time increment
                        print("Command number " + str(i+1) + " is going backwards in time. Exectuing null script")
                        return [[1,0.0,0,0],['END']]
                commandscript[i][2] = int(commandscript[i][2])
                commandscript[i][3] = int(commandscript[i][3])
                if commandscript[i][2] > max_rpm or commandscript[i][3] > max_rpm:
                        print("Command number " + str(i+1) + " is a bit agressive. Exectuing null script")
                        return [[1,0.0,0,0],['END']]
        print("Script Verified\n*-----------*\n")
        return commandscript


'''-------------------------------'''


'''--------------Wrting Datalog to File-----------------'''
def outputdata(datalog, outputfilename):
        with open(outputfilename, 'w') as f:
                for packet in datalog:
                        line = ""
                        for elem in packet:
                                line = line + str(elem)
                                line = line + ","
                        line = line + "\n"
                        f.write(line)
        f.close()
        return None




'''-------------------------------'''

'''--------Executing a scenario-------'''

def run(inputfilename, outputfilename):

        script = loadcommandscript(inputfilename)

        print("\n*-----------*\nChecking Output File does not already exist.")
        if os.path.exists(outputfilename): #Checks for preexisting output file - don't wanna overwrite data already collected!
                print("Output file already exists. Please remove or change output target\n*-----------*")
                return None
        print("Output does not exist. File will be created\n*-----------*")

        print("\n*-----------*\nScenario is ready to execute.\nEnter y to continue or anything else to abort.")
        print(script)
        cont_input = input()

        if cont_input == 'y':



                print("Executing Scenario\n*-----------*")
                '''------------SOCKETS Setup------------'''

                #Open a socket to send movement commands to the rover-side app
                #The command socket is a pair-type socket. In our case it is only
                #set up for 1-way communication to the rover.
                motor_cmd_socket = context.socket(zmq.PAIR)
                #The high water mark for the message queue is set
                #here. When the HWM is passed, messages are dumped
                #motor_cmd_socket.setsockopt(zmq.HWM, 1)                        #Commented HWM out as my ZMQ Installation was broken - might work for you.
                #The lower level tcp buffer limit is set here in bytes
                motor_cmd_socket.setsockopt(zmq.SNDBUF, 4)
                motor_cmd_socket.connect(url_motor_cmd)
                print("Motor Control Initialized")




                #Open a socket to send movement commands tot he rover-side app
                #This is a subscription socket, so it can only be read from
                motor_data_socket = context.socket(zmq.SUB)

                #The high water mark for the message queue is set
                #here. When the HWM is passed, messages are dumped
                #motor_data_socket.setsockopt(zmq.HWM, 0)                        #Commented HWM out as my ZMQ Installation was broken - might work for you.
                #The lower level tcp buffer limit is set here in bytes
                motor_data_socket.setsockopt(zmq.RCVBUF, 256)

                #The subscription socket can only be subscribed to after the connect
                motor_data_socket.connect(url_motor_data)
                motor_data_socket.setsockopt(zmq.SUBSCRIBE, b"")

                print("Motor Data Subscribed")

                '''-----------Receiving Data--------------'''
                def datareceive():
                        #use 'zmq.NOBLOCK as an argument if you
                        #don't want the function to wait for a message
                        try:
                                motor_message = motor_data_socket.recv()
                        except zmq.core.error.ZMQError as error:
                                if str('Resource temporarily unavailable') == str(error):
                #                        print("Warning: Socket not ready to receive")
                                        return None

                                else:
                                      raise



                #        print("Motor Message Received")

                #        data = processdata(motor_message)
                #       return data
                        return motor_message

                '''-------------------------------'''


                '''----------Sending Commands------------'''

                def sendrpmcmd(rpm_L, rpm_R):
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

                        #Send the commands to the rover
                        try:
                                motor_cmd_socket.send(bytes(raw_data))
                        except zmq.core.error.ZMQError as error:
                                if str('Resource temporarily unavailable') == str(error):
                                        print("Warning: Failure to send command")
                                        return None

                                else:
                                      raise

                        return 0
                '''-------------------------------'''

                currentcommand = script[0]
                nextcommand = script[1]
                rawdatalog = []
                datalog = []
                start = time.clock()
                elapsed = 0.0
                print("\n*-----------*\nStarting Scenario Timer\n")

                while nextcommand[0] != 'END':
                        print("Sending Command " + str(nextcommand[0]))
                        sendrpmcmd(currentcommand[2],currentcommand[3])
                        print(str(time.clock() - start) + "s - Sent Command " + str(currentcommand[0]) + " RPM L: " + str(currentcommand[2]) + " RPM R: " + str(currentcommand[3]) + "\n")
                        while elapsed < nextcommand[1]:
                                tmp = []
                                tmp2 = datareceive() #Get packet from ZMQ if possible
                                if tmp2 != None: #Check it recieved a packet
                                        tmp.append(elapsed) #Add Packet Recieve time to tmp list
                                        tmp.append(tmp2) #Add Actual data packet to tmp list
                                        rawdatalog.append(tmp) #Appends packet to raw data log.
                                elapsed = time.clock() - start
                        currentcommand = nextcommand
                        nextcommand = script[currentcommand[0]]

                sendrpmcmd(0,0)
                print("Script Complete\n*-----------*\n")

                motor_cmd_socket.close()
                motor_data_socket.close()

                print("ZMQ Sockets Closed")


                print("\n*-----------*\nPost Processing Data\n")
                print( str(len(rawdatalog)) + " data packets logged")
                                
                for packet in rawdatalog:
                        datalog.append([packet[0]] + list(processdata(packet[1])) )
 
                                        
                print("Post Processing Compete\n*-----------*\n")

                print("Writing Data to file")
                outputdata(datalog, outputfilename)
                print("Data written to file")

                return 0
        else:
                print("Aborting Excution\n*-----------*")
                return None

'''-------------------------------'''


