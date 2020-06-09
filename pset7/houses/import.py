from cs50 import SQL
from sys import argv, exit
import csv

db = SQL("sqlite:///students.db")

if len(argv) != 2:
    print("Usage: python import.py students.db")
    exit(1)

file = open(argv[1], "r")

data = csv.reader(file)

for row in data:

    if row[0] == "name":
        continue

    name = ""
    name = ' '.join(row)
    splname = name.split()
    n = len(splname)

    if n == 5:
        db.execute("INSERT INTO students(first, middle, last, house, birth) VALUES (?, ?, ?, ?, ?)", splname[0], splname[1], splname[2], splname[3], splname[4])

    elif n == 4:
        db.execute("INSERT INTO students(first, middle, last, house, birth) VALUES (?, NULL, ?, ?, ?)", splname[0], splname[1], splname[2], splname[3])

exit(0)



