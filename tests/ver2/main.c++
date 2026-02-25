#include <array>
#include <cmath>
#include "../../src/ver2/winhelp.hpp"
using namespace winhelp;

constexpr int numCircles = 12;
constexpr int width = 1000;
constexpr int height = 1000;

constexpr float baseRadius = 60.0f;
constexpr float radiusDecay = 0.85f;        // each next circle smaller
constexpr float speedBase = 0.02f;          // base angular speed

int main() {
    display d({width, height}, "Spline Pivot Pattern");

    std::array<vec2, numCircles> positions;
    std::array<float, numCircles> angles;

    for (int i = 0; i < numCircles; ++i) {
        angles[i] = i * 0.5f;
    }

    while (true) {
        for (auto& e : events::get()) {
            if (e.type == events::eventTypes::quit)
                return 0;
        }

        d.surface.fill(vec3(20, 20, 30));

        vec2 center = { width / 2.0f, height / 2.0f };
        positions[0] = center;

        float currentRadius = baseRadius;

        for (int i = 1; i < numCircles; ++i) {
            float armLength = currentRadius * 1.8f;

            positions[i] = {
                positions[i - 1].x + std::cos(angles[i]) * armLength,
                positions[i - 1].y + std::sin(angles[i]) * armLength
            };

            currentRadius *= radiusDecay;
        }

        currentRadius = baseRadius;

        for (int i = 0; i < numCircles; ++i) {
            draw::circle(d.surface, positions[i], currentRadius, {255, 120 - i * 6, 120 + i * 8});
            currentRadius *= radiusDecay;
        }

        for (int i = 1; i < numCircles; ++i) {
            angles[i] += speedBase * (1.0f + i * 0.15f);
        }

        d.flip();
        tick(60);
    }
}