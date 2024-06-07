#include <string>
#include <vector>

#include <iostream>

struct Person {
    std::string name;
    unsigned int age;

    Person(const std::string& name) {
        this->name  = name;
        this->age   = 25;
    }
};

struct PersonByName {
    PersonByName() {}

    PersonByName(const std::string& name) {
        this->name = name;
    }

    bool operator()(const std::string& other) const {
        return other.find(this->name) != std::string::npos;
        // return name == this->name;
    }

private:
    std::string name;
};

template<typename Predicate>
class Container {
public:
    Container(const Predicate& predicate) {
        this->predicate = predicate;
        this->persons = std::vector<Person>();
    }

    void addPerson(const std::string& name) {
        if (predicate(name)) {
            persons.push_back(Person(name));
        }
    }

    void printPersons() const {
        if (persons.size() == 0) {
            std::cout << "There are no people in the container!" << std::endl;
        } else {
            std::cout << "The persons in the container are: " << std::endl;

            for (std::size_t i = 0; i < persons.size(); ++i) {
                std::cout << "{ " << persons[i].name << " }" << std::endl;
            }
        }
    }

private:
    Predicate predicate;
    std::vector<Person> persons;
};

int main() {
    PersonByName policy = PersonByName("Kristina");
    Container<PersonByName> container(policy);

    container.addPerson("Kristina K.");
    container.addPerson("Kristina A.");
    container.addPerson("Teodor D.");
    container.addPerson("Simona");

    container.printPersons();

    return 0;
}
