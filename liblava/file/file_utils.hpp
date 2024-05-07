/**
 * @file         liblava/file/file_utils.hpp
 * @brief        File utilities
 * @authors      Lava Block OÜ and contributors
 * @copyright    Copyright (c) 2018-present, MIT License
 */

#pragma once

#include "liblava/core/data.hpp"

namespace lava {

/**
 * @brief Read data from file
 * @param out         File data
 * @param filename    Name of file
 * @return Read was successful or failed
 */
bool read_file(std::vector<char>& out, string_ref filename);

/**
 * @brief Write data to file
 * @param filename     Name of file
 * @param data         Data to write
 * @param data_size    Size of data
 * @return Write was successful or failed
 */
bool write_file(string_ref filename,
                char const* data,
                size_t data_size);

/**
 * @brief Check extension of file
 * @param filename     Name of file
 * @param extension    Extension to check
 * @return Extension found or not
 */
bool extension(string_ref filename, string_ref extension);

/**
 * @brief Check extensions of file
 * @param filename      Name of file
 * @param extensions    List of extensions to check
 * @return Extension found or not
 */
bool extension(string_ref filename,
               string_list_ref extensions);

/**
 * @brief Get the file name from path
 * @param path              Target path
 * @param with_extension    Include extension in file name
 * @return string           File name
 */
string get_filename_from(string_ref path,
                         bool with_extension = false);

/**
 * @brief Remove existing path
 * @param target    Target path
 * @param path      Path to remove
 * @return Remove was successful or failed
 */
bool remove_existing_path(string& target,
                          string_ref path);

/**
 * @brief Load file data
 * @param filename    Name of file
 * @param target      Target data
 * @return Load was successful or failed
 */
bool load_file_data(string_ref filename,
                    data& target);

/**
 * @brief File data
 */
struct file_data : u_data {
    /// Reference to file data
    using ref = file_data const&;

    /// Unique data constructors
    using u_data::u_data;

    /**
     * @brief Construct a new file data
     * @param filename    Name of file
     */
    explicit file_data(string_ref filename)
    : filename(filename) {
        load_file_data(filename, *this);
    }

    /// Name of file
    string filename;
};

/**
 * @brief File delete guard
 */
struct file_delete : no_copy_no_move {
    /**
     * @brief Construct a new file delete
     * @param filename    Name of file
     */
    explicit file_delete(string filename = "")
    : filename(filename) {}

    /**
     * @brief Destroy the file delete
     */
    ~file_delete();

    /// Name of file
    string filename;

    /// Active state
    bool active = true;
};

} // namespace lava
