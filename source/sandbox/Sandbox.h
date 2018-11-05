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
#ifndef AEONDE_SANDBOX_H
#define AEONDE_SANDBOX_H

#include <cstdint>

namespace AeonDE
{
class Sandbox
{
  public:
    Sandbox(int argc, char **argv);
    ~Sandbox();
    int Run();
    void OnSize(uint32_t aWidth, uint32_t aHeight);
    void *GetOpenGLContext() const;

  private:
    void PlatformDependentInitialize(int argc, char **argv);
    int PlatformDependentRun();
    void PlatformDependentFinalize();
    void Loop();
    /// Internal Window Id, required to create initial shared context
    void *mWindowId = nullptr;
    /// Internal OpenGL context, shared with all other contexts
    void *mOpenGLContext = nullptr;
};
} // namespace AeonDE
#endif
