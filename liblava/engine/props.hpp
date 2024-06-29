/**
 * @file         liblava/engine/props.hpp
 * @brief        Props
 * @authors      Lava Block OÜ and contributors
 * @copyright    Copyright (c) 2018-present, MIT License
 */

#pragma once

#include "liblava/file/file_utils.hpp"
#include "liblava/file/json.hpp"
#include "liblava/frame/argh.hpp"
#include "liblava/fwd.hpp"

namespace lava {

/**
 * @brief Props
 */
struct props : configurable {
    /// Engine
    engine* app = nullptr;

    /**
     * @brief Prop item
     */
    struct item {
        /// Map of items
        using map = std::map<string, item>;

        /**
         * @brief Construct a new prop
         * @param filename    File name of prop
         */
        item(string_ref filename)
        : filename(filename) {}

        /// File name of prop
        string filename;

        /// File data of prop
        file_data data;
    };

    /**
     * @brief Add a prop
     * @param name        Name of prop
     * @param filename    File name of prop
     */
    void add(string_ref name,
             string_ref filename);

    /**
     * @brief Remove a prop
     * @param name    Name of prop
     */
    void remove(string_ref name);

    /**
     * @brief Install a prop (add + load)
     * @param name        Name of prop
     * @param filename    File name of prop
     * @return Install was successful or failed
     */
    bool install(string_ref name,
                 string_ref filename);

    /**
     * @brief Get prop data
     * @param name       Name of prop
     * @return c_data    Prop const data
     */
    c_data operator()(string_ref name);

    /**
     * @brief Get file name of prop
     * @param name           Name of prop
     * @return string_ref    File name
     */
    string_ref get_filename(string_ref name) const {
        return m_map.at(name).filename;
    }

    /**
     * @brief Set filename of prop
     * @param name        Name of prop
     * @param filename    File name
     */
    void set_filename(string_ref name,
                      string_ref filename) {
        m_map.at(name).filename = filename;
    }

    /**
     * @brief Check if prop exists
     * @param name      Name of prop to check
     * @return Prop exists or not
     */
    bool exists(string_ref name) const {
        return m_map.count(name);
    }

    /**
     * @brief Check if prop data is empty
     * @param name      Name of prop
     * @return Prop data is empty or not
     */
    bool empty(string_ref name) const {
        return m_map.at(name).data.addr == nullptr;
    }

    /**
     * @brief Load prop data (reload if loaded)
     * @param name      Name of prop
     * @return Load was successful or failed
     */
    bool load(string_ref name);

    /**
     * @brief Unload prop data
     * @param name      Name of prop
     */
    void unload(string_ref name) {
        m_map.at(name).data = {};
    }

    /**
     * @brief Load all prop data (reload if loaded)
     * @return Load was successful or failed
     */
    bool load_all();

    /**
     * @brief Unload all prop data
     */
    void unload_all() {
        for (auto& [name, prop] : m_map)
            prop.data = {};
    }

    /**
     * @brief Check whether all props are available
     * @return All props are there or are missing (see log)
     */
    bool check();

    /**
     * @brief Parse prop overloads
     * @param cmd_line    Command line arguments
     */
    void parse(cmd_line cmd_line);

    /**
     * @brief Clear all props
     */
    void clear() {
        m_map.clear();
    }

    /**
     * @brief Get all props
     * @return item::map const&    Map of props
     */
    item::map const& get_all() const {
        return m_map;
    }

    /// @see configurable::set_json
    void set_json(json_ref j) override;

    /// @see configurable::get_json
    json get_json() const override;

private:
    /// Map of props
    item::map m_map;
};

} // namespace lava
