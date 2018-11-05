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
#ifdef _WIN32
#include "Sandbox.h"
#include <gl/gl.h>
#include <gl/glext.h>
#include <gl/wglext.h>
#include <iostream>
#include <stdexcept>

namespace AeonDE
{
static PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsString = nullptr;
static PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribs = nullptr;
const int ContextAttribs[] =
    {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
        WGL_CONTEXT_MINOR_VERSION_ARB, 5,
        WGL_CONTEXT_PROFILE_MASK_ARB,
        WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0};

LRESULT CALLBACK SandboxWindowProc(
    _In_ HWND hwnd,
    _In_ UINT uMsg,
    _In_ WPARAM wParam,
    _In_ LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_SIZE:
    {
        Sandbox *sandbox = reinterpret_cast<Sandbox *>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
        HDC hdc = GetDC(hwnd);
        wglMakeCurrent(hdc, reinterpret_cast<HGLRC>(sandbox->GetOpenGLContext()));
        ReleaseDC(hwnd, hdc);
        sandbox->OnSize(LOWORD(lParam), HIWORD(lParam));
        break;
    }
    case WM_CLOSE:
        DestroyWindow(hwnd);
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void Sandbox::PlatformDependentInitialize(int argc, char **argv)
{
    // Initialize Internal Window
    PIXELFORMATDESCRIPTOR pfd;
    RECT rect = {0, 0, 640, 360};
    AdjustWindowRectEx(&rect, WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
                       false,
                       WS_EX_APPWINDOW | WS_EX_WINDOWEDGE);
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wcex.lpfnWndProc = (WNDPROC)SandboxWindowProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = GetModuleHandle(nullptr);
    wcex.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = nullptr;
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = "glUnitTest";
    wcex.hIconSm = nullptr;
    ATOM atom = RegisterClassEx(&wcex);
    mWindowId = CreateWindowEx(WS_EX_APPWINDOW | WS_EX_WINDOWEDGE,
                               MAKEINTATOM(atom), "AeonDE Sandbox",
                               WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
                               (GetSystemMetrics(SM_CXSCREEN) - rect.right) / 2, // Location
                               (GetSystemMetrics(SM_CYSCREEN) - rect.bottom) / 2,
                               rect.right - rect.left, rect.bottom - rect.top, // Dimensions
                               nullptr,
                               nullptr,
                               GetModuleHandle(nullptr),
                               nullptr);
    SetWindowLongPtr(static_cast<HWND>(mWindowId), GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));
    HDC hdc = GetDC(static_cast<HWND>(mWindowId));
    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.cColorBits = 32;
    pfd.cRedBits = 0;
    pfd.cRedShift = 0;
    pfd.cGreenBits = 0;
    pfd.cGreenShift = 0;
    pfd.cBlueBits = 0;
    pfd.cBlueShift = 0;
    pfd.cAlphaBits = 0;
    pfd.cAlphaShift = 0;
    pfd.cAccumBits = 0;
    pfd.cAccumRedBits = 0;
    pfd.cAccumGreenBits = 0;
    pfd.cAccumBlueBits = 0;
    pfd.cAccumAlphaBits = 0;
    pfd.cDepthBits = 32;
    pfd.cStencilBits = 0;
    pfd.cAuxBuffers = 0;
    pfd.iLayerType = PFD_MAIN_PLANE;
    pfd.bReserved = 0;
    pfd.dwLayerMask = 0;
    pfd.dwVisibleMask = 0;
    pfd.dwDamageMask = 0;
    int pf = ChoosePixelFormat(hdc, &pfd);
    SetPixelFormat(hdc, pf, &pfd);

    // Create OpenGL Context
    mOpenGLContext = wglCreateContext(hdc);
    wglMakeCurrent(hdc, static_cast<HGLRC>(mOpenGLContext));

    // Get newer functions if needed
    if (!wglGetExtensionsString)
    {
        if (!(wglGetExtensionsString = (PFNWGLGETEXTENSIONSSTRINGARBPROC)wglGetProcAddress("wglGetExtensionsStringARB")))
        {
            wglMakeCurrent(hdc, nullptr);
            wglDeleteContext(static_cast<HGLRC>(mOpenGLContext));
            mOpenGLContext = nullptr;
            ReleaseDC(static_cast<HWND>(mWindowId), hdc);
            DestroyWindow(static_cast<HWND>(mWindowId));
            throw std::runtime_error("Failed retrieving a pointer to wglGetExtensionsString");
        }
    }
    if (!wglCreateContextAttribs)
    {
        if (!(wglCreateContextAttribs = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB")))
        {
            wglMakeCurrent(hdc, nullptr);
            wglDeleteContext(static_cast<HGLRC>(mOpenGLContext));
            mOpenGLContext = nullptr;
            ReleaseDC(static_cast<HWND>(mWindowId), hdc);
            DestroyWindow(static_cast<HWND>(mWindowId));
            throw std::runtime_error("Failed retrieving a pointer to wglCreateContextAttribsARB");
        }
    }
    if (strstr(wglGetExtensionsString(hdc), "WGL_ARB_create_context") != nullptr)
    {
        wglMakeCurrent(hdc, nullptr);
        wglDeleteContext(static_cast<HGLRC>(mOpenGLContext));
        if (!(mOpenGLContext = wglCreateContextAttribs(hdc, nullptr /* change to use local context */, ContextAttribs)))
        {
            ReleaseDC(static_cast<HWND>(mWindowId), hdc);
            DestroyWindow(static_cast<HWND>(mWindowId));
            throw std::runtime_error("wglCreateContextAttribs Failed");
        }
    }
    else
    {
        wglMakeCurrent(hdc, nullptr);
        wglDeleteContext(static_cast<HGLRC>(mOpenGLContext));
        mOpenGLContext = nullptr;
        ReleaseDC(static_cast<HWND>(mWindowId), hdc);
        DestroyWindow(static_cast<HWND>(mWindowId));
        throw std::runtime_error("WGL_ARB_create_context is not available");
    }

    // Get New OpenGL API
    if (!wglMakeCurrent(hdc, reinterpret_cast<HGLRC>(mOpenGLContext)))
    {
        std::cout << "wglMakeCurrent Failed. Error: " << GetLastError() << std::endl;
    }
    glViewport(0, 0, 640, 360);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    SwapBuffers(hdc);
    ReleaseDC(reinterpret_cast<HWND>(mWindowId), hdc);
}

int Sandbox::PlatformDependentRun()
{
    ShowWindow(reinterpret_cast<HWND>(mWindowId), SW_SHOWNORMAL);
    MSG msg{};
    while (true)
    {
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        if (msg.message == WM_QUIT)
        {
            std::cout << "Quit." << std::endl;
            break;
        }
        HDC hdc = GetDC(static_cast<HWND>(mWindowId));
        Loop();
        SwapBuffers(hdc);
        ReleaseDC(static_cast<HWND>(mWindowId), hdc);
    }
    return msg.wParam;
}

void Sandbox::PlatformDependentFinalize()
{
    HDC hdc = GetDC(static_cast<HWND>(mWindowId));
    wglMakeCurrent(hdc, static_cast<HGLRC>(mOpenGLContext));
    ATOM atom = GetClassWord(static_cast<HWND>(mWindowId), GCW_ATOM);
    wglMakeCurrent(hdc, nullptr);
    wglDeleteContext(static_cast<HGLRC>(mOpenGLContext));
    mOpenGLContext = nullptr;
    ReleaseDC(static_cast<HWND>(mWindowId), hdc);
    DestroyWindow(static_cast<HWND>(mWindowId));
    UnregisterClass(reinterpret_cast<LPCSTR>(
#if defined(_M_X64) || defined(__amd64__)
                        0x0ULL +
#endif
                        MAKELONG(atom, 0)),
                    nullptr);
}
} // namespace AeonDE
#endif
