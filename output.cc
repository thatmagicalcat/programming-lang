#include<iostream>
template <typename T1>
auto say_hello(T1 name){std::cout<<"Hello ";std::cout<<name<<std::endl;}int main(){say_hello("wiz");}