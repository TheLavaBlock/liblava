/**
 * @file         liblava/app/forward_shading.hpp
 * @brief        Forward shading
 * @authors      Lava Block OÜ and contributors
 * @copyright    Copyright (c) 2018-present, MIT License
 */

#pragma once

#include "liblava/block/render_pass.hpp"
#include "liblava/frame/render_target.hpp"

namespace lava {

/**
 * @brief Forward shading
 */
struct forward_shading {
    /**
     * @brief Construct a new forward shading
     */
    explicit forward_shading() = default;

    /**
     * @brief Destroy the forward shading
     */
    ~forward_shading() {
        destroy();
    }

    /**
     * @brief Create a forward shading for a render target
     * @param target    Render target
     * @return Create was successful or failed
     */
    bool create(render_target::s_ptr target);

    /**
     * @brief Destroy the forward shading
     */
    void destroy();

    /**
     * @brief Get the render pass
     * @return render_pass::s_ptr    Render pass
     */
    render_pass::s_ptr get_pass() const {
        return m_pass;
    }

    /**
     * @brief Get the Vulkan render pass
     * @return VkRenderPass    Vulkan Render pass
     */
    VkRenderPass get_vk_pass() const {
        return m_pass->get();
    }

    /**
     * @brief Get the depth stencil image
     * @return image::s_ptr    Depth stencil Image
     */
    image::s_ptr get_depth_stencil() const {
        return m_depth_stencil;
    }

private:
    /// Render target
    render_target::s_ptr m_target;

    /// Render pass
    render_pass::s_ptr m_pass;

    /// Depth stencil image
    image::s_ptr m_depth_stencil;
};

} // namespace lava
