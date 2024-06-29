/**
 * @file         liblava/frame/render_target.hpp
 * @brief        Render target
 * @authors      Lava Block OÜ and contributors
 * @copyright    Copyright (c) 2018-present, MIT License
 */

#pragma once

#include "liblava/core/misc.hpp"
#include "liblava/frame/swapchain.hpp"
#include "liblava/fwd.hpp"
#include "liblava/resource/format.hpp"

namespace lava {

/**
 * @brief Render target
 */
struct render_target : entity {
    /// Shared pointer to render target
    using s_ptr = std::shared_ptr<render_target>;

    /**
     * @brief Make a new render target
     * @return s_ptr    Shared pointer to render target
     */
    static s_ptr make() {
        return std::make_shared<render_target>();
    }

    /**
     * @brief Create a new render target
     * @param device           Vulkan device
     * @param surface          Vulkan surface
     * @param format           Surface format
     * @param size             Size of target
     * @param v_sync           V-Sync enabled
     * @param triple_buffer    VK_PRESENT_MODE_MAILBOX_KHR preferred over VK_PRESENT_MODE_IMMEDIATE_KHR
     * @return Create was successful or failed
     */
    bool create(device::ptr device,
                VkSurfaceKHR surface,
                VkSurfaceFormatKHR format,
                uv2 size,
                bool v_sync = false,
                bool triple_buffer = true);

    /**
     * @brief Destroy the render target
     */
    void destroy();

    /**
     * @brief Get the size of the render target
     * @return uv2    Size of render target
     */
    uv2 get_size() const {
        return m_target.get_size();
    }

    /**
     * @brief Resize the render target
     * @param new_size    New render target size
     * @return Resize was successful or failed
     */
    bool resize(uv2 new_size) {
        return m_target.resize(new_size);
    }

    /**
     * @brief Get the frame count
     * @return ui32    Number of frames
     */
    ui32 get_frame_count() const {
        return m_target.get_backbuffer_count();
    }

    /**
     * @brief Check if render target requests a reload
     * @return Request reload or not
     */
    bool reload_request() const {
        return m_target.reload_request();
    }

    /**
     * @brief Reload the render target
     */
    void reload() {
        m_target.resize(m_target.get_size());
    }

    /**
     * @brief Get the device
     * @return device::ptr    Vulkan device
     */
    device::ptr get_device() {
        return m_target.get_device();
    }

    /**
     * @brief Get the swapchain
     * @return swapchain*    Target swapchain
     */
    swapchain* get_swapchain() {
        return &m_target;
    }

    /**
     * @brief Get the format
     * @return VkFormat    Target format
     */
    VkFormat get_format() const {
        return m_target.get_format();
    }

    /**
     * @brief Get the backbuffers
     * @return image::s_list const&    List of backbuffer images
     */
    image::s_list const& get_backbuffers() const {
        return m_target.get_backbuffers();
    }

    /**
     * @brief Get the backbuffer by frame index
     * @param index            Frame index
     * @return image::s_ptr    Backbuffer image
     */
    inline image::s_ptr get_backbuffer(index index) {
        auto& backbuffers = get_backbuffers();
        if (index >= backbuffers.size())
            return nullptr;

        return backbuffers.at(index);
    }

    /**
     * @brief Get the backbuffer image by index
     * @param index       Frame index
     * @return VkImage    Vulkan image
     */
    inline VkImage get_backbuffer_image(index index) {
        auto result = get_backbuffer(index);
        return result ? result->get() : 0;
    }

    /**
     * @see get_backbuffer_image
     */
    inline VkImage get_image(index index) {
        return get_backbuffer_image(index);
    }

    /**
     * @brief Add callback
     * @param callback    Target callback
     */
    void add_callback(target_callback::c_ptr callback) {
        m_target_callbacks.push_back(callback);
    }

    /**
     * @brief Remove callback
     * @param callback    Target callback
     */
    void remove_callback(target_callback::c_ptr callback) {
        remove(m_target_callbacks, callback);
    }

    /// Swapchain start function
    using swapchain_start_func = std::function<bool()>;

    /// Called on swapchain start
    swapchain_start_func on_swapchain_start;

    /// Swapchain stop function
    using swapchain_stop_func = std::function<void()>;

    /// Called on swapchain stop
    swapchain_stop_func on_swapchain_stop;

    /// Create attachments function
    using create_attachments_func = std::function<VkAttachments()>;

    /// Called on create attachments
    create_attachments_func on_create_attachments;

    /// Destroy attachments function
    using destroy_attachments_func = std::function<void()>;

    /// Called on destroy attachments
    destroy_attachments_func on_destroy_attachments;

private:
    /// Target swapchain
    swapchain m_target;

    /// Swapchain callback
    swapchain::callback m_swapchain_callback;

    /// List of target callbacks
    target_callback::c_list m_target_callbacks;
};

/**
 * @brief Create a new render target
 * @param window                   Target window
 * @param device                   Vulkan device
 * @param v_sync                   V-Sync enabled
 * @param triple_buffer            VK_PRESENT_MODE_MAILBOX_KHR preferred over VK_PRESENT_MODE_IMMEDIATE_KHR
 * @param request                  Surface format request
 * @return render_target::s_ptr    Shared pointer to render target
 */
render_target::s_ptr create_target(window* window,
                                   device::ptr device,
                                   bool v_sync = false,
                                   bool triple_buffer = true,
                                   surface_format_request request = {});

/**
 * @brief Create a new render target with V-Sync enabled
 * @param window                   Target window
 * @param device                   Vulkan device
 * @param request                  Surface format request
 * @return render_target::s_ptr    Shared pointer to render target
 */
inline render_target::s_ptr create_target_v_sync(window* window,
                                                 device::ptr device,
                                                 surface_format_request request = {}) {
    return create_target(window,
                         device,
                         true,
                         true,
                         request);
}

/**
 * @brief Create a new render target that prefers VK_PRESENT_MODE_IMMEDIATE_KHR over VK_PRESENT_MODE_MAILBOX_KHR
 * @param window                   Target window
 * @param device                   Vulkan device
 * @param request                  Surface format request
 * @return render_target::s_ptr    Shared pointer to render target
 */
inline render_target::s_ptr create_target_no_triple_buffer(window* window,
                                                           device::ptr device,
                                                           surface_format_request request = {}) {
    return create_target(window,
                         device,
                         false,
                         false,
                         request);
}

} // namespace lava
