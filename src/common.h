#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <spdlog/spdlog.h>
#include <stb_image.h>

#include <filesystem>
#include <map>
#include <memory>
#include <vector>

namespace peculiar
{

namespace fs = std::filesystem;

template<typename K, typename V>
using map_t = std::map<K, V>;

template<typename T>
using ref_ptr_t = std::shared_ptr<T>;

template<typename T, typename... Args>
ref_ptr_t<T> make_ref(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T>
using vector_t = std::vector<T>;

}
