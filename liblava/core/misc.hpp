/**
 * @file         liblava/core/misc.hpp
 * @brief        Miscellaneous helpers
 * @authors      Lava Block OÜ and contributors
 * @copyright    Copyright (c) 2018-present, MIT License
 */

#pragma once

#include "liblava/core/types.hpp"
#include <algorithm>
#include <cstring>
#include <utility>

namespace lava {

/**
 * @brief Check if name exists in name list
 * @param list      List of names
 * @param item      Item to check
 * @return Item exists or not found
 */
inline bool exists(names_ref list, name item) {
    auto itr = std::find_if(list.begin(), list.end(),
                            [&](name entry) { return strcmp(entry, item) == 0; });
    return itr != list.end();
}

/**
 * @brief Remove item from list
 * @tparam T      Type of list
 * @param list    List of items
 * @param item    Item to remove
 */
template <typename T>
inline void remove(std::vector<T>& list, T item) {
    list.erase(std::remove(list.begin(), list.end(), item), list.end());
}

/**
 * @brief Check if item is included in list
 * @tparam T        Type of list
 * @param list      List of items
 * @param item      Item to check
 * @return Item exists or not found
 */
template <typename T>
inline bool contains(std::vector<T>& list, T item) {
    return std::find(list.begin(), list.end(), item) != list.end();
}

/**
 * @brief Append a list of items to another list
 * @tparam T       Type of list
 * @param list     List of items
 * @param items    Items to append
 */
template <typename T>
inline void append(std::vector<T>& list, std::vector<T>& items) {
    list.insert(list.end(), items.begin(), items.end());
}

/**
 * @brief Trim string only from start (in place)
 * @param s    String to trim
 */
inline void trim_start(string& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](uchar ch) {
                return !std::isspace(ch);
            }));
}

/**
 * @brief Trim string only from end (in place)
 * @param s    String to trim
 */
inline void trim_end(string& s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](uchar ch) {
                return !std::isspace(ch);
            }).base(),
            s.end());
}

/**
 * @brief Trim string from both ends (in place)
 * @param s    String to trim
 */
inline void trim(string& s) {
    trim_start(s);
    trim_end(s);
}

/**
 * @brief Trim string only from start (copying)
 * @param s          String to trim
 * @return string    Trimmed string
 */
inline string trim_start_copy(string s) {
    trim_start(s);
    return s;
}

/**
 * @brief Trim string only from end (copying)
 * @param s          String to trim
 * @return string    Trimmed string
 */
inline string trim_end_copy(string s) {
    trim_end(s);
    return s;
}

/**
 * @brief Trim string from both ends (copying)
 * @param s          String to trim
 * @return string    Trimmed string
 */
inline string trim_copy(string s) {
    trim(s);
    return s;
}

/**
 * @brief Remove chars in string
 * @param s           Target string
 * @param chars       Chars to remove
 * @return string&    Cleared string
 */
inline string& remove_chars(string& s, string_ref chars) {
    s.erase(std::remove_if(s.begin(), s.end(), [&chars](name_ref c) {
                return chars.find(c) != string::npos;
            }),
            s.end());
    return s;
}

/// Punctuation marks
constexpr name g_punctuation_marks_ = "\"\'";

/**
 * @brief Remove punctuation marks in string
 * @param s           Target string
 * @return string&    Cleared string
 */
inline string& remove_punctuation_marks(string& s) {
    return remove_chars(s, g_punctuation_marks_);
}

/**
 * @brief Remove chars in string (copying)
 * @param s          Target string
 * @param chars      Chars to remove
 * @return string    Cleared string
 */
inline string remove_chars_copy(string s, string_ref chars) {
    return remove_chars(s, chars);
}

/**
 * @brief Remove all non digit chars in string
 * @param s           Target string
 * @return string&    Cleared string
 */
inline string& remove_nondigit(string& s) {
    s.erase(std::remove_if(s.begin(), s.end(), [](name_ref c) {
                return !isdigit(c);
            }),
            s.end());
    return s;
}

/**
 * @brief Remove all non digit chars in string (copying)
 * @param s          Target string
 * @return string    Cleared string
 */
inline string remove_nondigit_copy(string s) {
    return remove_nondigit(s);
}

/**
 * @brief Remove all chars in string which are not allowed
 * @param s           Target string
 * @param allowed     Allowed chars
 * @return string&    Cleared string
 */
inline string& remove_chars_if_not(string& s, string_ref allowed) {
    s.erase(std::remove_if(s.begin(), s.end(), [&allowed](name_ref c) {
                return allowed.find(c) == string::npos;
            }),
            s.end());
    return s;
}

/**
 * @brief Remove all chars in string which are not allowed (copying)
 * @param s          Target string
 * @param allowed    Allowed chars
 * @return string    Cleared string
 */
inline string remove_chars_if_not_copy(string s, string_ref allowed) {
    return remove_chars_if_not(s, allowed);
}

/**
 * @brief Reversion Wrapper
 * @tparam T    Type to iterate
 */
template <typename T>
struct reversion_wrapper {
    /// Iterable to wrap
    T& iterable;
};

/**
 * @brief Begin the iterator
 * @tparam T       Type of iterable
 * @param w        Reversion wrapper
 * @return auto    Iterator
 */
template <typename T>
inline auto begin(reversion_wrapper<T> w) {
    return std::rbegin(w.iterable);
}

/**
 * @brief End the iterator
 * @tparam T       Type of iterable
 * @param w        Reversion wrapper
 * @return auto    Iterator
 */
template <typename T>
inline auto end(reversion_wrapper<T> w) {
    return std::rend(w.iterable);
}

/**
 * @brief Reverse iteration
 * @tparam T                       Type of iterable
 * @param iterable                 Iterable
 * @return reversion_wrapper<T>    Wrapper
 */
template <typename T>
inline reversion_wrapper<T> reverse(T&& iterable) {
    return {iterable};
}

} // namespace lava
