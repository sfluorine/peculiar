#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <spdlog/spdlog.h>

#include <memory>
#include <vector>

namespace peculiar
{

template<typename T>
using vector_t = std::vector<T>;

template<typename T>
using ref_ptr_t = std::shared_ptr<T>;

template<typename T, typename... Args>
ref_ptr_t<T> make_ref(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

}
