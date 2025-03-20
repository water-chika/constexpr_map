#include "constexpr_map.hpp"
#include <iostream>

using constexpr_map::construct_const_map;

int main() {
    constexpr auto init = std::to_array<std::pair<int,int>>({
        { 1,2 },
        { 3, 4 },
        { 7, 1 },
        { 10, 0 },
        { 110, 2 },
        { 32, 5},
        { 72, 36},
        { 100000, 5},
        { 100001, 6},
    });
    auto map = construct_const_map < init, decltype([](auto d) { return d.first; }) >();

    std::cout << "map's count: " << init.size() << std::endl;
    std::cout << "map's value size: " << sizeof(init[0].second) << std::endl;
    std::cout << "map's size: " << sizeof(map) << std::endl;

    int x{};
    std::cin >> x;
    std::cout << map[x].second;
    return 0;
}
