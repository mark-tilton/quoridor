import os
import mysql.connector

cnx = mysql.connector.connect(host='192.168.1.4',
                              user='bot', 
                              password=str(os.environ.get('quoridor_bot_db_pw')),                              
                              database='quoridor_bot')

query = ("SELECT VERSION()")

cursor = cnx.cursor()
cursor.execute(query)

for (version) in cursor.fetchone():
    print("{}".format(version))

cursor.close()
cnx.close()