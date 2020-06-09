from cs50 import get_int

while True:
    n = get_int("Height: ")
    if n > 0:
        if n < 9:
            break

j = n - 1

for i in range(n):
    print(" " * j, end="")
    j -= 1
    s = i + 1
    print("#" * s, end="")
    print("  ", end="")
    print("#" * s)