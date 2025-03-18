#pragma once

#include <array>
#include <vector>
#include <utility>
#include <iostream>
#include <cassert>
#include <initializer_list>

namespace constexpr_map {

template<typename T, size_t N, size_t Size>
constexpr size_t hash0(T t) {
    return (t^N) % Size;
}

template<typename Container, typename T>
concept container = std::same_as<typename Container::value_type, T>;

template<class T, class U, size_t Size, size_t MaxSize=Size*2, size_t N = 0, size_t MaxN = 100>
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

} // namespace constexpr_map
