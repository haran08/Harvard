import csv
import requests
from datetime import date


def main():
    # Read NYTimes Covid Database
    download = requests.get(
        "https://raw.githubusercontent.com/nytimes/covid-19-data/master/us-states.csv"
    )
    decoded_content = download.content.decode("utf-8")
    file = decoded_content.splitlines()
    reader = csv.DictReader(file)

    # Construct 14 day lists of new cases for each states
    new_cases = calculate(reader)

    # Create a list to store selected states
    states = []
    print("Choose one or more states to view average COVID cases.")
    print("Press enter when done.\n")

    while True:
        state = input("State: ")
        if state in new_cases:
            states.append(state)
        if len(state) == 0:
            break

    print(f"\nSeven-Day Averages")

    # Print out 7-day averages for this week vs last week
    comparative_averages(new_cases, states)


# TODO: Create a dictionary to store 14 most recent days of new cases by state
def calculate(reader):
    N_days = 14
    l = list(reader)
    new_cases = dict()
    f_day = date.fromisoformat(l[-1]["date"])
    l_day = f_day.replace(day = f_day.day - N_days)
    previous_cases = dict()


    for i in range(len(l)):
        day = date.fromisoformat(l[-1 - i]["date"])
        index = l[-1 - i]
        if day < l_day:
            break
        if index["state"] in new_cases:
                continue
        new_cases[index["state"]] = []
        previous_cases[index["state"]] = 0

    last_day_reverse_index = ( (len(previous_cases) * N_days) + len(previous_cases) ) * -1
    for i in range(last_day_reverse_index, 0):
        index = l[i]
        day = date.fromisoformat(index["date"])

        ci = int(index["cases"])

        c = ci - previous_cases[index["state"]]

        new_cases[index["state"]].append({ "date": str(day), "cases": c })

        previous_cases[index["state"]] = ci

    return new_cases

# TODO: Calculate and print out seven day average for given state
def comparative_averages(new_cases, states):
    last_seven_ac = 0
    preview_seven_ac = 0
    for state in states:
        for i in new_cases[state][:7]:
            preview_seven_ac += i["cases"]
        for i in new_cases[state][7:]:
            last_seven_ac += i["cases"]

        try:
            p = (last_seven_ac - preview_seven_ac) / last_seven_ac

            description_increase_decrease = "increase" if p > 0 else "decrease"
            print(f"{state} had a 7-day average of {round(last_seven_ac / 7)} and an {description_increase_decrease} of {round(p)}%")
        except ZeroDivisionError:
            print(f"{state} had a 7-day average of 0.")
main()
