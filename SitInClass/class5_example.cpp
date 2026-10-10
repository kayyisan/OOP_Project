#include <iostream>
#include <string>
using namespace std;

class worker{
    protected :
        string name;
        int salary;
    public:
        worker(string a, int s) : name(a), salary(s){}
        virtual void displaySalary(){
            cout << "nama : " << name << endl
            << "Gaji : " << salary << endl;
        }
};

class teacher : public worker{
    protected:
        string schoolName;
    public:
        teacher (string a, int s, string n) : worker(a,s), schoolName(n){}
        string getSchoolName(){
            return schoolName;
        }
        void displaySalary() override{
            cout << "Nama : " << name << endl
            << "Gaji : " << salary << endl
            << "Tempat Kerja : " << schoolName << endl;
        }
};

class smp : public teacher{
    protected:
        string subjectToTeach;
    public :
        
}

int main(){
    worker kay ("kayyisa", 900000);
    teacher azmud ("azmud", 19237318, "MBS");
    kay.displaySalary();
    azmud.displaySalary();
    return 0;
}