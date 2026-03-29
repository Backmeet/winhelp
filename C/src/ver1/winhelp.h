#pragma once
#include <windows.h>
#include <windowsx.h>
#include <string.h>

#include "../utils/utils.c"
#include "../utils/vectors.c"
#include "../utils/surface.c"
#include "../utils/display.c"
#include "../utils/draw.c"
#include "../utils/internal_timing.c"
#include "../utils/events.c"
#include "../utils/font.c"

static inline void WinhelpInit() {
    EventsInit();
}

static inline Array* WinhelpUpdate() {
    return EventsGet();
}

static inline void WinhelpQuit(display* d) {
    if (d) DisplayFree(d);
}
