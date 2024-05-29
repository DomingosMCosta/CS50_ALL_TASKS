import re
import sys

text = input("Text ")
if not text:
    sys.exit(1)

words = len(text.split())
letters = 0
phrases = sum(text.count(p) for p in ['?', '!', '.'])
# instead of using re library, I could replace all punctuations for the same. Ex: substitue ("!" and "?" for ".")
# this could be achieved with text = text.replace('!', '.').replace('?', '.')
# phrases = len(re.split('[!?.]', text)) - 1

for char in text:
    if char.isalpha():
        letters += 1
L = letters / words * 100
S = phrases / words * 100
index = round(0.0588 * L - 0.296 * S - 15.8)

if index <= 1:
    print("Before Grade 1")
elif index >= 16:
    print("Grade 16+")
else:
    print(f"Grade {int(index)}")
