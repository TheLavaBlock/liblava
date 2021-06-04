/**
 * @file liblava/util/utility.hpp
 * @brief Container utilites
 * @authors Lava Block OÜ and contributors
 * @copyright Copyright (c) 2018-present, MIT License
 */

#pragma once

#include <algorithm>
#include <cstring>
#include <liblava/core/types.hpp>
#include <utility>

namespace lava {

/**
 * @brief Check if name exists in name list
 * 
 * @param list List of names
 * @param item Item to check
 * @return true Item exists
 * @return false Item not found
 */
inline bool exists(names_ref list, name item) {
    auto itr = std::find_if(list.begin(), list.end(), [&](name entry) { return strcmp(entry, item) == 0; });
    return itr != list.end();
}

/**
 * @brief Remove item from list
 * 
 * @tparam T Type of list
 * @param list List of items
 * @param item Item to remove
 */
template<typename T>
inline void remove(std::vector<T>& list, T item) {
    list.erase(std::remove(list.begin(), list.end(), item), list.end());
}

/**
 * @brief Check if item contains in list
 * 
 * @tparam T Type of list
 * @param list List of items
 * @param item Item to check
 * @return true Item exists
 * @return false Item not found
 */
template<typename T>
inline bool contains(std::vector<T>& list, T item) {
    return std::find(list.begin(), list.end(), item) != list.end();
}

/**
 * @brief Append a list of items to another list
 * 
 * @tparam T Type of list
 * @param list List of items
 * @param items Items to append
 */
template<typename T>
inline void append(std::vector<T>& list, std::vector<T>& items) {
    list.insert(list.end(), items.begin(), items.end());
}

/**
 * @brief Reversion Wrapper
 * 
 * @tparam T Type to iterate
 */
template<typename T>
struct reversion_wrapper {
    /// Iterable to wrap
    T& iterable;
};

/**
 * @brief Begin the iterator
 * 
 * @tparam T Type of iterable
 * @param w Reversion wrapper
 * @return auto Iterator
 */
template<typename T>
inline auto begin(reversion_wrapper<T> w) {
    return std::rbegin(w.iterable);
}

/**
 * @brief End the iterator
 * 
 * @tparam T Type of iterable
 * @param w Reversion wrapper
 * @return auto Iterator
 */
template<typename T>
inline auto end(reversion_wrapper<T> w) {
    return std::rend(w.iterable);
}

/**
 * @brief Reverse iteration
 * 
 * @tparam T Type of iterable
 * @param iterable Iterable
 * @return reversion_wrapper<T> Wrapper
 */
template<typename T>
inline reversion_wrapper<T> reverse(T&& iterable) {
    return { iterable };
}

} // namespace lava
