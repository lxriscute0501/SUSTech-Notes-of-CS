# 1
class Vehicle:
    def __init__(self, name, max_speed, mileage):
        self.name = name
        self.max_speed = max_speed
        self.mileage = mileage


# 2
class Bus(Vehicle):
    vehicle_type = 'bus'

    def show(self):
        print("Name:", self.name)
        print("Max speed:", self.max_speed)
        print("Mileage:", self.mileage)
        print("Vehicle type:", self.vehicle_type)

bus = Bus("School Bus", 120, 5000)


# 3

class Rectangle:
    def __init__(self, length, width):
        self.length = length
        self.width = width

    def Perimeter(self):
        return 2 * (self.length + self.width)

    def Area(self):
        return self.length * self.width

    def display(self):
        print("Length:", self.length)
        print("Width:", self.width)
        print("Perimeter:", self.Perimeter())
        print("Area:", self.Area())

rect = Rectangle(5, 3)
rect.display()


# 4
class BankAccount:
    def __init__(self, accountNumber, name, balance):
        self.accountNumber = accountNumber
        self.name = name
        self.balance = balance

    def Deposit(self, amount):
        self.balance += amount

    def Withdrawal(self, amount):
        if self.balance >= amount:
            self.balance -= amount
        else:
            print("Insufficient balance")

    def bankFees(self):
        self.balance *= 0.95

    def display(self):
        print("Account Number:", self.accountNumber)
        print("Name:", self.name)
        print("Balance:", self.balance)

newAccount = BankAccount(2178514584, "Albert", 2700)
newAccount.Withdrawal(300)
newAccount.Deposit(200)
newAccount.bankFees()
newAccount.display()
