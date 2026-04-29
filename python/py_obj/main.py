class Animal:
    def __init__(self, name, age):
        self.name = name
    def make_sound(self):
        pass

class Dog(Animal):
    def __init__(self, name, age):
        super().__init__(name, age)
    @staticmethod
    def make_sound():
        print(f"woof")

class Cat(Animal):
    def __init__(self, name, age):
        super().__init__(name, age)
    @staticmethod
    def make_sound():
        print(f"meow")

class Bird(Animal):
    def __init__(self, name, age):
        super().__init__(name, age)
    @staticmethod
    def make_sound():
        print(f"tweet")


class AnimalFactory:
    def __init__(self):
        self.animals = {
            "dog": Dog,
            "cat": Cat,
            "bird": Bird
        }
    def create_animal(self, animal_type, name, age):
        if animal_type not in self.animals:
            raise ValueError(f"Invalid animal type: {animal_type}")
        return self.animals[animal_type](name, age)


dog = AnimalFactory().create_animal("dog", "Buddy", 3)
bird = AnimalFactory().create_animal("bird", "Tweetie", 1)
cat = AnimalFactory().create_animal("cat", "Whiskers", 2)
print(dog.make_sound())
print(bird.make_sound())
print(cat.make_sound())