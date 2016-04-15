import DBMgr
from DBMgr import DataBase
from time import gmtime, strftime

class DataMgr:
    def __init__(self, db):
        self.db = db
        self.db.execute("DROP TABLE IF EXISTS data")
        self.db.execute("CREATE TABLE IF NOT EXISTS data (recordID integer, recordTime text, value0 integer,\
            value1 integer, value2 integer, value3 integer, value4 integer, value5 integer, value6 integer)")
        self.db.save()
        self.id = 0
        pass
    
        
    def addARecord(self, value):
        if len(value) < 7:
            print 'Error len of value must be 7'
        else:
            time = "" + strftime("%Y-%m-%d %H:%M:%S", gmtime())
            cmd = "INSERT INTO data VALUES (%d, '%s'" %(self.id, time)
            for v in value:
                cmd += ", %d" %v
            cmd += ")"
            self.db.execute(cmd)
            self.id += 1