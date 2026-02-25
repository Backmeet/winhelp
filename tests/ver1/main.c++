#include "../../src/ver1/winhelp.hpp"
using namespace winhelp;

vec2 circlePos {400,300};

int main() {
    display d({800,600}, "Test");

    while (true) {
        for (auto& e : events::get()) {
            if (e.type == events::eventTypes::quit) {
                return 0;
            }
            if (e.type == events::eventTypes::mouse_move) {
                circlePos = e.hit;
            }
        }

        d.surface.fill({30,30,40});
        draw::circle(d.surface, circlePos, 100, {255,0,0});
        d.flip();
        
    }
}
