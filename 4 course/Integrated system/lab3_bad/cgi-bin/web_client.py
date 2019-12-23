
import cgi, pickle, cgitb, codecs, sys, datetime, os
import socket, struct, os, time, threading

from enum import IntFlag

cgitb.enable()

sys.stdout = codecs.getwriter("utf-8")(sys.stdout.detach())

def LoadTpl(tplName):
    docrootname = 'PATH_TRANSLATED'
    with open(os.environ[docrootname]+'/tpl/'+tplName+'.tpl', 'rt') as f:
        return f.read().replace('{selfurl}', os.environ['SCRIPT_NAME'])


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
    M_SERVER = 7

def SendData(s, msg):
    s.send(struct.pack('iiii', msg.to, msg.fr, msg.type, msg.size))
    if msg.size:
        str=msg.message.encode('cp866')
        l = len(str)
        s.send(struct.pack(f'{l}s', str))
    
def ReceiveData(s, msg):
    msg.to,msg.fr,msg.type,msg.size = struct.unpack('iiii', s.recv(4*4))
    if(msg.size == 0):
        msg.message = s.recv(msg.size+1).decode('cp1251')[:msg.size]
    return msg.type

def SendMessage(s, To, From, Type, str = ''):
    msg = MessageItem(To, From, Type, str)
    print(msg.to, msg.id, msg.type)
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
        with open('WebClient\message.db', 'rb') as f:
            (self.maxid, self.items) = pickle.load(f)

    def PrintHeader(self):
        print(LoadTpl('header'))

    def PrintFooter(self):
        print(LoadTpl('footer'))
 
    def PrintMessage(self):
        for(k, i) in self.items.items():
            i.MessageShow()
        print(LoadTpl('Add'))    

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
    def __init__(self, q, to=0, fr=0, type = Types.M_TEXT, str=''):
        self.id = 0
        self.to = to
        self.message = str
        self.size = len(str)
        self.fr = fr
        self.time = datetime.datetime.now()
        self.type = type

    def SetData(self, q):
        self.id = q.getvalue('id')
        self.to = q.getvalue('to')
        self.message = q.getvalue('message')
        self.fr = q.getvalue('fr')
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect(('localhost', 1214))
        SendMessage(s,self.to,self.fr,self.type,self.message)
        s.close()
        
    def MessageShow(self):
        print(LoadTpl('messageitem').format(**self.__dict__))
        
    def FormShow(self):
        print(LoadTpl('formitem').format(**self.__dict__))
        
            
def timer():
    while True:
        time.sleep(10);
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect(('localhost', 1214))

        msg = MessageItem();
        SendMessage(s, Addresses.A_BROCKER, ID, Types.M_GETDATA)
        print("Send")
        ReceiveData(s, msg);
        print("Receive")
        if (msg.to == ID):
            print("Message from " + str(msg.fr) + " : " + msg.message + os.linesep)
        s.close()


def init():
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.connect(('localhost', 1214))
    msg = MessageItem()
    SendMessage(s, Addresses.A_BROCKER, 0, Types.M_INIT, "py_server")
    if ReceiveData(s, msg)==Types.M_INIT:
        global ID
        ID = msg.to
        print(ID)
        global flag
        flag = 1
    s.close()
    

#Main здесь
flag = 0
print(flag);
q = cgi.FieldStorage()

message = Message(q)
message.PrintHeader()

if (flag != 1):
    init()

#timer = threading.Thread(target=timer, args=[])
#timer.start()
#print(flag);
    
    
MENU = {
        'PrintForm':    message.PrintForm, #кнопка добавления и редактирования
        'ProcessForm':  message.ProcessForm, #кнопка сохранения
        'DeleteItem':   message.DeleteItem, #кнопка удаления
        'ShowForm': message.PrintForm
        }
 
try:    
    print(q.getvalue('type'));
    MENU[q.getvalue('type')]()
    #message.PrintMessage();

except Exception as e:
    print("^_^g");
    print(" ", e, '<br>')
    #message.PrintMessage()
    
#message.StoreData() #сохраняем то что добавили
message.PrintFooter()
