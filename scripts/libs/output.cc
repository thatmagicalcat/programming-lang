#include<iostream>
template <typename T1>
auto exec(T1 command){system(command);}auto print_time(){exec("date");}int main(){exec("echo hello world");print_time();}