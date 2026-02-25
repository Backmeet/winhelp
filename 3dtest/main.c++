#include "../src/winhelp.hpp"
#include "render3d/render3d.hpp"

using namespace winhelp;
using namespace render3d;


int main() {
    display d({800, 600}, "3d test");
    Renderer renderer(60.0);
    renderer.cameraPos = {0.0f, 0.0f, -5.0f};
    renderer.addObject(create::cube(1.0f, {255, 0, 0}));

    while (true) {
        for (auto& e : events::get()) {
            if (e.type == events::eventTypes::quit) {
                return 0;
            }
            if (e.type == events::eventTypes::key_down) {
                for (auto& k : e.keys) {
                    if (k == events::key::Escape) {
                        return 0;
                    }
                    if (k == events::key::W) {
                        renderer.cameraPos.z += 0.1f;
                    }
                    if (k == events::key::S) {
                        renderer.cameraPos.z -= 0.1f;
                    }
                    if (k == events::key::A) {
                        renderer.cameraPos.x -= 0.1f;
                    }
                    if (k == events::key::D) {
                        renderer.cameraPos.x += 0.1f;
                    }
                    if (k == events::key::Space) {
                        renderer.cameraPos.y -= 0.1f;
                    }
                    if (k == events::key::Shift) {
                        renderer.cameraPos.y += 0.1f;
                    }
                }
            }
        }

        d.surface.fill({30,30,40});
        renderer.render(d.surface);
        d.flip();
        
    }

}