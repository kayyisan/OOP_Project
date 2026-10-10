#include<iostream>
#include<vector>

using namespace std;

//class abstract with its pure virtual function
class Vehicle{
    public:
        virtual void move() const = 0; //pure virtual function
        virtual ~Vehicle() {} //virtual destructor
};

class Car : public Vehicle{
    private:
        string model;
    public:
        Car(string m) : model(m) {}
        void move() const override{
            cout << model << " is driving in the road" << endl;
        }
};

class Bicycle : public Vehicle{
    private:
        string brand;
    public:
        Bicycle(string b) : brand(b) {}
        void move() const override{
            cout << brand << " bicycle is pedaling along the bike lane" << endl;
        }
};

int main(){
    //object car and bicycle and put them into vector
    Vehicle* myCar = new Car("Toyota");
    Vehicle* myBike = new Bicycle ("Polygon");

    vector<Vehicle*> vehicleCollection;
    vehicleCollection.push_back(myCar);
    vehicleCollection.push_back(myBike);

    cout << "---Demonstrasi Polimorfisme pada Vehicle---" << endl;
    for (auto v : vehicleCollection){
        v->move();
    }

    delete myCar;
    delete myBike;

    return 0;
}