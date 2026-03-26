import csv
import sys

S_USAGE = "Usage: ./dna.py {database.csv} {sequences.txt}"


def main():

    # TODO: Check for command-line usage
    if len(sys.argv) != 3:
        print(f"Error: invalid starting parameters.\n{S_USAGE}")
        return 1
    elif sys.argv[1][-4:] != ".csv":
        print("Error: invalid database file format. Expected {database.csv}")
        return 1
    elif sys.argv[2][-4:] != ".txt":
        print("Error: invalid sequence file format. Expected {sequence.txt}")
        return 1

    # TODO: Read database file into a variable
    database = load_file(sys.argv[1], "csv")
    if database == None:
        print("Unable to load database.")
        return 1

    # TODO: Read DNA sequence file into a variable
    sequence = load_file(sys.argv[2], "txt")
    if sequence == None:
        print("Unable to laod sequence")
        return 1

    # TODO: Find longest match of each STR in DNA sequence
    profiles = dict()

    for k in database[0].keys():
        if k == "name":
            continue

        profiles[k] = longest_match(sequence, k)

    # TODO: Check database for matching profiles

    equal = set()
    for p in database:
        equal.add(p["name"])

    for k in profiles.keys():
        for p in database:
            if int(p[k]) != profiles[k]:
                equal.discard(p["name"])
                continue

    if len(equal) > 0:
        print(equal.pop())
        return

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


def load_file(name: str, extension: str):
    try:
        match extension:
            case "csv":
                database = []
                with open(name, newline='') as csvfile:
                    reader = csv.DictReader(csvfile)
                    for row in reader:
                        database.append(row)
                return database

            case "txt":
                sequence = str()
                with open(sys.argv[2], "r", encoding="utf-8", newline='') as txtfile:
                    sequence = txtfile.read(-1)
                return sequence
            case _:
                print("Unable to load file: Unsuported extension used.")

    except FileNotFoundError:
        exc_type, exc_value, exc_traceback = sys.exc_info()
        frame = exc_traceback.tb_frame
        code = frame.f_code

        message = f"{exc_type.__name__} -> {exc_value}\n\n Function: Name -> {code.co_name} | Param -> ( name = {frame.f_locals["name"]}, extension = {frame.f_locals["extension"]} ) | Line -> {code.co_firstlineno}\n\n Code: Line -> {exc_traceback.tb_lineno}\n"

        print(message)
        return None


main()
