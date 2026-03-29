#include "winhelp.h"

int main() {
    WinhelpInit();
    vec2 size = {640, 480};
    display *d = DisplayInit(size, "C Display Test");
    Font* font = FontInit(18, "Consolas");

    if (!d || !font) {
        if (d) DisplayFree(d);
        if (font) FontFree(font);
        return 1;
    }

    while (1) {
        Array* events = WinhelpUpdate();
        for (size_t i = 0; i != events->size; i++) {
            Event* event = (Event*)Arrayindex(events, i);
            if (event->type == quit) {
                DisplayFree(d);
                puts("Bye-Bye!");
                return 0;
            }
        }
        SurfaceFill(d->surface, (vec4){0, 0, 0, 255});

        Surface* text = FontRender(font, "Hello world", (vec3){255, 255, 255}, (vec4){0,0,0,0});
        if (text) {
            DrawBlit(d->surface, text, (vec2){20, 20}, true);
            SurfaceFree(text);
        }

        DrawCircle(d->surface, (vec2){320, 240}, 10, (vec3){255, 255, 255}, true);
        DisplayFlip(d);
        tick();
    }

    FontFree(font);
    DisplayFree(d);
    return 0;
}
