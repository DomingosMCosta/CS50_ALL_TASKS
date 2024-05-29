import csv
import sys


def main():
    # TODO: Check for command-line usage
    if len(sys.argv) != 3:
        print(f"Invalid number of inputs")
        sys.exit(1)
    f = sys.argv[1]
    d = sys.argv[2]
    # TODO: Read database file into a variable
    adn_bank = []
    with open(f) as file:
        reader = csv.DictReader(file)
        # print(reader.fieldnames)
        for row in reader:
            adn_bank.append(row)
    # TODO: Read DNA sequence file into a variable
    with open(d) as file:
        adn_user = file.read()
        # adn_user = file.readlines() - this method read each line and puts it in a list, meaning when I do
        # len(adn_user) it will return 1. Instead I could use len(adn_user[0])

    # TODO: Find longest match of each STR in DNA sequence
    max_str = ""
    max_counter = 0
    user_dict = {}

    for adn_string in (reader.fieldnames[1:]):
        buffer = longest_match(adn_user, adn_string)
        user_dict[adn_string] = buffer

    '''This was my first attempt, with the purpose to then
    create a list with value. This solution is not eficient
    as you can see
    for i in range(len(adn_user) - 5):
        buffer_counter = 0
        char_4 = adn_user[index_string:index_string+4]
        if char_4 in(reader.fieldnames[1:]):
            buffer_counter = adn_user.count(char_4)
        if buffer_counter > max_counter:
            max_counter = buffer_counter
            max_str = char_4
        index_string += 1'''

    # TODO: Check database for matching profiles
    '''This will iterate through all dictionaries inside the list of dicionaries (adn_bank) then
    iterate through all keys and values inside the user dictionary (user_dict) (not list because its just one user,
    so 1 dictionary)
    then if the value from current dictionary (row in adn_bank) doesnt match the value inside key of user_key
    switch to false and breaks loop move to next dictionary (row)
    if all values inside all keys of the user_dict mactches all values inside all keys of current dictionary (row)
    then in the end it will print the value of the key 'name' of that row
    the "int(dict.get(key)) != value" will compare all value in just that line of code
    '''
    for dict in adn_bank:
        match = True
        for key, value in user_dict.items():
            if int(dict.get(key)) != value:
                match = False
                break
        if match:
            print(f"{dict['name']}")
            break
    else:
        print("No match")

    return


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
