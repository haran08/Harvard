while True:
    height = input("Height: ")

    if not height.isdigit() or height == "":
        continue

    height = int(height)
    if height > 8 or height < 1:
        continue
    break

for i in range(height):
    s = "#" * (i + 1)
    print(f"{s}  {s}".center((height + 1) * 2).rstrip())
