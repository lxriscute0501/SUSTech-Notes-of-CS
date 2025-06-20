# 1
words = "southern, university, of, science, and, technology"

word_list = words.split(',')

new_word = []

for word in word_list:
    vocab = word.strip()
    capitalized_word = vocab.capitalize()
    new_word.append(capitalized_word)

result = ' '.join(new_word)

print(result)


# 2
sentence = "The SUsTech was found in the year 2010"

word_count = len(sentence.split())
digit_count = sum(c.isdigit() for c in sentence)
upper_count = sum(c.isupper() for c in sentence)
lower_count = sum(c.islower() for c in sentence)

print("Word count:", word_count)
print("Digit count:", digit_count)
print("Uppercase letters:", upper_count)
print("Lowercase letters:", lower_count)


# 3
sentence = "You cannot end a sentence with because because because is a conjunction"

because_count = sentence.lower().split().count("because")

print("Occurrences of 'because':", because_count)


# 4
number = int(input("Enter a positive integer with at least 3 digits: "))
sum = 0
while number > 0:
    digit = number % 10
    sum += digit
    number = number // 10
print("Sum of digits:", sum)


#5
matrix = [[10, 20], [30, 40], [50, 60]]

rows = len(matrix)
cols = len(matrix[0])

transpose = []

for i in range(cols):
    column = []
    for j in range(rows):
        column.append(matrix[j][i])
    transpose.append(column)

print(transpose)


# 6
sentence = "can you can a can as a canner can can a can"

words = sentence.split()

word_count = {}

for word in words:
    word_count[word] = word_count.get(word, 0) + 1

print(word_count)
