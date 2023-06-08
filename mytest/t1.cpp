#include <cassert>
#include <cstdlib>
#include <functional>
#include <list>
#include <utility>
#include <iostream>
#include <vector>
// using namespace std;
#include <unordered_map>



int main(){

    std::unordered_map<int, int> mp;
    mp[0];
    std::cout << "mp[0] is " << mp[0] << std::endl;
    std::cout << "mp[1] is " << mp[1] << std::endl;

    return 0;
}