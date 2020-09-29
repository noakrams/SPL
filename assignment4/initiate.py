import os, sys
import sqlite3

filename = "{}".format(sys.argv[1])
try:
    os.remove('moncafe.db')
except FileNotFoundError:
    pass
finally:
    conn = sqlite3.connect('moncafe.db')

c = conn.cursor()


def create_database():
    # Create table
    c.execute('''
        CREATE TABLE Coffee_stands
        (
            id INTEGET PRIMARY KEY,
            location TEXT NOT NULL,
            number_of_employees INTEGER
        );
        ''')

    c.execute('''
        CREATE TABLE Employees
        (
            id INTEGER PRIMARY KEY,
            name TEXT NOT NULL,
            salary REAL NOT NULL,
            coffee_stand INTEGER REFERENCES Coffee_stands(id)
        );
        ''')

    c.execute('''
        CREATE TABLE Suppliers
        (
            id INTEGER PRIMARY KEY,
            name TEXT NOT NULL,
            contact_inforamtion TEXT
        );
        ''')

    c.execute('''
        CREATE TABLE Products
        (
            id INTEGER PRIMARY KEY,
            description TEXT NOT NULL,
            price REAL NOT NULL,
            quantity INTEGER REFERENCES Coffee_stands(id)
        );
        ''')

    c.execute('''
        CREATE TABLE Activities
        (
            product_id INTEGER REFERENCES Product(id),
            quantity INTEGER NOT NULL,
            activator_id INTEGER NOT NULL,
            date DATE NOT NULL
        );
        ''')

    conn.commit()


create_database()
with open(filename, "r") as config:
    for line in config:
        line = line.strip()
        line = [item.strip() for item in line.split(',')]
        if line[0] == 'C':
            c.execute("INSERT INTO Coffee_stands VALUES (?, ?, ?);", line[1:])
        elif line[0] == 'S':
            c.execute("INSERT INTO Suppliers VALUES (?, ?, ?);", line[1:])
        elif line[0] == 'E':
            c.execute("INSERT INTO Employees VALUES (?, ?, ?, ?);", line[1:])
        elif line[0] == 'P':
            c.execute("INSERT INTO Products VALUES (?, ?, ?, 0);", line[1:])
        conn.commit()

conn.close()
