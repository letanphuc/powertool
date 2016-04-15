

import DBMgr
from DBMgr import DataBase


class SensorMgr:
    def __init__(self, db):
        self.db = db
        self.db.execute("DROP TABLE IF EXISTS detail")
        self.db.execute("CREATE TABLE IF NOT EXISTS detail (sensorID integer, sensorName text, sensorType text)")
        self.db.save()
        pass
    
    def removeASensor(self, id):
        self.db.execute('DELETE FROM detail WHERE sensorID=%d' %id)
        self.db.save()
        
    def addASensor(self, id, name, type):
        self.db.execute("INSERT INTO detail VALUES (%i, '%s', '%s')" %(id, name, type))
        self.db.save()