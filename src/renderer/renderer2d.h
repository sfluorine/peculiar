#pragma once

#include "common.h"

namespace peculiar
{

namespace renderer2d
{

void init(int32_t width, int32_t height);
void shutdown();

void begin_drawing();
void end_drawing();

void draw_quad(glm::vec2 position, glm::vec2 dimension, glm::vec3 color);

}

}