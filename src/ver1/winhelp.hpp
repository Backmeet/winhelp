#pragma once
#include <windows.h>
#include <vector>
#include <array>
#include <string>
#include <cstdint>
#include <algorithm>
#include <cmath>
#include <format>
#include <chrono>
#include <thread>

namespace winhelp {
    LRESULT CALLBACK wndproc(HWND handle, UINT message, WPARAM wparam, LPARAM lparam);

    struct vec2 {
        float x;
        float y;
        vec2() : x(0), y(0) {}
        vec2(float X, float Y) : x(X), y(Y) {}
        vec2 operator+(const vec2& other) const { return { x + other.x, y + other.y }; }
        vec2 operator-(const vec2& other) const { return { x - other.x, y - other.y }; }
        vec2 operator*(float scalar) const { return { x * scalar, y * scalar }; }
        vec2 operator/(float scalar) const { return { x / scalar, y / scalar }; }
        vec2& operator+=(const vec2& other) { x += other.x; y += other.y; return *this; }
        vec2& operator-=(const vec2& other) { x -= other.x; y -= other.y; return *this; }
        vec2& operator*=(float scalar) { x *= scalar; y *= scalar; return *this; }
        vec2& operator/=(float scalar) { x /= scalar; y /= scalar; return *this; }
        float& operator[](int index) { return index ? y : x; }
        const float& operator[](int index) const { return index ? y : x; }
    };

    struct vec3 {
        float x;
        float y;
        float z;
        vec3() : x(0), y(0), z(0) {}
        vec3(float X, float Y, float Z) : x(X), y(Y), z(Z) {}
        vec3 operator+(const vec3& other) const { return { x + other.x, y + other.y, z + other.z }; }
        vec3 operator-(const vec3& other) const { return { x - other.x, y - other.y, z - other.z }; }
        vec3 operator*(float scalar) const { return { x * scalar, y * scalar, z * scalar }; }
        vec3 operator/(float scalar) const { return { x / scalar, y / scalar, z / scalar }; }
        vec3& operator+=(const vec3& other) { x += other.x; y += other.y; z += other.z; return *this; }
        vec3& operator-=(const vec3& other) { x -= other.x; y -= other.y; z -= other.z; return *this; }
        vec3& operator*=(float scalar) { x *= scalar; y *= scalar; z *= scalar; return *this; }
        vec3& operator/=(float scalar) { x /= scalar; y /= scalar; z /= scalar; return *this; }
        float& operator[](int index) { return index == 0 ? x : (index == 1 ? y : z); }
        const float& operator[](int index) const { return index == 0 ? x : (index == 1 ? y : z); }
    };

    inline vec2& internal_mouse() {
        static vec2 pos;
        return pos;
    }

    inline vec2 mouse() {
        return internal_mouse();
    }

    struct Surface {
        vec2 size;
        std::vector<uint32_t> pixels;
        Surface() : size(0, 0) {}
        Surface(vec2 surfaceSize) : size(surfaceSize), pixels((size_t)surfaceSize.x * surfaceSize.y, 0xFF000000) {}
        void fill(vec3 colour) {
            uint32_t value = 0xFF000000 | (uint32_t(colour.x) << 16) | (uint32_t(colour.y) << 8) | uint32_t(colour.z);
            std::fill(pixels.begin(), pixels.end(), value);
        }
        void blit(vec2 position, const Surface& source) {
            for (int y = 0; y < source.size.y; y++) {
                for (int x = 0; x < source.size.x; x++) {
                    int destinationX = position.x + x;
                    int destinationY = position.y + y;
                    if (destinationX < 0 || destinationY < 0 || destinationX >= size.x || destinationY >= size.y) continue;
                    pixels[(size_t)destinationY * size.x + destinationX] = source.pixels[(size_t)y * source.size.x + x];
                }
            }
        }
    };

    namespace events {
        enum class eventTypes {
            key_down,
            key_up,
            mouse_down,
            mouse_up,
            mouse_move,
            scroll_wheel_up,
            scroll_wheel_down,
            quit,
            minimized,
            fullscreened
        };

        enum class key {
            A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
            Num0, Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9,
            Left, Right, Up, Down,
            Space, Enter, Escape, Tab, Backspace, Shift, Ctrl, Alt,
            F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12
        };

        struct event {
            eventTypes type;
            vec2 hit;
            std::vector<key> keys;
            vec2 mouse;
        };

        inline std::vector<event>& queue() {
            static std::vector<event> internalQueue;
            return internalQueue;
        }

        inline std::vector<event> get() {
            MSG message;
            while (PeekMessageW(&message, nullptr, 0, 0, PM_REMOVE)) {
                TranslateMessage(&message);
                DispatchMessageW(&message);
            }
            std::vector<event> output = queue();
            queue().clear();
            return output;
        }
    }

    struct display {
        vec2 size;
        std::string title;
        HWND handle;
        Surface surface;
        display(vec2 displaySize, std::string windowTitle) : size(displaySize), title(windowTitle), handle(nullptr), surface(displaySize) {
            static HINSTANCE instance = GetModuleHandleW(nullptr);
            static bool registered = false;
            if (!registered) {
                WNDCLASSW windowClass{};
                windowClass.lpfnWndProc = wndproc;
                windowClass.hInstance = instance;
                windowClass.lpszClassName = L"winhelp";
                RegisterClassW(&windowClass);
                registered = true;
            }
            std::wstring wideTitle(title.begin(), title.end());
            handle = CreateWindowW(L"winhelp", wideTitle.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, size.x, size.y, nullptr, nullptr, instance, nullptr);
            ShowWindow(handle, SW_SHOW);
        }
        void set_size(vec2 newSize) {
            size = newSize;
            surface = Surface(newSize);
            SetWindowPos(handle, 0, 0, 0, newSize.x, newSize.y, SWP_NOMOVE | SWP_NOZORDER);
        }
        void set_title(std::string newTitle) {
            title = newTitle;
            std::wstring wideTitle(newTitle.begin(), newTitle.end());
            SetWindowTextW(handle, wideTitle.c_str());
        }
        void flip() {
            HDC deviceContext = GetDC(handle);
            BITMAPINFO bitmapInfo{ { sizeof(BITMAPINFOHEADER), size.x, -size.y, 1, 32, BI_RGB } };
            StretchDIBits(deviceContext, 0, 0, size.x, size.y, 0, 0, size.x, size.y, surface.pixels.data(), &bitmapInfo, DIB_RGB_COLORS, SRCCOPY);
            ReleaseDC(handle, deviceContext);
        }
        void close() {
            DestroyWindow(handle);
        }
    };

    namespace draw {
        inline void put_pixel(Surface& surface, int x, int y, vec3 colour) {
            if (x < 0 || y < 0 || x >= surface.size.x || y >= surface.size.y) return;
            surface.pixels[(size_t)y * surface.size.x + x] = 0xFF000000 | (uint32_t(colour.x) << 16) | (uint32_t(colour.y) << 8) | uint32_t(colour.z);
        }

        inline void line(Surface& surface, vec2 start, vec2 end, vec3 colour, float thickness = 1.0f) {
            int deltaX = std::abs(end.x - start.x);
            int deltaY = std::abs(end.y - start.y);
            int stepX = start.x < end.x ? 1 : -1;
            int stepY = start.y < end.y ? 1 : -1;
            int error = deltaX - deltaY;
            while (true) {
                put_pixel(surface, start.x, start.y, colour);
                if (start.x == end.x && start.y == end.y) break;
                int error2 = 2 * error;
                if (error2 > -deltaY) { error -= deltaY; start.x += stepX; }
                if (error2 < deltaX) { error += deltaX; start.y += stepY; }
            }
        }

        inline void lines(Surface& surface, std::vector<std::array<vec2, 2>> lineSegments, vec3 colour, float thickness = 1.0f) {
            for (auto& segment : lineSegments) {
                line(surface, segment[0], segment[1], colour, thickness);
            }
        }

        inline void polygon(Surface& surface, const std::vector<std::array<vec2, 2>>& lineSegments, vec3 colour, float thickness = 1.0f) {
            if (lineSegments.empty()) return;
            int minimumY = lineSegments[0][0].y;
            int maximumY = lineSegments[0][0].y;
            for (const auto& segment : lineSegments) {
                minimumY = std::min(minimumY, (const int)std::min(segment[0].y, segment[1].y));
                maximumY = std::max(maximumY, (const int)std::max(segment[0].y, segment[1].y));
            }
            uint32_t colourValue = 0xFF000000 | (uint32_t(colour.x) << 16) | (uint32_t(colour.y) << 8) | uint32_t(colour.z);
            for (int y = minimumY; y <= maximumY; y++) {
                std::vector<int> intersections;
                for (const auto& segment : lineSegments) {
                    vec2 a = segment[0];
                    vec2 b = segment[1];
                    if (a.y > b.y) std::swap(a, b);
                    if (y < a.y || y >= b.y) continue;
                    int dx = b.x - a.x;
                    int dy = b.y - a.y;
                    int x = a.x + (y - a.y) * dx / dy;
                    intersections.push_back(x);
                }
                std::sort(intersections.begin(), intersections.end());
                for (size_t i = 0; i + 1 < intersections.size(); i += 2) {
                    for (int x = intersections[i]; x <= intersections[i + 1]; x++) {
                        if (x < 0 || y < 0 || x >= surface.size.x || y >= surface.size.y) continue;
                        surface.pixels[(size_t)y * surface.size.x + x] = colourValue;
                    }
                }
            }
        }

        inline void rect(Surface& surface, vec2 position, vec2 rectangleSize, vec3 colour, float thickness = 1.0f) {
            line(surface, position, { position.x + rectangleSize.x, position.y }, colour, thickness);
            line(surface, { position.x + rectangleSize.x, position.y }, { position.x + rectangleSize.x, position.y + rectangleSize.y }, colour, thickness);
            line(surface, { position.x + rectangleSize.x, position.y + rectangleSize.y }, { position.x, position.y + rectangleSize.y }, colour, thickness);
            line(surface, { position.x, position.y + rectangleSize.y }, position, colour, thickness);
        }

        inline void circle(Surface& surface, vec2 center, int radius, vec3 colour, float thickness = 1.0f) {
            for (int y = -radius; y <= radius; y++) {
                for (int x = -radius; x <= radius; x++) {
                    if (x * x + y * y <= radius * radius) {
                        put_pixel(surface, center.x + x, center.y + y, colour);
                    }
                }
            }
        }
    }

    inline events::key map_key(WPARAM keyCode) {
        if (keyCode >= 'A' && keyCode <= 'Z') return (events::key)(keyCode - 'A');
        if (keyCode >= '0' && keyCode <= '9') return (events::key)((int)events::key::Num0 + (keyCode - '0'));
        return events::key::Space;
    }

    inline LRESULT CALLBACK wndproc(HWND handle, UINT message, WPARAM wparam, LPARAM lparam) {
        using namespace events;
        switch (message) {
            case WM_DESTROY:
                queue().push_back({ eventTypes::quit, { 0, 0 }, {}, { 0, 0 } });
                PostQuitMessage(0);
                return 0;
            case WM_KEYDOWN:
                queue().push_back({ eventTypes::key_down, { 0, 0 }, { map_key(wparam) }, { 0, 0 } });
                break;
            case WM_KEYUP:
                queue().push_back({ eventTypes::key_up, { 0, 0 }, { map_key(wparam) }, { 0, 0 } });
                break;
            case WM_LBUTTONDOWN:
                queue().push_back({ eventTypes::mouse_down, { (int)(short)LOWORD(lparam), (int)(short)HIWORD(lparam) }, {}, { 0, 0 } });
                break;
            case WM_LBUTTONUP:
                queue().push_back({ eventTypes::mouse_up, { (int)(short)LOWORD(lparam), (int)(short)HIWORD(lparam) }, {}, { 0, 0 } });
                break;
            case WM_MOUSEWHEEL:
                queue().push_back({ GET_WHEEL_DELTA_WPARAM(wparam) > 0 ? eventTypes::scroll_wheel_up : eventTypes::scroll_wheel_down, { 0, (int)GET_WHEEL_DELTA_WPARAM(wparam) }, {}, { 0, 0 } });
                break;
            case WM_MOUSEMOVE: {
                vec2 p{ (short)LOWORD(lparam), (short)HIWORD(lparam) };
                internal_mouse() = p;
                queue().push_back({ eventTypes::mouse_move, p, {}, p });
                break;
            }
        }
        return DefWindowProcW(handle, message, wparam, lparam);
    }

    inline float fps = 0;

    namespace internal_timing {
        using clock = std::chrono::steady_clock;

        inline clock::time_point& last_frame() {
            static clock::time_point t = clock::now();
            return t;
        }

        inline clock::time_point& last_fps_tick() {
            static clock::time_point t = last_frame();
            return t;
        }

        inline unsigned int& frame_counter() {
            static unsigned int c = 0;
            return c;
        }
    }

    inline void tick() {
        using namespace internal_timing;

        const auto now = clock::now();
        frame_counter()++;

        const auto elapsed = now - last_fps_tick();
        if (elapsed >= std::chrono::seconds(1)) {
            fps = frame_counter();
            frame_counter() = 0;
            last_fps_tick() = now;
        }

        last_frame() = now;
    }

    inline void tick(int target) {
        using namespace internal_timing;

        if (target <= 0) {
            tick();
            return;
        }

        const auto frame_duration =
            std::chrono::duration<double>(1.0 / double(target));

        const auto next_frame_time = last_frame() + frame_duration;

        std::this_thread::sleep_until(next_frame_time);

        tick();
    }

}