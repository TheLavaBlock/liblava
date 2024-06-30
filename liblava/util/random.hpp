/**
 * @file         liblava/util/random.hpp
 * @brief        Random generator
 * @authors      Lava Block OÜ and contributors
 * @copyright    Copyright (c) 2018-present, MIT License
 */

#pragma once

#include "liblava/core/types.hpp"
#include <random>

namespace lava {

/**
 * @brief Random generator
 */
struct random_generator {
    /**
     * @brief Construct a new random generator
     */
    random_generator() {
        std::random_device rd;
        m_engine = std::mt19937(rd());
    }

    /**
     * @brief Get next random number
     * @param low     Lowest number
     * @param high    Highest number
     * @return i32    Random number
     */
    i32 get(i32 low, i32 high) {
        std::uniform_int_distribution<i32> dist(low, high);
        return dist(m_engine);
    }

    /**
     * @brief Get next real random number
     * @tparam T      Type of number
     * @param low     Lowest number
     * @param high    Highest number
     * @return T      Random number
     */
    template <typename T = real>
    T get(T low, T high) {
        std::uniform_real_distribution<T> dist(low, high);
        return dist(m_engine);
    }

private:
    /// Mersenne twister algorithm
    std::mt19937 m_engine;
};

/**
 * @brief Get next random number
 * @param low      Lowest number
 * @param high     Highest number
 * @return auto    Random number
 */
inline auto random(auto low, auto high) {
    return random_generator().get(low, high);
}

/**
 * @brief Get next random number (lowest is 0)
 * @param high     Highest number
 * @return auto    Random number
 */
inline auto random(auto high) {
    return random_generator().get({}, high);
}

/**
 * @brief Pseudorandom generator
 */
struct pseudorandom_generator {
    /**
     * @brief Construct a new pseudorandom generator
     * @param seed    Seed for generator
     */
    explicit pseudorandom_generator(ui32 seed)
    : m_seed(seed) {}

    /**
     * @brief Set the seed
     * @param value    Generator seed
     */
    void set_seed(ui32 value) {
        m_seed = value;
    }

    /**
     * @brief Get next pseudorandom number
     * @return ui32    Random number
     */
    ui32 get() {
        return generate_fast() ^ (generate_fast() >> 7);
    }

private:
    /// Generator seed
    ui32 m_seed = 0;

    /**
     * @brief Generate fast random number
     * @return ui32    Random number
     */
    ui32 generate_fast() {
        return m_seed = (m_seed * 196314165 + 907633515);
    }
};

} // namespace lava
