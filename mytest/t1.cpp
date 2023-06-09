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

    std::unordered_map<int, int> mp{{1,2},{11,22},{33,44}};
    // mp[0];



    // std::cout << "mp[0] is " << mp[1] << std::endl;
    mp.erase(mp.find(111));
        for(auto e:mp){

            std::cout << e.first<< " " << e.second << std::endl;
        }

    return 0;
}