from cs50 import get_float

while True:
    d = get_float("Change: ")
    if d > 0:
        break
c = round(d * 100)

t = c - c % 25
f = t / 25

c = c - f * 25

e = c - c % 10
n = e / 10

c = c - n * 10

v = c - c % 5
r = v / 5

c = c - r * 5

o = c - c % 1
u = o / 1

c = c - u * 1

q = round(r + u + n + f)

print(f"{q}")
