#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <typeinfo>

using namespace std;

class Planet{
private:
    string name;
    int age;
    float food_units;

public:

    float get_food(){
        return food_units;
    }
    void increase_age(int add_age){
        age = age+add_age;
        return;
    }
    bool more_food(float amount){
        food_units = food_units+amount;
        return true;
    }
    bool less_food(float amount){
        if(food_units-amount<0) {
                return false;
                }else{
                food_units = food_units-amount;
                }
        if(food_units == 0) cout<<"No food left"<<endl;
        return true;
    }
    void set_age(int given_age){
        if (given_age<age) {
            cerr<<"Given age is lower than current age of planet "<<this->get_name()<<".\n";
            return;
        }
    age=given_age;
    }

    string get_name() const {
    return name;
    }

    int get_age(){
    return age;
    }

Planet(string given_name, int given_age = 0){

    food_units = rand() % 100 + 15;
    name = given_name;
    age = given_age;

}

~Planet(){
 cout<<"Planet "<<this->get_name()<<" was destructed."<<endl;
}

};

class Organism {
private:
    bool is_alive = 1;
    Planet const& home_planet;
public:

    bool check_is_alive(){
        return is_alive;
    }
    bool set_is_alive(int aliveness){
        is_alive=aliveness;
        return true;
    };

    Organism (Planet const& planet) : home_planet(planet) {
    }

    string get_planet_name() {
        return home_planet.get_name();
    }
    Planet const& get_planet() const {
        return home_planet;
    }
};

class Animal: public Organism {
private:
    int age;
    float weight;
public:
    Animal(Planet const& home_planet) : Organism(home_planet) {
    this->age = rand()%101 +1;
    this->weight = rand()%51+14;
    }

int add_age(){
    return age++;
}
int add_weight(){
    return weight++;
}

bool consumeFood(){
    if (!this->check_is_alive()) return false;
    float x = weight/10;
    bool result = const_cast<Planet&>(this->get_planet()).less_food(x);//1 unit of food per each 10 kilograms
    if (result){
        cout<<"Food was taken from the planet"<<endl;
        return true;
    }else {
        this->set_is_alive(0);
        cout<<"An animal died."<<endl;
        //this->~Animal();
        return false;
    }
}

~Animal(){
cout<<"\tRIP animal"<<endl;
};

};

class Plant : Organism{
private:
    float production_coefficient;
    float height = 1;

public:
    Plant(Planet const& home_planet) : Organism(home_planet) {
    float x = (rand()%5+1);
    this->production_coefficient = x/10;
    }

    float add_height(float value){
        height = height+value;
    }

    bool produceFood(){
    cout<<"Production coefficient: "<<production_coefficient<<endl;
    float x = production_coefficient * height;
    // const_cast changes const& to &
    bool result = const_cast<Planet&>(this->get_planet()).more_food(x);
    if (result){
        cout<<x<<" Food was added to the planet"<<endl;
        return true;
        }
    }
};


struct List{
    Animal * animal;
    Plant * plant;
    List * next;
};


void add_to_list(List *head, Animal * animal){
    List * current = head;
    if (current == NULL){
        List * new_item = new List();
        new_item->animal = animal;
        new_item->plant = NULL;
        new_item->next = NULL;
        head = new_item;
        return;
    }

    while(current->next){
        current = current->next;
    }

        List * new_item = new List();
        new_item->animal = animal;
        new_item->plant = NULL;
        new_item->next = NULL;
        current->next = new_item;


}

void add_to_list(List *head, Plant * plant){
    List * current = head;
    if (current == NULL){
        List * new_item = new List();
        new_item->animal = NULL;
        new_item->plant = plant;
        new_item->next = NULL;
        head = new_item;
        return;
    }

    while(current->next){
        current = current->next;
    }

        List * new_item = new List();
        new_item->animal = NULL;
        new_item->plant = plant;
        new_item->next = NULL;
        current->next = new_item;
}

bool is_at_least_one_animal_alive(List * head){
    List * current = head;
    while(current){
        if (current->animal){
            if (current->animal->check_is_alive()) return true;
        }
        current = current->next;
    }
    return false; //no alive animals :C
}

int main(){
srand(time(NULL));

List * head = new List();


Planet Earth("Earth");
cout<<Earth.get_name()<<" wiek: "<<Earth.get_age()<<" food: "<<Earth.get_food()<<endl;

Animal shark(Earth);
Animal boar(Earth);
Animal cat(Earth);
Animal opossum(Earth);
Animal rat(Earth);

Plant tree(Earth);
Plant grass(Earth);
Plant banana(Earth);

add_to_list(head,&shark);
add_to_list(head,&boar);
add_to_list(head,&cat);
add_to_list(head,&opossum);
add_to_list(head,&rat);

add_to_list(head, &tree);
add_to_list(head, &grass);
add_to_list(head, &banana);


List * current = head;

while(Earth.get_food() && is_at_least_one_animal_alive(head)){
    Earth.increase_age(1);
    cout<<"Age: "<<Earth.get_age()<<endl;
    List * current = head;
    while(current){
        if (current->animal!=NULL) {
            current->animal->consumeFood();
            current->animal->add_weight();
            current->animal->add_age();
        };
        if (current->plant!=NULL) {
            current->plant->produceFood();
            current->plant->add_height(0.1);
        };
    current = current->next;
    }
cout<<"Food amount: "<<Earth.get_food()<<endl;
}
cout<<"After all consumption: "<<Earth.get_food()<<endl;

return 0;
}
