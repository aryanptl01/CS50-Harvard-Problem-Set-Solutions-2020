from cs50 import SQL
from sys import argv

if len(argv) != 2:
    print("Usage: python roster.py House")
    exit(1)

db = SQL("sqlite:///students.db")

output_dict = {}
output_dict = db.execute("SELECT first, middle, last, birth FROM students WHERE house = ? ORDER BY last, first;", argv[1])

for row in output_dict:
    n = row["middle"]
    if n != None:
        print(f"{row['first']} {row['middle']} {row['last']}, born {row['birth']}")
    else:
        print(f"{row['first']} {row['last']}, born {row['birth']}")

exit(0)



