/**
 * @file         liblava-stage/examples.hpp
 * @brief        Example stages
 * @authors      Lava Block OÜ and contributors
 * @copyright    Copyright (c) 2018-present, MIT License
 */

#include "liblava/lava.hpp"

using namespace lava;

//-----------------------------------------------------------------------------
LAVA_STAGE(7, "forward shading") {
    frame frame(argh);
    if (!frame.ready())
        return error::not_ready;

    window window;
    if (!window.create())
        return error::create_failed;

    input input;
    window.assign(&input);

    input.key.listeners.add([&](key_event::ref event) {
        if (event.pressed(key::escape))
            return frame.shut_down();

        return input_ignore;
    });

    device::ptr device = frame.platform.create_device();
    if (!device)
        return error::create_failed;

    render_target::s_ptr render_target = create_target(&window, device);
    if (!render_target)
        return error::create_failed;

    forward_shading shading;
    if (!shading.create(render_target))
        return error::create_failed;

    render_pass::s_ptr render_pass = shading.get_pass();

    block block;
    if (!block.create(device,
                      render_target->get_frame_count(),
                      device->graphics_queue().family))
        return error::create_failed;

    block.add_command([&](VkCommandBuffer cmd_buf) {
        render_pass->set_clear_color({random(1.f),
                                      random(1.f),
                                      random(1.f)});

        render_pass->process(cmd_buf, block.get_current_frame());
    });

    renderer renderer;
    if (!renderer.create(render_target->get_swapchain()))
        return error::create_failed;

    frame.add_run([&](id::ref run_id) {
        input.handle_events();

        if (window.close_request())
            return frame.shut_down();

        if (window.resize_request())
            return window.handle_resize();

        if (window.iconified()) {
            frame.set_wait_for_events(true);
            return run_continue;
        } else {
            if (frame.waiting_for_events())
                frame.set_wait_for_events(false);
        }

        optional_index current_frame = renderer.begin_frame();
        if (!current_frame.has_value())
            return run_continue;

        if (!block.process(*current_frame))
            return run_abort;

        return renderer.end_frame(block.collect_buffers());
    });

    frame.add_run_end([&]() {
        block.destroy();
        shading.destroy();

        renderer.destroy();
        render_target->destroy();
    });

    return frame.run();
}

//-----------------------------------------------------------------------------
LAVA_STAGE(8, "gamepad") {
    frame frame(argh);
    if (!frame.ready())
        return error::not_ready;

    gamepad_manager::singleton().add([&](gamepad pad, bool active) {
        ui32 id = pad.get_id();

        if (active)
            logger()->info("gamepad: {} - active ({})", id, pad.get_name());
        else
            logger()->info("gamepad: {} - inactive", id);

        return input_ignore;
    });

    for (gamepad& pad : gamepads())
        logger()->info("gamepad: {} - active ({})", pad.get_id(), pad.get_name());

    logger()->info("Waiting some seconds for gamepads...");

    frame.add_run([&](id::ref run_id) {
        sleep(one_second);

        if (frame.get_running_time_sec() > 10.)
            return frame.shut_down();

        return run_continue;
    });

    return frame.run();
}
