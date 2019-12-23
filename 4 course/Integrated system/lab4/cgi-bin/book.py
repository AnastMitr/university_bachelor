import cgi, pickle, cgitb, codecs, sys, datetime, os, json, requests

cgitb.enable()
sys.stdout.reconfigure(encoding='utf-8')

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

    def GetMessages(self):
        itm = {}
        for(k, i) in self.items.items():
            itm[k] = {'id':f'{i.id}', 'to':f'{i.to}', 'msg':f'{i.message}'}
        print(json.dumps(itm))
     

    def AddMessages(self):
        id = int(self.q.getvalue('id', 0))
        item = self.GetItem(id)
        item.SetData(self.q)

        if id == 0:
            self.maxid += 1
            item.id = self.maxid
            self.items[item.id] = item
        print(json.dumps({'res':'OK'}))

    def GetItem(self, id):
        if id == 0:
            return MessageItem() 
        else:
            return self.items[id] 

    def DeleteItem(self):
        id = int(self.q.getvalue('id', 0))
        del(self.items[id])
        print(json.dumps({'res':'OK'}))


class MessageItem:
    def __init__(self, to=0, fr=0, str=''):
        self.id = 0
        self.to = to
        self.fr = fr
        self.message = str
        self.size = len(str)
        self.time = datetime.datetime.now()

    def SetData(self, q):
        self.id = q.getvalue('id')
        self.to = int(q.getvalue('to'))
        self.message = q.getvalue('message')

       
def main():
    q = cgi.FieldStorage()
    message = Message(q)

    MENU = {
        'Getmsg': message.GetMessages,
        'add': message.AddMessages,
        'DeleteItem': message.DeleteItem,
    }
    try:
        print("Content-type: text/json\n")
        MENU[q.getvalue('type')]();

    except Exception as e:
        print("Exception");
        print(" ", e, '<br>');

    message.store();


main()
