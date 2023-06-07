// #include<iostream>
// using namespace std;
// class A;
// class B;
// class A{
//     int a;
//     B* ptb;
// public:
//     A(int ,B*);
//     bool set(int );
//     bool show();
// };

// class B {
//     int b;
//     A ta = A(5,this);
//     // A ta(5,this);
//     A ta;
// public:
//     B(int v );
//     bool set(int);
//     bool show();

// };

// A::A(int v ,B* p){
//     a = v;
//     ptb = p;
// }

// bool A::set(int v){
//     a = v;
//     ptb->show();
//     ptb->set(100);
//     ptb->show();
// }
// bool A::show(){
//     cout << "A::a is " << a << endl;
// }

// B::B(int v ){
//     b = v;
// }

// bool B::set(int v){
//     b = v;
//     ta.show();
//     ta.set(10);
//     ta.show();
// }
// bool B::show(){
//     cout << "B::b is " << b <<endl;
// }

// int main(){
//     B tb = B(15);
//     tb.show();
//     tb.set(10);
//     tb.show();
//     return 0;
// };




#include <iostream>
#include <vector>
using namespace std;



class A{
    vector<int> a = vector<int>(10);
public:
    bool show()
    {
        for(auto& e : a){
            cout << e << endl;
        }
        return true;
    }
    bool add(){
        a.push_back(3);
        return true;
    }

};




int main()
{
    A ta = A();
    ta.show();
    ta.add();
    ta.show();
    return 0;
}


