/**
 * @file         liblava/base/base.hpp
 * @brief        Vulkan base types
 * @authors      Lava Block OÜ and contributors
 * @copyright    Copyright (c) 2018-present, MIT License
 */

#pragma once

#include "liblava/core/version.hpp"
#include "liblava/util/math.hpp"

// clang-format off

#define VK_NO_PROTOTYPES
#include "vulkan/vulkan.h"
#include "volk.h"

// clang-format on

namespace lava {

/// Vulkan version
using VkVersion = ui32;

/// Vulkan object handle
using VkObjectHandle = ui64;

/// List of Vulkan formats
using VkFormats = std::vector<VkFormat>;

/// List of Vulkan images
using VkImages = std::vector<VkImage>;

/// Reference to list of Vulkan images
using VkImagesRef = VkImages const&;

/// List of Vulkan images views
using VkImageViews = std::vector<VkImageView>;

/// List of Vulkan frame buffers
using VkFramebuffers = std::vector<VkFramebuffer>;

/// List of Vulkan command pools
using VkCommandPools = std::vector<VkCommandPool>;

/// List of Vulkan command buffers
using VkCommandBuffers = std::vector<VkCommandBuffer>;

/// List of Vulkan fences
using VkFences = std::vector<VkFence>;

/// List of Vulkan semaphores
using VkSemaphores = std::vector<VkSemaphore>;

/// List of Vulkan present modes
using VkPresentModeKHRs = std::vector<VkPresentModeKHR>;

/// List of Vulkan descriptor sets
using VkDescriptorSets = std::vector<VkDescriptorSet>;

/// List of Vulkan descriptor set layouts
using VkDescriptorSetLayouts = std::vector<VkDescriptorSetLayout>;

/// List of Vulkan descriptor set layout bindings
using VkDescriptorSetLayoutBindings = std::vector<VkDescriptorSetLayoutBinding>;

/// List of Vulkan descriptor pool sizes
using VkDescriptorPoolSizes = std::vector<VkDescriptorPoolSize>;

/// Reference to a list of Vulkan descritpr pool sizes
using VkDescriptorPoolSizesRef = VkDescriptorPoolSizes const&;

/// List of Vulkan push constant ranges
using VkPushConstantRanges = std::vector<VkPushConstantRange>;

/// List of Vulkan attachment references
using VkAttachmentReferences = std::vector<VkAttachmentReference>;

/// List of Vulkan clear values
using VkClearValues = std::vector<VkClearValue>;

/// List of Vulkan pipeline shader stage create infos
using VkPipelineShaderStageCreateInfos = std::vector<VkPipelineShaderStageCreateInfo>;

/// List of Vulkan specialization map entries
using VkSpecializationMapEntries = std::vector<VkSpecializationMapEntry>;

/// List of Vulkan vertex input binding descriptions
using VkVertexInputBindingDescriptions = std::vector<VkVertexInputBindingDescription>;

/// List of Vulkan vertex input attribute descriptions
using VkVertexInputAttributeDescriptions = std::vector<VkVertexInputAttributeDescription>;

/// List of Vulkan pipeline color blend attachment states
using VkPipelineColorBlendAttachmentStates = std::vector<VkPipelineColorBlendAttachmentState>;

/// List of Vulkan pipeline stage flags
using VkPipelineStageFlagsList = std::vector<VkPipelineStageFlags>;

/// List of Vulkan dynamic states
using VkDynamicStates = std::vector<VkDynamicState>;

/// List of Vulkan queue family properties
using VkQueueFamilyPropertiesList = std::vector<VkQueueFamilyProperties>;

/// List of Vulkan extension properties
using VkExtensionPropertiesList = std::vector<VkExtensionProperties>;

/// List of Vulkan layer properties
using VkLayerPropertiesList = std::vector<VkLayerProperties>;

/// List of Vulkan extension properties
using VkExtensionPropertiesList = std::vector<VkExtensionProperties>;

/// List of Vulkan physical devices
using VkPhysicalDevices = std::vector<VkPhysicalDevice>;

/**
 * @brief Check a Vulkan result
 * @param result    Result to check
 * @return Okay or error
 */
bool check(VkResult result);

/**
 * @brief Check if a Vulkan result failed
 * @param result    Result to check
 * @return Error or okay
 */
inline bool failed(VkResult result) {
    return !check(result);
}

/**
 * @brief Convert a Vulkan result to string
 * @param result     Result to convert
 * @return string    String of result
 */
string to_string(VkResult result);

/**
 * @brief Convert a Vulkan version to string
 * @param version    Version to convert
 * @return string    String of version
 */
string vk_version_to_string(VkVersion version);

/**
 * @brief Convert a Vulkan version to semantic version
 * @param version         Vulkan version to convert
 * @return sem_version    Converted semantic version
 */
sem_version to_version(VkVersion version);

/**
 * @brief Convert a semantic version to Vulkan version
 * @param version       Semantic version to convert
 * @return VkVersion    Converted Vulkan version
 */
VkVersion to_vk_version(sem_version version);

/**
 * @brief Vulkan API versions
 */
enum class api_version : index {
    v1_0 = 0,
    v1_1,
    v1_2,
    v1_3
};

/**
 * @brief Convert a Vulkan version to API version
 * @param version         Vulkan version to convert
 * @return api_version    Converted API version
 */
api_version to_api_version(VkVersion version);

/**
 * @brief Vulkan result
 */
struct vk_result {
    /// State of result
    bool state = false;

    /// Value of result
    VkResult value = VK_NOT_READY;

    /**
     * @brief Check result
     * @return Okay or error
     */
    operator bool() {
        return state;
    }
};

/// Build failed
constexpr bool const build_failed = false;

/// Build done
constexpr bool const build_done = true;

/// List of Vulkan attachments (image views)
using VkAttachments = std::vector<VkImageViews>;

/// Reference of Vulkan attachments (image views)
using VkAttachmentsRef = VkAttachments const&;

/**
 * @brief Target callback
 */
struct target_callback {
    /// Const pointer to target callback
    using c_ptr = target_callback const*;

    /// List of target callbacks
    using list = std::vector<target_callback*>;

    /// Const list of target callbacks
    using c_list = std::vector<c_ptr>;

    /// Created function
    using created_func = std::function<bool(VkAttachmentsRef, rect::ref)>;

    /// Called on target created
    created_func on_created;

    /// Destroy function
    using destroyed_func = std::function<void()>;

    /// Called on target destroyed
    destroyed_func on_destroyed;
};

/// Limit of Vulkan description sets
constexpr ui32 const Vk_Limit_DescriptorSets = 4;

/// Limit of Vulkan bindings
constexpr ui32 const Vk_Limit_Bindings = 16;

/// Limit of Vulkan attachments
constexpr ui32 const Vk_Limit_Attachments = 8;

/// Limit of Vulkan vertex attributes
constexpr ui32 const Vk_Limit_VertexAttribs = 16;

/// Limit of Vulkan vertex buffers
constexpr ui32 const Vk_Limit_VertexBuffers = 4;

/// Limit of Vulkan push constant size
constexpr ui32 const Vk_Limit_PushConstant_Size = 128;

/// Limit of Vulkan UBO size
constexpr ui32 const Vk_Limit_UBO_Size = 16 * 1024;

} // namespace lava
