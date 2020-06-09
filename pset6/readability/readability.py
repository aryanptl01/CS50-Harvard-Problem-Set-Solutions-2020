from cs50 import get_string

letters = 0
words = 1
sentences = 0

text = get_string("Text to be analysed: ")
n = len(text)
for i in range(n):
    if text[i].isalpha():
        letters += 1
    elif text[i].isspace():
        words += 1
    elif text[i] == '.' or text[i] == '?' or text[i] == '!':
        sentences += 1

L = ((letters * 100.00) / words)
S = ((sentences * 100.00) / words)

answer = 0.0588 * L - 0.296 * S - 15.8

answer = round(answer)
final = answer

if answer < 1:
    print("Before Grade 1")
elif answer >= 16:
    print("Grade 16+")
elif answer >= 1:
    print("Grade", answer)
