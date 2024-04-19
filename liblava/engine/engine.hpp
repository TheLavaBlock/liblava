/**
 * @file         liblava/engine/engine.hpp
 * @brief        Engine
 * @authors      Lava Block OÜ and contributors
 * @copyright    Copyright (c) 2018-present, MIT License
 */

#pragma once

#include "liblava/app/app.hpp"
#include "liblava/engine/producer.hpp"
#include "liblava/engine/property.hpp"

namespace lava {

/// config
constexpr name _props_ = "props";

/**
 * @brief Engine
 */
struct engine : app {
    // Pointer to engine
    using ptr = engine*;

    /// App constructors
    using app::app;

    /**
     * @brief Set up the engine
     * @return Setup was successful or failed
     */
    bool setup() override;

    /// Props master
    property props;

    /// Producer
    producer producer;

private:
    /**
     * @brief Handle configuration file
     */
    void handle_config();

    /// Configuration file callback
    json_file::callback config_callback;
};

} // namespace lava
