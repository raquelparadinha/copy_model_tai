import random
import string

def create_random_file(file_path, num_letters):
    with open(file_path, 'a') as file:
        for _ in range(num_letters):
            random_letter = random.choice(string.ascii_uppercase)
            file.write(random_letter)

file_path = '../../example/random_file.txt'
num_letters = 100000000

create_random_file(file_path, num_letters)