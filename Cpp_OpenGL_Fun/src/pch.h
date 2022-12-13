#pragma once

#ifndef OPENGLFUN_PCH_H
#define OPENGLFUN_PCH_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#pragma warning(push)
#pragma warning(disable : 4201)
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>
#pragma warning(pop)

#include <rapidjson/document.h>

#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <queue>
#include <set>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>

#include "Exceptions.h"
#include "Types.h"
#endif