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
#include <cstring>

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

    struct ivec2 {
        int x;
        int y;

        ivec2() : x(0), y(0) {}
        ivec2(int X, int Y) : x(X), y(Y) {}

        // Explicit float -> int
        explicit ivec2(const vec2& v)
            : x((int)v.x), y((int)v.y) {}

        ivec2 operator+(const ivec2& other) const { return { x + other.x, y + other.y }; }
        ivec2 operator-(const ivec2& other) const { return { x - other.x, y - other.y }; }
        ivec2 operator*(int scalar) const { return { x * scalar, y * scalar }; }
        ivec2 operator/(int scalar) const { return { x / scalar, y / scalar }; }

        ivec2& operator+=(const ivec2& other) { x += other.x; y += other.y; return *this; }
        ivec2& operator-=(const ivec2& other) { x -= other.x; y -= other.y; return *this; }
        ivec2& operator*=(int scalar) { x *= scalar; y *= scalar; return *this; }
        ivec2& operator/=(int scalar) { x /= scalar; y /= scalar; return *this; }

        int& operator[](int index) { return index ? y : x; }
        const int& operator[](int index) const { return index ? y : x; }

        // Explicit int -> float
        explicit operator vec2() const {
            return vec2((float)x, (float)y);
        }
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

    struct vec4 {
        float x;
        float y;
        float z;
        float w;

        vec4() : x(0), y(0), z(0), w(255) {}

        vec4(float X, float Y, float Z, float W)
            : x(X), y(Y), z(Z), w(W) {}

        // Implicit vec3 -> vec4 (alpha defaults to 255)
        vec4(const vec3& v)
            : x(v.x), y(v.y), z(v.z), w(255) {}

        vec4 operator+(const vec4& other) const {
            return { x + other.x, y + other.y, z + other.z, w + other.w };
        }

        vec4 operator-(const vec4& other) const {
            return { x - other.x, y - other.y, z - other.z, w - other.w };
        }

        vec4 operator*(float scalar) const {
            return { x * scalar, y * scalar, z * scalar, w * scalar };
        }

        vec4 operator/(float scalar) const {
            return { x / scalar, y / scalar, z / scalar, w / scalar };
        }

        vec4& operator+=(const vec4& other) {
            x += other.x; y += other.y; z += other.z; w += other.w;
            return *this;
        }

        vec4& operator-=(const vec4& other) {
            x -= other.x; y -= other.y; z -= other.z; w -= other.w;
            return *this;
        }

        vec4& operator*=(float scalar) {
            x *= scalar; y *= scalar; z *= scalar; w *= scalar;
            return *this;
        }

        vec4& operator/=(float scalar) {
            x /= scalar; y /= scalar; z /= scalar; w /= scalar;
            return *this;
        }

        float& operator[](int index) {
            return index == 0 ? x :
                index == 1 ? y :
                index == 2 ? z : w;
        }

        const float& operator[](int index) const {
            return index == 0 ? x :
                index == 1 ? y :
                index == 2 ? z : w;
        }

        // Explicit vec4 -> vec3
        explicit operator vec3() const {
            return vec3(x, y, z);
        }
    };

    inline vec2& internal_mouse() {
        static vec2 pos;
        return pos;
    }

    inline vec2 mouse() {
        return internal_mouse();
    }

    struct Surface {
        ivec2 size;
        std::vector<uint32_t> pixels;

        Surface() : size(0, 0) {}

        Surface(vec2 surfaceSize)
            : size(surfaceSize),
            pixels((size_t)surfaceSize.x * surfaceSize.y, 0xFF000000) {}

        static uint32_t pack(const vec4& c) {
            return
                (uint32_t(c.w) << 24) |
                (uint32_t(c.x) << 16) |
                (uint32_t(c.y) << 8)  |
                uint32_t(c.z);
        }

        static vec4 unpack(uint32_t v) {
            return vec4(
                float((v >> 16) & 0xFF),
                float((v >> 8)  & 0xFF),
                float(v & 0xFF),
                float((v >> 24) & 0xFF)
            );
        }

        void fill(vec4 colour) {
            uint32_t value = pack(colour);
            std::fill(pixels.begin(), pixels.end(), value);
        }

        void blit(vec2 position, const Surface& source) {
            for (int y = 0; y < source.size.y; y++) {
                for (int x = 0; x < source.size.x; x++) {

                    int dx = position.x + x;
                    int dy = position.y + y;

                    if (dx < 0 || dy < 0 ||
                        dx >= size.x || dy >= size.y)
                        continue;

                    uint32_t src = source.pixels[(size_t)y * source.size.x + x];
                    uint32_t& dst = pixels[(size_t)dy * size.x + dx];

                    uint8_t srcA = (src >> 24) & 0xFF;

                    if (srcA == 255) {
                        dst = src;
                        continue;
                    }

                    if (srcA == 0)
                        continue;

                    uint8_t srcR = (src >> 16) & 0xFF;
                    uint8_t srcG = (src >> 8)  & 0xFF;
                    uint8_t srcB =  src        & 0xFF;

                    uint8_t dstR = (dst >> 16) & 0xFF;
                    uint8_t dstG = (dst >> 8)  & 0xFF;
                    uint8_t dstB =  dst        & 0xFF;

                    float alpha = srcA / 255.0f;
                    float inv   = 1.0f - alpha;

                    uint8_t outR = uint8_t(srcR * alpha + dstR * inv);
                    uint8_t outG = uint8_t(srcG * alpha + dstG * inv);
                    uint8_t outB = uint8_t(srcB * alpha + dstB * inv);

                    dst =
                        (0xFF << 24) |
                        (outR << 16) |
                        (outG << 8)  |
                        outB;
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

        enum class mouse {
            left, right, middle, none
        };

        struct event {
            eventTypes type;
            vec2 hit;
            std::vector<key> keys;
            mouse click;
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

        BITMAPINFO bitmapInfo{};

        display(vec2 displaySize, std::string windowTitle)
            : size(displaySize),
            title(windowTitle),
            handle(nullptr),
            surface(displaySize)
        {
            static HINSTANCE instance = GetModuleHandleW(nullptr);
            static bool registered = false;

            if (!registered) {
                WNDCLASSW windowClass{};
                windowClass.lpfnWndProc = wndproc;
                windowClass.hInstance = instance;
                windowClass.lpszClassName = L"winhelp";
                windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);

                if (!RegisterClassW(&windowClass)) {
                    if (GetLastError() != ERROR_CLASS_ALREADY_EXISTS) {
                        MessageBoxA(NULL, "RegisterClass failed", "Error", MB_OK);
                        return;
                    }
                }

                registered = true;
            }

            std::wstring wideTitle(title.begin(), title.end());

            handle = CreateWindowW(
                L"winhelp",
                wideTitle.c_str(),
                WS_OVERLAPPEDWINDOW,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                (int)size.x,
                (int)size.y,
                nullptr,
                nullptr,
                instance,
                nullptr
            );

            if (!handle) {
                DWORD err = GetLastError();

                LPSTR msg = nullptr;
                FormatMessageA(
                    FORMAT_MESSAGE_ALLOCATE_BUFFER |
                    FORMAT_MESSAGE_FROM_SYSTEM |
                    FORMAT_MESSAGE_IGNORE_INSERTS,
                    NULL,
                    err,
                    0,
                    (LPSTR)&msg,
                    0,
                    NULL
                );

                MessageBoxA(NULL, msg ? msg : "Unknown error",
                            "CreateWindowEx failed", MB_OK);

                if (msg) LocalFree(msg);
                return;
            }

            ShowWindow(handle, SW_SHOW);
            UpdateWindow(handle);

            configure_bitmap();
        }

        void configure_bitmap() {
            ZeroMemory(&bitmapInfo, sizeof(bitmapInfo));
            bitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
            bitmapInfo.bmiHeader.biWidth = (int)size.x;
            bitmapInfo.bmiHeader.biHeight = -(int)size.y; // top-down
            bitmapInfo.bmiHeader.biPlanes = 1;
            bitmapInfo.bmiHeader.biBitCount = 32;
            bitmapInfo.bmiHeader.biCompression = BI_RGB;
        }

        void set_size(vec2 newSize) {
            size = newSize;
            surface = Surface(newSize);

            SetWindowPos(
                handle,
                nullptr,
                0, 0,
                (int)newSize.x,
                (int)newSize.y,
                SWP_NOMOVE | SWP_NOZORDER
            );

            configure_bitmap();
        }

        void set_title(std::string newTitle) {
            title = newTitle;
            std::wstring wideTitle(newTitle.begin(), newTitle.end());
            SetWindowTextW(handle, wideTitle.c_str());
        }

        void flip() {
            HDC dc = GetDC(handle);

            StretchDIBits(
                dc,
                0, 0,
                (int)size.x,
                (int)size.y,
                0, 0,
                (int)size.x,
                (int)size.y,
                surface.pixels.data(),
                &bitmapInfo,
                DIB_RGB_COLORS,
                SRCCOPY
            );

            ReleaseDC(handle, dc);
        }

        void close() {
            DestroyWindow(handle);
            handle = nullptr;
        }
    };

    namespace font {

        inline Surface text(
            const std::wstring& content,
            vec4 textColour,
            vec4 bgColour,
            int fontSize = 16,
            const std::wstring& fontName = L"Consolas"
        ) {
            if (content.empty())
                return Surface({0, 0});

            HDC hdc = CreateCompatibleDC(nullptr);

            SetMapMode(hdc, MM_TEXT);
            SetGraphicsMode(hdc, GM_COMPATIBLE);

            HFONT font = CreateFontW(
                -fontSize,
                0, 0, 0,
                FW_NORMAL,
                FALSE, FALSE, FALSE,
                DEFAULT_CHARSET,
                OUT_DEFAULT_PRECIS,
                CLIP_DEFAULT_PRECIS,
                ANTIALIASED_QUALITY,
                DEFAULT_PITCH | FF_DONTCARE,
                fontName.c_str()
            );

            HGDIOBJ oldFont = SelectObject(hdc, font);

            // Measure text
            RECT rc = {0, 0, 0, 0};
            DrawTextW(
                hdc,
                content.c_str(),
                (int)content.length(),
                &rc,
                DT_CALCRECT | DT_LEFT | DT_TOP | DT_NOPREFIX
            );

            int width  = rc.right - rc.left;
            int height = rc.bottom - rc.top;

            if (width <= 0 || height <= 0) {
                SelectObject(hdc, oldFont);
                DeleteObject(font);
                DeleteDC(hdc);
                return Surface({0, 0});
            }

            Surface result({ (float)width, (float)height });

            BITMAPINFO bmi{};
            bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
            bmi.bmiHeader.biWidth = width;
            bmi.bmiHeader.biHeight = -height;
            bmi.bmiHeader.biPlanes = 1;
            bmi.bmiHeader.biBitCount = 32;
            bmi.bmiHeader.biCompression = BI_RGB;

            void* bits = nullptr;

            HBITMAP dib = CreateDIBSection(
                hdc,
                &bmi,
                DIB_RGB_COLORS,
                &bits,
                nullptr,
                0
            );

            HGDIOBJ oldBitmap = SelectObject(hdc, dib);

            std::memset(bits, 0, width * height * 4);

            bool transparentBG = (bgColour.w == 0);

            if (transparentBG) {
                SetBkMode(hdc, OPAQUE);
                SetBkColor(hdc, RGB(0,0,0));
            } else {
                SetBkMode(hdc, OPAQUE);
                SetBkColor(hdc, RGB(
                    (int)bgColour.x,
                    (int)bgColour.y,
                    (int)bgColour.z
                ));
            }

            SetTextColor(hdc, RGB(255,255,255)); // render white for alpha extraction

            RECT drawRc = {0, 0, width, height};

            DrawTextW(
                hdc,
                content.c_str(),
                (int)content.length(),
                &drawRc,
                DT_LEFT | DT_TOP | DT_NOPREFIX
            );

            uint32_t* src = (uint32_t*)bits;

            for (size_t i = 0; i < result.pixels.size(); ++i) {

                uint32_t pixel = src[i];

                uint8_t r = (pixel >> 16) & 0xFF;
                uint8_t g = (pixel >> 8)  & 0xFF;
                uint8_t b =  pixel        & 0xFF;

                uint8_t glyphAlpha = r; // white text → grayscale

                if (glyphAlpha > 0) {

                    result.pixels[i] =
                        (uint32_t(glyphAlpha) << 24) |
                        (uint32_t(textColour.x) << 16) |
                        (uint32_t(textColour.y) << 8)  |
                        uint32_t(textColour.z);

                } else {

                    if (!transparentBG) {
                        result.pixels[i] =
                            (uint32_t(bgColour.w) << 24) |
                            (uint32_t(bgColour.x) << 16) |
                            (uint32_t(bgColour.y) << 8)  |
                            uint32_t(bgColour.z);
                    } else {
                        result.pixels[i] = 0;
                    }
                }
            }

            SelectObject(hdc, oldBitmap);
            SelectObject(hdc, oldFont);

            DeleteObject(dib);
            DeleteObject(font);
            DeleteDC(hdc);

            return result;
        }

    }

    namespace draw {

        inline uint32_t pack_colour(vec3 c, uint8_t a = 255) {
            return (uint32_t(a) << 24) |
                (uint32_t(c.x) << 16) |
                (uint32_t(c.y) << 8)  |
                uint32_t(c.z);
        }

        inline void put_pixel(Surface& surface, int x, int y, vec3 colour) {
            if (x < 0 || y < 0 || x >= surface.size.x || y >= surface.size.y)
                return;

            surface.pixels[(size_t)y * surface.size.x + x] =
                pack_colour(colour);
        }

        // Alpha blended pixel write (for font blitting etc.)
        inline void put_pixel_alpha(Surface& surface, int x, int y, uint32_t src) {
            if (x < 0 || y < 0 || x >= surface.size.x || y >= surface.size.y)
                return;

            uint32_t& dst = surface.pixels[(size_t)y * surface.size.x + x];

            uint8_t srcA = (src >> 24) & 0xFF;
            if (srcA == 255) {
                dst = src;
                return;
            }
            if (srcA == 0) return;

            uint8_t dstR = (dst >> 16) & 0xFF;
            uint8_t dstG = (dst >> 8) & 0xFF;
            uint8_t dstB = dst & 0xFF;

            uint8_t srcR = (src >> 16) & 0xFF;
            uint8_t srcG = (src >> 8) & 0xFF;
            uint8_t srcB = src & 0xFF;

            uint8_t invA = 255 - srcA;

            uint8_t r = (srcR * srcA + dstR * invA) / 255;
            uint8_t g = (srcG * srcA + dstG * invA) / 255;
            uint8_t b = (srcB * srcA + dstB * invA) / 255;

            dst = (0xFF << 24) | (r << 16) | (g << 8) | b;
        }

        inline void blit(Surface& target, const Surface& source, vec2 position) {
            for (int y = 0; y < source.size.y; ++y) {
                for (int x = 0; x < source.size.x; ++x) {
                    uint32_t srcPixel =
                        source.pixels[(size_t)y * source.size.x + x];

                    put_pixel_alpha(
                        target,
                        position.x + x,
                        position.y + y,
                        srcPixel
                    );
                }
            }
        }

        inline void line(Surface& surface, vec2 start, vec2 end,
                        vec3 colour, float thickness = 1.0f) {

            int x0 = start.x;
            int y0 = start.y;
            int x1 = end.x;
            int y1 = end.y;

            int dx = std::abs(x1 - x0);
            int dy = std::abs(y1 - y0);
            int sx = x0 < x1 ? 1 : -1;
            int sy = y0 < y1 ? 1 : -1;
            int err = dx - dy;

            while (true) {

                int half = (int)(thickness * 0.5f);
                for (int ty = -half; ty <= half; ++ty)
                    for (int tx = -half; tx <= half; ++tx)
                        put_pixel(surface, x0 + tx, y0 + ty, colour);

                if (x0 == x1 && y0 == y1) break;

                int e2 = err * 2;
                if (e2 > -dy) { err -= dy; x0 += sx; }
                if (e2 < dx)  { err += dx; y0 += sy; }
            }
        }

        inline void rect(Surface& surface, vec2 pos,
                        vec2 size, vec3 colour,
                        bool filled = true,
                        float thickness = 1.0f) {

            if (not filled) {
                line(surface, pos,
                    { pos.x + size.x, pos.y }, colour, thickness);

                line(surface,
                    { pos.x + size.x, pos.y },
                    { pos.x + size.x, pos.y + size.y },
                    colour, thickness);

                line(surface,
                    { pos.x + size.x, pos.y + size.y },
                    { pos.x, pos.y + size.y },
                    colour, thickness);

                line(surface,
                    { pos.x, pos.y + size.y },
                    pos, colour, thickness);
            } else {
                int w = static_cast<int>(size.x);
                int h = static_cast<int>(size.y);
                int px = static_cast<int>(pos.x);
                int py = static_cast<int>(pos.y);

                for (int xinc = 0; xinc < w; xinc++) {
                    for (int yinc = 0; yinc < h; yinc++) {
                        put_pixel(surface, px + xinc, py - yinc, colour);
                    }
                }
            }
        }

        inline void circle(Surface& surface, vec2 center,
                        int radius, vec3 colour,
                        bool filled = true) {

            for (int y = -radius; y <= radius; ++y) {
                for (int x = -radius; x <= radius; ++x) {

                    int dist2 = x * x + y * y;

                    if (filled) {
                        if (dist2 <= radius * radius)
                            put_pixel(surface,
                                    center.x + x,
                                    center.y + y,
                                    colour);
                    } else {
                        if (dist2 <= radius * radius &&
                            dist2 >= (radius - 1) * (radius - 1))
                            put_pixel(surface,
                                    center.x + x,
                                    center.y + y,
                                    colour);
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
            case WM_CLOSE:
                
                DestroyWindow(handle);
                return 0;
            case WM_DESTROY:
                queue().push_back({ eventTypes::quit, { 0, 0 }, {}, events::mouse::none});
                PostQuitMessage(0);
                return 0;
            case WM_KEYDOWN:
                queue().push_back({ eventTypes::key_down, { 0, 0 }, { map_key(wparam) }, events::mouse::none});
                break;
            case WM_KEYUP:
                queue().push_back({ eventTypes::key_up, { 0, 0 }, { map_key(wparam) }, events::mouse::none});
                break;
            case WM_LBUTTONDOWN:
                queue().push_back({ eventTypes::mouse_down, { (int)(short)LOWORD(lparam), (int)(short)HIWORD(lparam) }, {}, events::mouse::left});
                break;
            case WM_LBUTTONUP:
                queue().push_back({ eventTypes::mouse_up, { (int)(short)LOWORD(lparam), (int)(short)HIWORD(lparam) }, {}, events::mouse::none});
                break;
            case WM_RBUTTONDOWN:
                queue().push_back({ eventTypes::mouse_down, { (int)(short)LOWORD(lparam), (int)(short)HIWORD(lparam) }, {}, events::mouse::right});
                break;
            case WM_RBUTTONUP:
                queue().push_back({ eventTypes::mouse_up, { (int)(short)LOWORD(lparam), (int)(short)HIWORD(lparam) }, {}, events::mouse::none});
                break;
            case WM_MBUTTONDOWN:
                queue().push_back({ eventTypes::mouse_down, { (int)(short)LOWORD(lparam), (int)(short)HIWORD(lparam) }, {}, events::mouse::middle});
                break;
            case WM_MBUTTONUP:
                queue().push_back({ eventTypes::mouse_up, { (int)(short)LOWORD(lparam), (int)(short)HIWORD(lparam) }, {}, events::mouse::none});
                break;
            case WM_MOUSEWHEEL:
                queue().push_back({ GET_WHEEL_DELTA_WPARAM(wparam) > 0 ? eventTypes::scroll_wheel_up : eventTypes::scroll_wheel_down, { 0, (int)GET_WHEEL_DELTA_WPARAM(wparam) }, {}, events::mouse::none});
                break;
            case WM_MOUSEMOVE: {
                vec2 p{ (short)LOWORD(lparam), (short)HIWORD(lparam) };
                internal_mouse() = p;
                queue().push_back({ eventTypes::mouse_move, p, {}, events::mouse::none});
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