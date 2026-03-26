#pragma once
#include <windows.h>
#include <windowsx.h>

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
#include <stdexcept>

#include <stdint.h>


namespace winhelp {
    LRESULT CALLBACK wndproc(HWND handle, UINT message, WPARAM wparam, LPARAM lparam);

    struct VectorDivideByZero : std::exception {
        std::string msg;
        VectorDivideByZero(const std::string& m) : msg(m) {}

        const char* what() const noexcept override {
            return msg.c_str();
        }
    };

    struct vec2 {
        float x;
        float y;
        vec2() : x(0), y(0) {}
        vec2(float X, float Y) : x(X), y(Y) {}
        vec2 operator+(const vec2& other) const { return { x + other.x, y + other.y }; }
        vec2 operator-(const vec2& other) const { return { x - other.x, y - other.y }; }
        vec2 operator*(float scalar) const { return { x * scalar, y * scalar }; }
        vec2 operator/(float scalar) const { 
            if (scalar == 0) {
                throw VectorDivideByZero("Tried to divide a vector2 by scalar zero");
            }
            return { x / scalar, y / scalar };
        }
        vec2& operator+=(const vec2& other) { x += other.x; y += other.y; return *this; }
        vec2& operator-=(const vec2& other) { x -= other.x; y -= other.y; return *this; }
        vec2& operator*=(float scalar) { x *= scalar; y *= scalar; return *this; }
        vec2& operator/=(float scalar) { 
            if (scalar == 0) {
                throw VectorDivideByZero("Tried to divide a vector2 by scalar zero");
            }
            x /= scalar; y /= scalar;
            return *this;
        }
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
        ivec2 operator/(int scalar) const { 
            if (scalar == 0) {
                throw VectorDivideByZero("Tried to divide a int vector2 by scalar zero");
            }
            return { x / scalar, y / scalar };
        }

        ivec2& operator+=(const ivec2& other) { x += other.x; y += other.y; return *this; }
        ivec2& operator-=(const ivec2& other) { x -= other.x; y -= other.y; return *this; }
        ivec2& operator*=(int scalar) { x *= scalar; y *= scalar; return *this; }
        ivec2& operator/=(int scalar) {
            if (scalar == 0) {
                throw VectorDivideByZero("Tried to divide a int vector2 by scalar zero");
            }
            x /= scalar; y /= scalar;
            return *this;
        }

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
        vec3 operator/(float scalar) const {
            if (scalar == 0) {
                throw VectorDivideByZero("Tried to divide a int vector2 by scalar zero");
            }

            return { x / scalar, y / scalar, z / scalar };
        }

        vec3& operator+=(const vec3& other) { x += other.x; y += other.y; z += other.z; return *this; }
        vec3& operator-=(const vec3& other) { x -= other.x; y -= other.y; z -= other.z; return *this; }
        vec3& operator*=(float scalar) { x *= scalar; y *= scalar; z *= scalar; return *this; }
        vec3& operator/=(float scalar) {
            if (scalar == 0) {
                throw VectorDivideByZero("Tried to divide a int vector2 by scalar zero");
            }

            x /= scalar; y /= scalar; z /= scalar;
            return *this;
        }
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
        bool hasAlpha = false;

        Surface() : size(0, 0) {}

        Surface(vec2 surfaceSize)
            : size(std::max(0, (int)surfaceSize.x), std::max(0, (int)surfaceSize.y)),
            pixels((size_t)size.x * size.y, 0xFF000000) {}

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
            uint32_t* ptr = pixels.data();
            uint32_t* end = ptr + pixels.size();
            while (ptr < end)
                *ptr++ = value;
        }

        void blit(vec2 position, const Surface& source, bool blend = true) {

            int startX = std::max(0, (int)position.x);
            int startY = std::max(0, (int)position.y);

            int endX = std::min(size.x, (int)position.x + source.size.x);
            int endY = std::min(size.y, (int)position.y + source.size.y);

            if (startX >= endX || startY >= endY)
                return;

            int srcOffsetX = startX - (int)position.x;
            int srcOffsetY = startY - (int)position.y;

            // i am speed
            if (!blend || !source.hasAlpha) {

                for (int y = startY; y < endY; ++y) {

                    uint32_t* dstRow =
                        &pixels[(size_t)y * size.x + startX];

                    const uint32_t* srcRow =
                        &source.pixels[
                            (size_t)(y - startY + srcOffsetY) *
                            source.size.x + srcOffsetX
                        ];

                    memcpy(dstRow, srcRow,
                        (endX - startX) * sizeof(uint32_t));
                }

                return;
            }

            // i am not speed
            for (int y = startY; y < endY; ++y) {

                uint32_t* dstRow =
                    &pixels[(size_t)y * size.x + startX];

                const uint32_t* srcRow =
                    &source.pixels[
                        (size_t)(y - startY + srcOffsetY) *
                        source.size.x + srcOffsetX
                    ];

                for (int x = startX; x < endX; ++x) {

                    uint32_t src = *srcRow++;
                    uint32_t& dst = *dstRow++;

                    uint32_t srcA = src >> 24;

                    if (srcA == 255) {
                        dst = src;
                        continue;
                    }

                    if (srcA == 0)
                        continue;

                    uint32_t dstRB = dst & 0x00FF00FF;
                    uint32_t dstG  = dst & 0x0000FF00;

                    uint32_t srcRB = src & 0x00FF00FF;
                    uint32_t srcG  = src & 0x0000FF00;

                    uint32_t invA = 255 - srcA;

                    dstRB = (dstRB * invA) >> 8;
                    dstG  = (dstG  * invA) >> 8;

                    srcRB = (srcRB * srcA) >> 8;
                    srcG  = (srcG  * srcA) >> 8;

                    uint32_t outRB = (srcRB + dstRB) & 0x00FF00FF;
                    uint32_t outG  = (srcG  + dstG ) & 0x0000FF00;

                    dst = 0xFF000000 | outRB | outG;
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
            fullscreened,
            charin,
        };

        enum class key {
            A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
            Num0, Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9,
            Left, Right, Up, Down,
            Space, Enter, Escape, Tab, Backspace, Shift, Ctrl, Alt,
            F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
            none
        };

        enum class mouse {
            left, right, middle, none
        };

        struct event {
            eventTypes type;
            vec2 hit;
            key keys;
            mouse click;
            uint32_t KeyAsChar;
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
            if (!handle) return;
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

    class Font {
    private:
        HFONT hfont;
        HDC   hdc;
        HBITMAP hdib;
        void* bits;
        int   size;
        std::wstring name;

    public:
        const int lineHeight;

        Font(int fontSize = 16, const std::wstring& fontName = L"Consolas") 
        : hfont(nullptr),
        hdc(nullptr),
        hdib(nullptr),
        bits(nullptr),
        size(fontSize),
        name(fontName),
        lineHeight([&]() { // yes i hate you, what you gona do about it?
            SetMapMode(hdc, MM_TEXT);
            SetGraphicsMode(hdc, GM_COMPATIBLE);

            HFONT tempFont = CreateFontW(
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

            SelectObject(hdc, tempFont);

            TEXTMETRIC tm{};
            int h = fontSize;
            if (GetTextMetrics(hdc, &tm))
                h = tm.tmHeight;

            DeleteObject(tempFont);
            return h;
        }()) {
            hdc = CreateCompatibleDC(nullptr);

            SetMapMode(hdc, MM_TEXT);
            SetGraphicsMode(hdc, GM_COMPATIBLE);

            hfont = CreateFontW(
                -size,
                0, 0, 0,
                FW_NORMAL,
                FALSE, FALSE, FALSE,
                DEFAULT_CHARSET,
                OUT_DEFAULT_PRECIS,
                CLIP_DEFAULT_PRECIS,
                ANTIALIASED_QUALITY,
                DEFAULT_PITCH | FF_DONTCARE,
                name.c_str()
            );

            SelectObject(hdc, hfont);
        }

        ~Font() {
            if (hfont) {
                DeleteObject(hfont);
                hfont = nullptr;
            }
            if (hdib) {
                DeleteObject(hdib);
                hdib = nullptr;
            }
            if (hdc) {
                DeleteDC(hdc);
                hdc = nullptr;
            }
        }

        Surface render(const std::string& text, vec3 textColour, vec4 bgColour) {
            SIZE textSize{};
            GetTextExtentPoint32A(hdc, text.c_str(),
                                (int)text.length(), &textSize);

            Surface result({ (float)textSize.cx,
                            (float)textSize.cy });

            if (textSize.cx == 0 || textSize.cy == 0)
                return result;

            // Create DIB
            BITMAPINFOHEADER bih{};
            bih.biSize = sizeof(BITMAPINFOHEADER);
            bih.biWidth = textSize.cx;
            bih.biHeight = -textSize.cy;
            bih.biPlanes = 1;
            bih.biBitCount = 32;
            bih.biCompression = BI_RGB;

            if (hdib) DeleteObject(hdib);
            hdib = CreateDIBSection(hdc, (BITMAPINFO*)&bih, DIB_RGB_COLORS, &bits, nullptr, 0);
            if (!hdib || !bits) {
                return result;
            }
            SelectObject(hdc, hdib);

            // Clear DIB
            RECT rect = { 0, 0, textSize.cx, textSize.cy };
            HBRUSH clearBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
            FillRect(hdc, &rect, clearBrush);

            SetBkMode(hdc, TRANSPARENT);
            SetTextColor(hdc, RGB(255, 255, 255));

            TextOutA(hdc, 0, 0,
                    text.c_str(),
                    (int)text.length());

            // Read pixels from DIB
            uint32_t* src =
                reinterpret_cast<uint32_t*>(bits);

            size_t total =
                (size_t)textSize.cx *
                (size_t)textSize.cy;

            for (size_t i = 0; i < total; ++i) {

                uint32_t pixel = src[i];

                // GDI text is grayscale in R channel
                uint8_t coverage = (pixel >> 16) & 0xFF;

                if (coverage) {

                    uint32_t a = coverage;

                    uint32_t r =
                        ((uint32_t)textColour.x * a) >> 8;
                    uint32_t g =
                        ((uint32_t)textColour.y * a) >> 8;
                    uint32_t b =
                        ((uint32_t)textColour.z * a) >> 8;

                    result.pixels[i] =
                        (a << 24) |
                        (r << 16) |
                        (g << 8)  |
                        b;

                } else {

                    if (bgColour.w != 0) {

                        uint32_t a = (uint32_t)bgColour.w;

                        uint32_t r =
                            ((uint32_t)bgColour.x * a) >> 8;
                        uint32_t g =
                            ((uint32_t)bgColour.y * a) >> 8;
                        uint32_t b =
                            ((uint32_t)bgColour.z * a) >> 8;

                        result.pixels[i] =
                            (a << 24) |
                            (r << 16) |
                            (g << 8)  |
                            b;

                    } else {
                        result.pixels[i] = 0;
                    }
                }
            }
            result.hasAlpha = true;
            return result;
        }

        ivec2 sizeOf(const std::string& text) const {
            if (!hdc)
                return { 0, 0 };

            SIZE textSize{};
            if (!GetTextExtentPoint32A(
                    hdc,
                    text.c_str(),
                    (int)text.length(),
                    &textSize))
            {
                return { 0, 0 };
            }

            return { textSize.cx, textSize.cy };
        }

        void inline setSize(const int newSize) {
            size = newSize;
        }
    };

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

            uint32_t srcA = src >> 24;

            if (srcA == 255) {
                dst = src;
                return;
            }

            if (srcA == 0)
                return;

            uint32_t dstRB = dst & 0x00FF00FF;
            uint32_t dstG  = dst & 0x0000FF00;

            uint32_t srcRB = src & 0x00FF00FF;
            uint32_t srcG  = src & 0x0000FF00;

            uint32_t invA = 255 - srcA;

            // multiply packed channels
            dstRB = (dstRB * invA) >> 8;
            dstG  = (dstG  * invA) >> 8;

            srcRB = (srcRB * srcA) >> 8;
            srcG  = (srcG  * srcA) >> 8;

            uint32_t outRB = (srcRB + dstRB) & 0x00FF00FF;
            uint32_t outG  = (srcG  + dstG ) & 0x0000FF00;

            dst = 0xFF000000 | outRB | outG;
        }

        inline void blit(Surface& target, const Surface& source, vec2 position, bool blend = true) {
            target.blit(position, source, blend);
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
                for (int ty = -half; ty <= half; ++ty) {
                    int py = y0 + ty;
                    if (py < 0 || py >= surface.size.y) continue;

                    for (int tx = -half; tx <= half; ++tx) {
                        int px = x0 + tx;
                        if (px < 0 || px >= surface.size.x) continue;

                        surface.pixels[(size_t)py * surface.size.x + px] =
                            pack_colour(colour);
                    }
                }

                if (x0 == x1 && y0 == y1) break;

                int e2 = 2 * err;
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
                int w = (int)size.x;
                int h = (int)size.y;
                int px = (int)pos.x;
                int py = (int)pos.y;

                uint32_t packed = pack_colour(colour);

                for (int y = 0; y < h; ++y) {

                    int drawY = py + y;
                    if (drawY < 0 || drawY >= surface.size.y)
                        continue;

                    int startX = px;
                    int endX   = px + w;

                    if (endX < 0 || startX >= surface.size.x)
                        continue;

                    startX = std::max(startX, 0);
                    endX   = std::min(endX, surface.size.x);

                    uint32_t* row =
                        &surface.pixels[(size_t)drawY * surface.size.x + startX];

                    for (int x = startX; x < endX; ++x)
                        *row++ = packed;
                }
            }
    }

    inline void circle(Surface& surface, vec2 center, int radius, vec3 colour, bool filled = true) {
            int cx = (int)center.x;
            int cy = (int)center.y;

            int r2 = radius * radius;

            for (int y = -radius; y <= radius; ++y) {

                int yy = y * y;
                int xSpan = (int)std::sqrt((float)(r2 - yy));

                int drawY = cy + y;
                if (drawY < 0 || drawY >= surface.size.y)
                    continue;

                if (filled) {

                    int startX = cx - xSpan;
                    int endX   = cx + xSpan;

                    if (endX < 0 || startX >= surface.size.x)
                        continue;

                    startX = std::max(startX, 0);
                    endX   = std::min(endX, surface.size.x - 1);

                    uint32_t packed = pack_colour(colour);
                    uint32_t* row = &surface.pixels[(size_t)drawY * surface.size.x + startX];

                    for (int x = startX; x <= endX; ++x)
                        *row++ = packed;

                } else {

                    int x1 = cx - xSpan;
                    int x2 = cx + xSpan;

                    put_pixel(surface, x1, drawY, colour);
                    put_pixel(surface, x2, drawY, colour);
                }
            }
        }
    }

    inline events::key map_key(WPARAM keyCode) {
        switch (keyCode) {
            // Letters
            case 'A': return events::key::A;
            case 'B': return events::key::B;
            case 'C': return events::key::C;
            case 'D': return events::key::D;
            case 'E': return events::key::E;
            case 'F': return events::key::F;
            case 'G': return events::key::G;
            case 'H': return events::key::H;
            case 'I': return events::key::I;
            case 'J': return events::key::J;
            case 'K': return events::key::K;
            case 'L': return events::key::L;
            case 'M': return events::key::M;
            case 'N': return events::key::N;
            case 'O': return events::key::O;
            case 'P': return events::key::P;
            case 'Q': return events::key::Q;
            case 'R': return events::key::R;
            case 'S': return events::key::S;
            case 'T': return events::key::T;
            case 'U': return events::key::U;
            case 'V': return events::key::V;
            case 'W': return events::key::W;
            case 'X': return events::key::X;
            case 'Y': return events::key::Y;
            case 'Z': return events::key::Z;

            // Numbers (top row)
            case '0': return events::key::Num0;
            case '1': return events::key::Num1;
            case '2': return events::key::Num2;
            case '3': return events::key::Num3;
            case '4': return events::key::Num4;
            case '5': return events::key::Num5;
            case '6': return events::key::Num6;
            case '7': return events::key::Num7;
            case '8': return events::key::Num8;
            case '9': return events::key::Num9;

            // Arrows
            case VK_LEFT:  return events::key::Left;
            case VK_RIGHT: return events::key::Right;
            case VK_UP:    return events::key::Up;
            case VK_DOWN:  return events::key::Down;

            // Control keys
            case VK_SPACE:    return events::key::Space;
            case VK_RETURN:   return events::key::Enter;
            case VK_ESCAPE:   return events::key::Escape;
            case VK_TAB:      return events::key::Tab;
            case VK_BACK:     return events::key::Backspace;
            case VK_SHIFT:    return events::key::Shift;
            case VK_CONTROL:  return events::key::Ctrl;
            case VK_MENU:     return events::key::Alt;

            // Function keys
            case VK_F1:  return events::key::F1;
            case VK_F2:  return events::key::F2;
            case VK_F3:  return events::key::F3;
            case VK_F4:  return events::key::F4;
            case VK_F5:  return events::key::F5;
            case VK_F6:  return events::key::F6;
            case VK_F7:  return events::key::F7;
            case VK_F8:  return events::key::F8;
            case VK_F9:  return events::key::F9;
            case VK_F10: return events::key::F10;
            case VK_F11: return events::key::F11;
            case VK_F12: return events::key::F12;

            default:
                return events::key::none; // or define a `None`
        }
    }

    inline LRESULT CALLBACK wndproc(HWND handle, UINT message, WPARAM wparam, LPARAM lparam) {
        using namespace events;

        auto push_mouse = [&](eventTypes type, events::mouse btn) {
            vec2 p{ GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam) };
            queue().push_back({ type, p, events::key::none, btn, (wchar_t)0 });
        };

        auto push_key = [&](eventTypes type) {
            queue().push_back({ type, { 0, 0 }, map_key(wparam), events::mouse::none, (wchar_t)0 });
        };

        switch (message) {
            case WM_CLOSE:
                DestroyWindow(handle);
                return 0;

            case WM_DESTROY:
                queue().push_back({ eventTypes::quit, { 0, 0 }, events::key::none, events::mouse::none, (wchar_t)0 });
                PostQuitMessage(0);
                return 0;

            case WM_KEYDOWN:
                push_key(eventTypes::key_down);
                return 0;

            case WM_KEYUP:
                push_key(eventTypes::key_up);
                return 0;

            case WM_LBUTTONDOWN:
                push_mouse(eventTypes::mouse_down, events::mouse::left);
                return 0;

            case WM_LBUTTONUP:
                push_mouse(eventTypes::mouse_up, events::mouse::left);
                return 0;

            case WM_RBUTTONDOWN:
                push_mouse(eventTypes::mouse_down, events::mouse::right);
                return 0;

            case WM_RBUTTONUP:
                push_mouse(eventTypes::mouse_up, events::mouse::right);
                return 0;

            case WM_MBUTTONDOWN:
                push_mouse(eventTypes::mouse_down, events::mouse::middle);
                return 0;

            case WM_MBUTTONUP:
                push_mouse(eventTypes::mouse_up, events::mouse::middle);
                return 0;

            case WM_MOUSEMOVE: {
                vec2 p{ GET_X_LPARAM(lparam), GET_Y_LPARAM(lparam) };
                internal_mouse() = p;
                queue().push_back({ eventTypes::mouse_move, p, events::key::none, events::mouse::none, (wchar_t)0 });
                return 0;
            }

            case WM_MOUSEWHEEL: {
                int delta = GET_WHEEL_DELTA_WPARAM(wparam);
                queue().push_back({
                    delta > 0 ? eventTypes::scroll_wheel_up : eventTypes::scroll_wheel_down,
                    { 0, (float)delta },
                    events::key::none,
                    events::mouse::none,
                    (wchar_t)0
                });
                return 0;
            }

            case WM_CHAR: {
                queue().push_back({
                    eventTypes::charin,
                    { 0, 0 },
                    events::key::none,
                    events::mouse::none,
                    (wchar_t)wparam
                });
                return 0;
            }

            default:
                return DefWindowProcW(handle, message, wparam, lparam);
        }
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