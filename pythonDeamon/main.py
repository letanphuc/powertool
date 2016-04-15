


from DataMgr import DataMgr
from SensorMgr import SensorMgr
from DBMgr import DataBase
from objc import nil

# initializing
db = DataBase('new2.db')
sensorMgr = SensorMgr(db)
dataMgr = DataMgr(db)

def test():
    global db, sensorMgr, dataMgr
    
    v = range(0,7)
    for _ in range(0,100):
        dataMgr.addARecord(v)
    
    for i in range(0, 7):
        sensorMgr.addASensor(i, 'name %i' %i, 'type %i' %i)
    
    sensorMgr.removeASensor(4);
    sensorMgr.removeASensor(8);

if __name__ == '__main__':
    test()
    print 'End'
    
    

    