import cgi, pickle, cgitb, codecs, sys, datetime, os
import socket, struct, os, time, threading

from enum import IntFlag
import http.cookies


cgitb.enable()
sys.stdout = codecs.getwriter("utf-8")(sys.stdout.detach())

q = cgi.FieldStorage()
ID = 0;

def getID():
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect(('localhost', 1214))
    msg = MessageItem()
    SendMessage(s, Addresses.A_BROCKER, Addresses.A_SERVER, Types.M_GET_ID)
    if(ReceiveData(s, msg) == Types.M_CONFIRM):
        global ID;
        ID = msg.fr;
        print("ID was geted   ID = " + str(ID))
    s.close()

def init():
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect(('localhost', 1214))
    msg = MessageItem()
    SendMessage(s, Addresses.A_BROCKER, Addresses.A_SERVER, Types.M_INIT, "py_server")
    if ReceiveData(s, msg)==Types.M_INIT:
        global ID
        ID = msg.to
        print(ID)
    s.close()

def check_NewMessage():
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect(('localhost', 1214))

    msg = MessageItem()
    SendMessage(s, Addresses.A_BROCKER, ID, Types.M_GETDATA)
    if (ReceiveData(s, msg) == Types.M_TEXT):
        print("NEW MESSAGE!\n")
        print("  FROM: " + str(msg.fr))
        print("   Message:   " + msg.message)
    s.close()

class Addresses(IntFlag):
    A_BROCKER = 0,
    A_ALL = 1,
    A_USER = 100,
    A_SERVER = 2

class Types(IntFlag):
    M_INIT = 0,
    M_EXIT= 1,
    M_GETDATA = 2,
    M_NODATA = 3,
    M_TEXT = 4,
    M_CONFIRM = 5,
    M_DELETE = 6,
    M_SERVER = 7,
    M_GET_ID = 8

def SendData(s, msg):
    s.send(struct.pack('iiii', msg.to, msg.fr, msg.type, msg.size))
    if msg.size:
        str=msg.message.encode('cp866')
        l = len(str)
        s.send(struct.pack(f'{l}s', str))

    
def ReceiveData(s, msg):
    msg.to = struct.unpack('i', s.recv(4))[0]
    msg.fr = struct.unpack('i', s.recv(4))[0]
    msg.type = struct.unpack('i', s.recv(4))[0]
    msg.size = struct.unpack('i', s.recv(4))[0]
    msg.message = struct.unpack(f'{msg.size}s', s.recv(msg.size))[0].decode('cp866')
    return msg.type

def SendMessage(s, To, From, Type, str = ''):
    msg = MessageItem(To, From, Type, str)
    SendData(s, msg)

class Message:
    def __init__(self, q):
        self.q = q
        try:
            self.LoadData()
        except:
            self.maxid = 0 
            self.items = {} 

    def LoadData(self):
        with open('message.db', 'rb') as f:
            (self.maxid, self.items) = pickle.load(f)


    def store(self):
        with open('message.db', 'wb') as f:
            pickle.dump((self.maxid, self.items), f)

    def PrintHeader(self):
        print(LoadTpl('header'))

    def PrintFooter(self):
        print(LoadTpl('footer'))
 
    def PrintMessage(self):
        for(k, i) in self.items.items():
            i.MessageShow()
        print(LoadTpl('add'))    

    def PrintForm(self):
        id = int(self.q.getvalue('id', 0))
        self.GetItem(id).FormShow()

    def ProcessForm(self):
        id = int(self.q.getvalue('id', 0))
        item = self.GetItem(id)
        item.SetData(self.q)

        if id == 0:
            self.maxid += 1
            item.id = self.maxid
            self.items[item.id] = item

    def GetItem(self, id):
        if id == 0:
            return MessageItem() 
        else:
            return self.items[id] 

    def DeleteItem(self):
        id = int(self.q.getvalue('id', 0))
        del(self.items[id])
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect(('localhost', 1214))
        SendMessage(s, ID, 10, Types.M_DELETE, str(id))
        print(str(id))
        s.close()

class MessageItem:
    def __init__(self, to=0, fr=0, type = Types.M_TEXT, str=''):
        self.id = 0
        self.to = to
        self.message = str
        self.size = len(str)
        self.fr = fr
        self.time = datetime.datetime.now()
        self.type = type

    def SetData(self, q):
        self.id = q.getvalue('id')
        self.to = int(q.getvalue('to'))
        self.message = q.getvalue('message')
        getID();
        self.fr = ID;
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect(('localhost', 1214))
        print("connect")
        SendMessage(s,self.to,self.fr, self.type,self.message)
        s.close()
        
    def MessageShow(self):
        print(LoadTpl('messageitem').format(**self.__dict__))
        
    def FormShow(self):
        print(LoadTpl('formitem').format(**self.__dict__))


def LoadTpl(tplName):
    docrootname = 'PATH_TRANSLATED'
    with open(os.environ[docrootname]+'/tpl/'+tplName+'.tpl', 'rt') as f:
        return f.read().replace('{selfurl}', os.environ['SCRIPT_NAME'])

def Show():
    print(LoadTpl('formitem'))

    
message = Message(q)
message.PrintHeader()


getID();
if ID == 0:
    init()
#else:
    #message.LoadData();
   

MENU = {
        'ShowForm': message.PrintForm,
        'add': message.ProcessForm,
        'PrintForm': message.PrintForm,
        'DeleteItem': message.DeleteItem,
    }
try:    
    MENU[q.getvalue('type')]();

except Exception as e:
    print("Exception");
    print(" ", e, '<br>');

message.PrintMessage();
message.store();
check_NewMessage()



