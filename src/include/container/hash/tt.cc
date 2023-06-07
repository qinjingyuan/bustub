#include<iostream>
using namespace std;
class A;
class B;
class A{
    int a;
    B* ptb;
public:
    A(int v ,B* p){
        a = v;
        ptb = p;
    }

    bool set(int v){
        a = v;
        ptb->show();
        ptb->set(100);
        ptb->show();
    }
    bool show(){
        cout << "A::a is " << a << endl;
    }

};

class B {
    int b;
    A ta = A(5,this);
public:
    B(int v ){
        b = v;
    }

    bool set(int v){
        b = v;
        ta.show();
        ta.set(10);
        ta.show();
    }
    bool show(){
        cout << "B::b is " << b <<endl;
    }

};




int main(){
    B tb = B(15);
    tb.show();
    tb.set(10);
    tb.show();
    return 0;
};




