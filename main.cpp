#include "constexpr_map.hpp"

using constexpr_map::const_map;
using constexpr_map::select_parameter;

template<typename T, T init>
constexpr auto get_map() {
    constexpr auto parameters = select_parameter<int, int, init.size()>::select(
            init
        );
    constexpr auto Size = parameters.first;
    constexpr auto N = parameters.second;
    constexpr auto map = const_map<int, int, Size, N>{
        init
    };
    return map;
}

int main() {
    constexpr auto init = std::array{
        std::pair{ 1,2 },
        std::pair{ 3, 4 },
        std::pair{ 7, 1 },
        std::pair{ 10, 0 },
        std::pair{ 110, 2 },
        std::pair{ 32, 5},
        std::pair{ 72, 36},
        std::pair{ 100000, 5},
        std::pair{ 100001, 6},
    };
    auto map = get_map<typeof(init),init>();

    std::cout << "map's count: " << init.size() << std::endl;
    std::cout << "map's value size: " << sizeof(init[0].second) << std::endl;
    std::cout << "map's size: " << sizeof(map) << std::endl;

    int x{};
    std::cin >> x;
    std::cout << map[x];
    return 0;
}
