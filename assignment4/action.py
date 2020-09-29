import sys
import sqlite3
from printdb import printdb


filename = "{}".format(sys.argv[1])
try:
    conn = sqlite3.connect('moncafe.db')
except Exception:
    print('There is no database')
    sys.exit()

c = conn.cursor()

with open(filename, "r") as config:
    for line in config:
        line = line.strip()
        line = [item.strip() for item in line.split(',')]
        product_id = line[0]
        action = int(line[1])
        activator = line[2]
        date = line[3]
        c.execute('SELECT quantity FROM Products WHERE id=?', (product_id,))
        current_quantity = c.fetchone()[0]
        if (action < 0):
            if abs(action) > current_quantity:
                continue
        elif (action == 0):
            continue

        updated_quantity = current_quantity + action
        c.execute("INSERT INTO Activities VALUES (?, ?, ?, ?);", line)
        c.execute("UPDATE Products SET quantity=? WHERE id=?", (updated_quantity, product_id,))
        conn.commit()

printdb()

