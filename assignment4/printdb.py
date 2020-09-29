import sys
import sqlite3


def printdb():

    try:
        conn = sqlite3.connect('moncafe.db')
    except Exception:
        print('There is no database')
        sys.exit()

    c = conn.cursor()

    print('Activities')
    for row in c.execute('SELECT * FROM Activities ORDER BY date ASC'):
        print(row)

    print('Coffee stands')
    for row in c.execute('SELECT * FROM Coffee_stands ORDER BY id ASC'):
        print(row)

    print('Employees')
    for row in c.execute('SELECT * FROM Employees ORDER BY id ASC'):
        print(row)

    print('Products')
    for row in c.execute('SELECT * FROM Products ORDER BY id ASC'):
        print(row)

    print('Suppliers')
    for row in c.execute('SELECT * FROM Suppliers ORDER BY id ASC'):
        print(row)


    c1 = conn.cursor()
    print("\nEmployees report")
    for row in c.execute('SELECT id FROM Employees'):
        c1.execute("""
                SELECT name, salary, location, SUM(Products.price*-Activities.quantity)
                FROM ((Employees LEFT JOIN Activities ON activator_id = id) AS REC
                LEFT JOIN Products on REC.product_id = Products.id) AS REC1
                JOIN Coffee_stands ON Coffee_stands.id = Employees.coffee_stand
                WHERE Employees.id = ?
        """, (row[0],))
        data = c1.fetchone()

        if not data[-1]:
            data = (*data[:-1], 0)
        print(*data)
    c1.close()
    

    query = """
        SELECT date, description, Activities.quantity, Employees.name, Suppliers.name
        FROM ((Activities JOIN Products ON Activities.product_id = Products.id) AS REC
        LEFT JOIN Suppliers ON Suppliers.id = REC.activator_id) AS REC1
        LEFT JOIN Employees ON Employees.id = REC1.activator_id
        order by date
    """
    c.execute(query)
    data = c.fetchall()
    if len(data) > 0:
        print("\nActivities")
        for row in data:
            print(row)
            

if __name__ == "__main__":
    printdb()
