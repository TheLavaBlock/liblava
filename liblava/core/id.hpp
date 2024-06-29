/**
 * @file         liblava/core/id.hpp
 * @brief        Object Identification
 * @authors      Lava Block OÜ and contributors
 * @copyright    Copyright (c) 2018-present, MIT License
 */

#pragma once

#include "liblava/core/types.hpp"
#include <atomic>
#include <deque>
#include <memory>
#include <mutex>
#include <set>

namespace lava {

/**
 * @brief Identification
 */
struct id {
    /// Reference to id
    using ref = id const&;

    /// Set of ids
    using set = std::set<id>;

    /// Reference to set of ids
    using set_ref = set const&;

    /// List if ids
    using list = std::vector<id>;

    /// Map of ids
    using map = std::map<id, id>;

    /// Index map by ids
    using index_map = std::map<id, index>;

    /// String map by ids
    using string_map = std::map<id, string>;

    /**
     * @brief Construct a new id
     */
    id() = default;

    /**
     * @brief Construct a new id
     * @param value    Value of id
     */
    id(index value)
    : value(value) {}

    /// Value
    index value = no_index;

    /**
     * @brief Check if the id is valid
     * @return Id is valid or not
     */
    bool valid() const {
        return value != no_index;
    }

    /**
     * @brief Convert the id to string
     * @return string    String representation of id
     */
    string to_string() const {
        return std::to_string(value);
    }

    /**
     * @brief Invalidate id
     */
    void invalidate() {
        *this = {};
    }

    /**
     * @brief Compare operator
     */
    auto operator<=>(id const&) const = default;
};

/// Map of string ids
using string_id_map = std::map<string, id>;

/// Undefined id
constexpr id const undef_id = id();

/**
 * @brief Convert to id
 * @param value    Source value
 * @return id      Converted value
 */
inline id to_id(auto value) {
    return {static_cast<index>(value)};
}

/**
 * @brief Id factory
 */
struct ids {
    /**
     * @brief Get id factory instance
     * @return ids&    Id factory
     */
    static ids& instance() {
        static ids ids;
        return ids;
    }

    /**
     * @brief Get next id from factory
     * @return id    Next id
     */
    id next() {
        return {++m_next};
    }

private:
    /// Next id
    std::atomic<index> m_next = {no_index};
};

/**
 * @brief Add object to id map
 * @tparam T        Type of object
 * @param object    Object to add
 * @param map       Target map
 * @return id       Id of object in map
 */
template <typename T>
inline id add_id_map(T const& object,
                     std::map<id, T>& map) {
    auto next = ids::instance().next();
    map.emplace(next, std::move(object));
    return next;
}

/**
 * @brief Remove object from id map
 * @tparam T           Type of object
 * @param object_id    Object to remove
 * @param map          Target map
 * @return Removed object from map or object not found
 */
template <typename T>
inline bool remove_id_map(id::ref object_id,
                          std::map<id, T>& map) {
    if (!map.count(object_id))
        return false;

    map.erase(object_id);

    return true;
}

/**
 * @brief Id listeners
 * @tparam T    Listener
 */
template <typename T>
struct id_listeners {
    /**
     * @brief Add listener to map
     * @param listener    Target listener
     * @return id         Id of listener
     */
    id add(typename T::func const& listener) {
        return add_id_map(listener, m_list);
    }

    /**
     * @brief Remove listener from map by id
     * @param id    Id of listener
     */
    void remove(id& id) {
        if (remove_id_map(id, m_list))
            id.invalidate();
    }

    /**
     * @brief Get the list
     * @return T::listeners const&    List of listeners
     */
    typename T::listeners const& get_list() const {
        return m_list;
    }

private:
    /// List of listeners
    typename T::listeners m_list = {};
};

/**
 * @brief Entity
 */
struct entity : no_copy_no_move, interface {
    /**
     * @brief Construct a new entity
     */
    entity()
    : m_id(ids::instance().next()) {}

    /**
     * @brief Get the id of entity
     * @return id::ref    Entity id
     */
    id::ref get_id() const {
        return m_id;
    }

private:
    /// Entity id
    id m_id;
};

/**
 * @brief Id registry
 * @tparam T       Type of objects hold in registry
 * @tparam Meta    Meta type for object
 */
template <typename T, typename Meta>
struct id_registry {
    /// Shared pointer to id registry
    using s_ptr = std::shared_ptr<T>;

    /// Map of id registries
    using s_map = std::map<id, s_ptr>;

    /// Map of ids with meta
    using meta_map = std::map<id, Meta>;

    /**
     * @brief Create a new object in registry
     * @param info    Meta information
     * @return id     Object id
     */
    id create(Meta info = {}) {
        auto object = std::make_shared<T>();
        add(object, info);

        return object->get_id();
    }

    /**
     * @brief Add a object with meta to registry
     * @param object    Object to add
     * @param info      Meta of object
     */
    void add(s_ptr object,
             Meta info = {}) {
        m_objects.emplace(object->get_id(), object);
        m_meta.emplace(object->get_id(), info);
    }

    /**
     * @brief Check if object exists in registry
     * @param object_id    Object to check
     * @return Object exists or not
     */
    bool exists(id::ref object_id) const {
        return m_objects.count(object_id);
    }

    /**
     * @brief Get the object by id
     * @param object_id    Object id
     * @return s_ptr       Shared pointer to object
     */
    s_ptr get(id::ref object_id) const {
        return m_objects.at(object_id);
    }

    /**
     * @brief Get the meta by id
     * @param object_id    Object id
     * @return Meta        Meta object
     */
    Meta const& get_meta(id::ref object_id) const {
        return m_meta.at(object_id);
    }

    /**
     * @brief Get all objects
     * @return s_map const&    Map with objects
     */
    s_map const& get_all() const {
        return m_objects;
    }

    /**
     * @brief Get all meta objects
     * @return meta_map const&    Map with metas
     */
    meta_map const& get_all_meta() const {
        return m_meta;
    }

    /**
     * @brief Update meta of object
     * @param object_id    Object id
     * @param meta         Meta to update
     * @return Meta updated or not
     */
    bool update(id::ref object_id,
                Meta const& meta) {
        if (!exists(object_id))
            return false;

        m_meta.at(object_id) = meta;
        return true;
    }

    /**
     * @brief Remove object from registry
     * @param object_id    Object id
     */
    void remove(id::ref object_id) {
        m_objects.erase(object_id);
        m_meta.erase(object_id);
    }

    /**
     * @brief Clear the registry
     */
    void clear() {
        m_objects.clear();
        m_meta.clear();
    }

private:
    /// Map of objects
    s_map m_objects;

    /// Map of metas
    meta_map m_meta;
};

} // namespace lava
