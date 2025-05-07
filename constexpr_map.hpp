#pragma once

#include <array>
#include <cassert>
#include <algorithm>

namespace constexpr_map {

using std::size_t;

template<typename T, size_t N, size_t Size>
constexpr size_t hash0(T t) {
    return (t^N) % Size;
}

template<typename Container, typename T>
concept container = std::same_as<typename Container::value_type, T>;

template<class T, class U, size_t Size, size_t MaxSize=Size*2, size_t N = 0, size_t MaxN = 2>
struct select_parameter {
    static constexpr std::pair<size_t,size_t> select(container<std::pair<T, U>> auto map) {
        if (MaxN == N) {
            assert(0);
        }
        else {
            std::array<bool, Size> used_indices{};
            for (auto [x, y] : map) {
                auto value_index = hash0<T, N, Size>(x);
                if (used_indices[value_index]) {
                    return select_parameter<T, U, Size, MaxSize, N + 1, MaxN>::select(map);
                }
                used_indices[value_index] = true;
            }
        }
        return { Size, N };
    }
};
template<class T, class U, size_t Size, size_t MaxSize, size_t MaxN>
struct select_parameter<T, U, Size, MaxSize, MaxN, MaxN> {
    static constexpr std::pair<size_t, size_t> select(container<std::pair<T, U>> auto map) {
        return select_parameter<T, U, Size + 1, MaxSize, 0, MaxN>::select(map);
    }
};
template<class T, class U, size_t Size, size_t N>
struct select_parameter<T, U, Size, Size, N, N> {
    static constexpr std::pair<size_t, size_t> select(container<std::pair<T, U>> auto map) {
        assert(0);
        return {};
    }
};

template<class T, class U, size_t Size, size_t N>
class const_map {
public:
    consteval const_map(container<std::pair<T,U>> auto map) : m_values{} {
        std::array<bool, Size> used_indices{};
        for (auto [x, y] : map) {
            auto value_index = hash0<T, N, Size>(x);
            assert(!used_indices[value_index]);
            m_values[value_index] = y;
            used_indices[value_index] = true;
        }
    }
    constexpr U operator[](T t) const {
        return m_values[hash0<T, N, Size>(t)];
    }
private:
    std::array<U, Size> m_values;
};

template<typename T, T init>
constexpr auto get_map() {
    constexpr auto parameters = select_parameter<decltype(init[0].first), decltype(init[0].second), init.size()>::select(
        init
    );
    constexpr auto Size = parameters.first;
    constexpr auto N = parameters.second;
    constexpr auto map = const_map<decltype(init[0].first), decltype(init[0].second), Size, N>{
        init
    };
    return map;
}


constexpr auto get_from(auto data, auto get_key) {
    std::array<std::pair<decltype(get_key(data[0])), std::remove_cvref_t<decltype(data[0])>>, data.size()> map{};
    for (int i = 0; i < map.size(); i++) {
        map[i] = { get_key(data[i]), data[i] };
    }
    return map;
}

template<auto Data, typename GetKey>
constexpr auto construct_const_map() {
    constexpr auto map = get_from(Data, GetKey{});
    return constexpr_map::get_map<decltype(map), map>();
}

constexpr auto unique_map(auto data, auto get_key) {
    std::sort(data.begin(), data.end(), [&get_key](auto l, auto r) { return get_key(l) < get_key(r); });
    auto last = std::unique(data.begin(), data.end(), [&get_key](auto l, auto r) { return get_key(l) == get_key(r); });
    return std::pair{ data, last - data.begin() };
}
template<auto Init>
constexpr auto construct_unique_map() {
    constexpr auto data = Init.first;
    constexpr auto size = Init.second;
    std::array<std::remove_cvref_t<decltype(data[0])>, size> res{};
    std::copy(data.begin(), data.begin() + size, res.begin());
    return res;
}
} // namespace constexpr_map
