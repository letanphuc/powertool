from DataMgr import DataMgr
from SensorMgr import SensorMgr
from DBMgr import DataBase
from time import sleep
from threading import Thread
from Queue import Queue
from SimpleWebSocketServer import SimpleWebSocketServer, WebSocket

running = True
recording = False
liveViewData = []
ans = ''
count  = 0
MAX_LIVE_VIEW_COUNT = 10

def test():
    global db, sensorMgr, dataMgr
    
    v = range(0,7)
    for _ in range(0,100):
        dataMgr.addARecord(v)
    
    for i in range(0, 7):
        sensorMgr.addASensor(i, 'name %i' %i, 'type %i' %i)
    
    sensorMgr.removeASensor(4);
    sensorMgr.removeASensor(8);
    
    
def getSensorValueThread(queue):
    # initializing
    db = DataBase('powertool.db')
    sensorMgr = SensorMgr(db)
    dataMgr = DataMgr(db)

    while True:
        (cmd, data) = queue.get()
        if cmd == 'addsensor':
            if len(data) == 3:
                sensorMgr.addASensor(int(data[0]), data[1], data[2])
                
        elif cmd == 'removesensor':
            if len(data) == 1:
                sensorMgr.removeASensor(int(data[0]))
                
        elif cmd == 'addrecord':
            if len(data) == 7:
                intData = []
                for d in range(0,7):
                    intData.append(int(data[d]))
                dataMgr.addARecord(intData)
                db.save()
                
        elif cmd == 'exit':
            break
        
        
    print 'Subthread end'
def collecData(queue):
    global running, recording, liveViewData, ans, count
    while running:
        v = [count] * 7
        count = (count + 1)%1000
        if (recording):
            queue.put(("addrecord", v))
        else:
            if (len(liveViewData) > MAX_LIVE_VIEW_COUNT):
                liveViewData = liveViewData[-MAX_LIVE_VIEW_COUNT:]
            liveViewData.append(v)
            
            # broadcast here
            ans = ''
            for v in liveViewData[-10:]:
                ans += '%d,%d,%d,%d,%d,%d,%d|' \
                    %(v[0], v[1], v[2], v[3], v[4], v[5], v[6])
        sleep(0.025)
            

class SimpleEcho(WebSocket):

    def handleMessage(self):
        self.sendMessage(self.data + ans)
        liveViewData = []

    def handleConnected(self):
        print self.address, 'connected'
        print ''

    def handleClose(self):
        print self.address, 'closed'
        print ''
server = SimpleWebSocketServer('localhost', 8000, SimpleEcho)
def handleLiveViewThread():
    global running, recording, liveViewData, ans, server
    server.serveforever()


if __name__ == '__main__':
    q = Queue()
    dbThrad = Thread(target=getSensorValueThread, args=(q,))
    dbThrad.start()
    collecThread = Thread(target=collecData, args=(q,))
    collecThread.start()
    
    liveViewThread = Thread(target=handleLiveViewThread)
    liveViewThread.start()
    
    while True:
        line = raw_input(">> ")
        arg = [x for x in line.split(' ') if x != '']
        if len(arg) > 0:
            q.put((arg[0], arg[1:]))
            if arg[0] == 'exit':
                running = False
                server.close()
                break 
            elif arg[0] == "startrecord":
                recording = True
            elif arg[0] == "stoprecord":
                recording = False

    print 'Main Thread End'
    
    

    