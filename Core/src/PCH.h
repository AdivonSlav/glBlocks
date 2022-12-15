#pragma once

#define GLFW_INCLUDE_NONE
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// STL
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <string>
#include <sstream>
#include <chrono>
#include <fstream>
#include <iostream>
#include <filesystem>
#include <cmath>
#include <random>
#include <iterator>
#include <future>
#include <deque>
#include <queue>
#include <thread>
#include <mutex>

// GLM
#define GLM_FORCE_SWIZZLE
#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#define GLM_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

// STB
#include <stb/stb_image.h>

// ImGui
#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw.h"
#include "vendor/imgui/imgui_impl_opengl3.h"

// Core utils
#include "utils/Logger.h"
#include "utils/Maths.h"
#include "utils/Timer.h"