
import socket, struct, os, time, threading
from enum import IntFlag

myName = "null";
myID = -1;

class Addresses(IntFlag):
    A_NULL = -1,
    A_BROCKER = 0,
    A_ALL = 1,
    A_USER = 100

class Types(IntFlag):
    M_INIT = 0,
    M_EXIT= 1,
    M_GETDATA = 2,
    M_NODATA = 3,
    M_TEXT = 4,
    M_CONFIRM = 5

class Message:
    def __init__(self, To, From, Type, str):
        self.To = To 
        self.From = From
        self.Type = Type
        self.Size = len(str)
        self.str = str
    
def SendData(s, msg):
    s.send(struct.pack('iiii', msg.To, msg.From, msg.Type, msg.Size))
    if msg.Size:
        str=msg.str.encode('cp866')
        l = len(str)
        s.send(struct.pack(f'{l}s', str))
    
def ReceiveData(s, msg):
    msg.To = struct.unpack('i', s.recv(4))[0]
    msg.From = struct.unpack('i', s.recv(4))[0]
    msg.Type = struct.unpack('i', s.recv(4))[0]
    msg.Size = struct.unpack('i', s.recv(4))[0]
    msg.str = struct.unpack(f'{msg.Size}s', s.recv(msg.Size))[0].decode('cp866')
    return msg

def SendMessage(s, To, From, Type, str = ''):
    msg = Message(To, From, Type, str)
    SendData(s, msg)

def timer():
    while True:
        time.sleep(10);
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect(('localhost', 1214))

        msg = Message(-1,-1,-1,"")
        SendMessage(s, Addresses.A_BROCKER, myID, Types.M_GETDATA)
        print("Send")
        ReceiveData(s, msg);
        print("Receive")
        if (msg.To == myID):
            print("Message from " + str(msg.From) + " : " + msg.str + os.linesep)
        s.close()


#Main здесь
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect(('localhost', 1214))
    print("Enter your name");
    myName = input();

    #SendMessage(Addresses.A_BROCKER,0,Types.M_INIT,name)
    SendMessage(s,Addresses.A_BROCKER,Addresses.A_USER,Types.M_INIT,myName)

    msg = Message(0,0,0,"")
    ReceiveData(s,msg)
    s.close()
    myID = msg.To
    print("Your name: ",myName, " Your ID: ", myID)

    timer = threading.Thread(target=timer, args=[])
    timer.start()

    while True:

        print("1: Send message")
        print("2: Sign out")

        cur = input();

        if(cur == "1"):

            print("Add id user. 100 - ALL")
            to = int(input());
            if(to == 100):
                to = Addresses.A_ALL
            print("What?")
            strin = input()
            s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            s.connect(('localhost', 1214))
            SendMessage(s,to,myID,Types.M_TEXT,strin)
            s.close()
        else: 
            if(cur == "0" ):
                s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                s.connect(('localhost', 1214))
                SendMessage(s,Addresses.A_BROCKER,Types.M_EXIT)
                s.close()
            else:
                print("Repeat, please")


