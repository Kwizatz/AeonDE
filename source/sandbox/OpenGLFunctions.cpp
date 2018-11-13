/*
Copyright (C) 2016-2018 Rodrigo Jose Hernandez Cordoba

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/
#include "OpenGLFunctions.h"
#include <cstring>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#ifdef WIN32
#include <gl/wglext.h>
#define GLGETPROCADDRESS(glFunctionType, glFunction)                      \
    if (glFunction == nullptr)                                            \
    {                                                                     \
        glFunction = (glFunctionType)wglGetProcAddress(#glFunction);      \
        if (glFunction == nullptr)                                        \
        {                                                                 \
            std::ostringstream stream;                                    \
            stream << "OpenGL: Unable to load " #glFunction " function."; \
            throw std::runtime_error(stream.str().c_str());               \
        }                                                                 \
    }
#elif defined(ANDROID)
#define GLGETPROCADDRESS(glFunctionType, glFunction)                                     \
    if (glFunction == nullptr)                                                           \
    {                                                                                    \
        glFunction = (glFunctionType)eglGetProcAddress((const char *)#glFunction "OES"); \
        if (glFunction == nullptr)                                                       \
        {                                                                                \
            std::ostringstream stream;                                                   \
            stream << "OpenGL: Unable to load " #glFunction " function.";                \
            throw std::runtime_error(stream.str().c_str());                              \
        }                                                                                \
    }
#else
#include <GL/gl.h>
#include <GL/glx.h>
#define GLGETPROCADDRESS(glFunctionType, glFunction)                                  \
    if (glFunction == nullptr)                                                        \
    {                                                                                 \
        glFunction = (glFunctionType)glXGetProcAddress((const GLubyte *)#glFunction); \
        if (glFunction == nullptr)                                                    \
        {                                                                             \
            std::ostringstream stream;                                                \
            stream << "OpenGL: Unable to load " #glFunction " function.";             \
            throw std::runtime_error(stream.str().c_str());                           \
        }                                                                             \
    }
#endif

namespace AeonDE
{
#include "glDefinitions.h"
bool LoadOpenGLAPI()
{
#include "glAssignments.h"
    return true;
}
} // namespace AeonDE
