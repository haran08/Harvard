import csv
import sys

def load_file(name:str, extension:str):
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
