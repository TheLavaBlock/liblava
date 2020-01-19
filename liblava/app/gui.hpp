// file      : liblava/app/gui.hpp
// copyright : Copyright (c) 2018-present, Lava Block OÜ
// license   : MIT; see accompanying LICENSE file

#pragma once

#include <liblava/frame/input.hpp>
#include <liblava/block/pipeline.hpp>
#include <liblava/resource/texture.hpp>

// fwd
struct GLFWwindow;
struct GLFWcursor;

namespace lava {

constexpr const r32 default_font_size = 18.f;

struct gui : input_callback {

    explicit gui() = default;
    explicit gui(GLFWwindow* window) { setup(window); }
    ~gui() { destroy(); }

    struct icon_font {

        data font_data;

        ui16 range_begin = 0;
        ui16 range_end = 0;

        r32 size = default_font_size;
    };

    struct config {

        data font_data;
        r32 font_size = default_font_size;

        icon_font icon;
    };

    void setup(GLFWwindow* window, config config);
    void setup(GLFWwindow* window) { setup(window, config()); }

    bool create(graphics_pipeline::ptr pipeline, index max_frames);
    bool create(device_ptr device, index max_frames) {
        
        return create(make_graphics_pipeline(device), max_frames);
    }
    bool create(device_ptr device, index max_frames, VkRenderPass pass) {

        if (!create(device, max_frames))
            return false;

        return pipeline->create(pass);
    }

    bool upload_fonts(texture::ptr texture);
    
    void destroy();

    bool is_initialized() const { return initialized; }
    graphics_pipeline::ptr get_pipeline() { return pipeline; }

    using draw_func = std::function<void()>;
    draw_func on_draw;

    bool want_capture_mouse() const;

    void set_active(bool value = true) { active = value; }
    bool is_active() const { return active; }

    void toggle() { active = !active; }

private:
    void handle_key_event(i32 key, i32 scancode, i32 action, i32 mods);
    void handle_mouse_button_event(i32 button, i32 action, i32 mods);
    void handle_scroll_event(r64 x_offset, r64 y_offset);

    void render_draw_lists(VkCommandBuffer cmd_buf);
    void invalidate_device_objects();

    void update_mouse_pos_and_buttons();
    void update_mouse_cursor();

    void new_frame();
    void render(VkCommandBuffer cmd_buf);

    device_ptr device = nullptr;
    bool initialized = false;

    graphics_pipeline::ptr pipeline;
    lava::pipeline_layout::ptr pipeline_layout;

    size_t buffer_memory_alignment = 256;
    index frame = 0;
    index max_frames = 4;

    buffer::list vertex_buffers;
    buffer::list index_buffers;

    descriptor::ptr descriptor_set_layout;
    VkDescriptorSet descriptor_set = 0;

    GLFWwindow* window = nullptr;

    bool mouse_just_pressed[5] = { false, false, false, false, false };
    r64 current_time = 0.0;

    std::vector<GLFWcursor*> mouse_cursors;

    bool active = true;
};

} // lava
