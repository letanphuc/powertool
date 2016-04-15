

import sqlite3

class DataBase:
    def __init__(self, fileName):
        self.fileName = fileName
        self.conn = sqlite3.connect(self.fileName)
        self.cursor = self.conn.cursor()
        pass
    
    def close(self):
        self.conn.close()
        
    def execute(self, cmd):
        self.cursor.execute(cmd)
        pass

    def save(self):
        self.conn.commit()
        pass