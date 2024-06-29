/**
 * @file         liblava/engine/props.cpp
 * @brief        Props
 * @authors      Lava Block OÜ and contributors
 * @copyright    Copyright (c) 2018-present, MIT License
 */

#include "liblava/engine/props.hpp"
#include "liblava/base/base.hpp"
#include "liblava/engine/engine.hpp"
#include "liblava/file/file_system.hpp"

namespace lava {

//-----------------------------------------------------------------------------
void props::add(string_ref name,
                string_ref filename) {
    m_map.emplace(name, filename);

    logger()->trace("prop: {} = {}", name, filename);
}

//-----------------------------------------------------------------------------
void props::remove(string_ref name) {
    if (!m_map.count(name))
        return;

    m_map.erase(name);
}

//-----------------------------------------------------------------------------
bool props::install(string_ref name,
                    string_ref filename) {
    add(name, filename);
    return load(name);
}

//-----------------------------------------------------------------------------
c_data props::operator()(string_ref name) {
    auto& prop = m_map.at(name);
    if (prop.data.addr)
        return {prop.data.addr, prop.data.size};

    if (!load_file_data(prop.filename, prop.data)) {
        logger()->error("prop get: {} = {}",
                        name, prop.filename);
        return {};
    }

    return {prop.data.addr, prop.data.size};
}

//-----------------------------------------------------------------------------
bool props::check() {
    auto result = true;

    for (auto& [name, prop] : m_map) {
        if (!app->fs.exists(prop.filename)) {
            logger()->warn("prop missing: {} = {}",
                           name, prop.filename);

            result = false;
        }
    }

    return result;
}

//-----------------------------------------------------------------------------
bool props::load(string_ref name) {
    auto& prop = m_map.at(name);
    if (prop.data.addr)
        prop.data = {}; // reload

    if (!load_file_data(prop.filename, prop.data)) {
        logger()->error("prop load: {} = {}",
                        name, prop.filename);
        return false;
    }

    return true;
}

//-----------------------------------------------------------------------------
bool props::load_all() {
    for (auto& [name, prop] : m_map) {
        if (!load_file_data(name, prop.data)) {
            logger()->error("prop load (all): {} = {}",
                            name, prop.filename);
            return false;
        }
    }

    return true;
}

//-----------------------------------------------------------------------------
void props::parse(cmd_line cmd_line) {
    for (auto& [name, prop] : m_map) {
        auto cmd_arg = "--" + name;

        auto filename = get_cmd(cmd_line, {cmd_arg.c_str()});
        if (!filename.empty()) {
            prop.filename = filename;

            logger()->debug("prop parse: {} = {}",
                            name, filename);
        }
    }
}

//-----------------------------------------------------------------------------
void props::set_json(json_ref j) {
    for (auto& [name, prop] : m_map) {
        if (j.count(name)) {
            string filename = j[name];

            if (prop.filename == filename)
                continue;

            prop.filename = filename;

            logger()->debug("prop config: {} = {}",
                            name, filename);
        }
    }
}

//-----------------------------------------------------------------------------
json props::get_json() const {
    json j;

    for (auto& [name, prop] : m_map)
        j[name] = prop.filename;

    return j;
}

} // namespace lava
