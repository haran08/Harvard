class Jar:
    def __init__(self, capacity=12):
        if capacity < 0:
            raise ValueError("Capacity can't be smaller than 0")

        self.__capacity = capacity
        self.__size = 0

    def __str__(self):
        return "🍪" * self.size

    def deposit(self, n):
        if self.size + n > self.capacity:
            raise ValueError(f"Passed the limit of cookies in the jar. Olny {self.capacity} / current: {self.size}")

        self.__size += n

    def withdraw(self, n):
        if self.size < n:
            raise ValueError(f"Jar don't have {n} cookies. Only {self.size}")

        self.__size -= n

    @property
    def capacity(self):
        return self.__capacity

    @property
    def size(self):
        return self.__size

jar = Jar(5)
jar.deposit(4)
jar.withdraw(2)
print(jar)
