/*
Copyright (C) 2018 Rodrigo Jose Hernandez Cordoba

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

#include <gl/gl.h>
#include <gl/glext.h>
#include <iostream>

#include "OpenGLFunctions.h"
#include "Sandbox.h"

namespace AeonDE
{
Sandbox::Sandbox(int argc, char **argv)
{
    PlatformDependentInitialize(argc, argv);
    if (!LoadOpenGLAPI())
    {
        throw std::runtime_error("Error while loading OpenGL functions.");
    }
    glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
Sandbox::~Sandbox()
{
    PlatformDependentFinalize();
}

void Sandbox::OnSize(uint32_t aWidth, uint32_t aHeight)
{
    std::cout << "Resize " << aWidth << ", " << aHeight << std::endl;
    if (aWidth && aHeight)
    {
        glViewport(0, 0, aWidth, aHeight);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}

int Sandbox::Run()
{
    return PlatformDependentRun();
}

void *Sandbox::GetOpenGLContext() const
{
    return mOpenGLContext;
}

void Sandbox::Loop()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

} // namespace AeonDE
