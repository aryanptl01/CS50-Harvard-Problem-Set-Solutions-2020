from sys import argv, exit
import csv

if len(argv) != 3:
    print("Usage: database.csv sequence.txt")
    exit(1)
database = open(argv[1], "r")                 # Creates an object to read items into a dictionary
dnasequence = open(argv[2], "r")              # Creates an object to read items into a list

data = csv.reader(database)
seq = csv.reader(dnasequence)

fieldlist = []
STRdict = {}
sequence = []
userstr = []

i = 0
for row in data:                            # Making our own dict of applicants and their STR counts
    if i == 0:
        fieldlist = row.copy()
    else:
        STRdict[row[0]] = row.copy()
        STRdict[row[0]].pop(0)
    i = 1

for row in seq:                              # Abstracting the given sequence from its file
    sequence = row.copy()
    seq = sequence[0]
    sequence.clear()

for j in range(1, len(fieldlist)):
    reps = fieldlist[j]
    maxreps = 0
    while seq.count(reps) > 0:
        maxreps += 1
        reps += fieldlist[j]
    userstr.append(str(maxreps))

for items in STRdict:
    if STRdict[items] == userstr:
        print(items)
        exit(0)

print("No match")
database.close()
dnasequence.close()


# one place i have some issues about is that on line 7-8 and 46-47, we're opening and closing our file
# david said on the lecture that you can do the shorthand, "with open (argv[1], "r") as filename"
# also on line 27, i'm wondering if there's a shorter way to just get the string from the list! i'll have a look at that tomorrow as well