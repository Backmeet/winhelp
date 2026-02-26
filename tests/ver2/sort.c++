#include <random>
#include <vector>
#include <cmath>
#include <optional>
#include <cstddef>
#include <iostream>
#include "../../src/ver2/winhelp.hpp"
using namespace winhelp;

constexpr int width = 900;
constexpr int height = 600;
constexpr int startingHeight = static_cast<int>(height * ((100.0f - 36.0f) / 100.0f)); // some % shnanigins
constexpr int chartBufferSpace = 305;

struct BubbleState {
    std::size_t pass;      // how many completed passes
    bool swapped_last;     // whether last pass swapped anything
};

std::optional<BubbleState> bubble_sort_step(std::vector<int>& v, std::optional<BubbleState> state) {
    BubbleState current;

    if (!state.has_value()) {
        current.pass = 0;
        current.swapped_last = true;
    } else {
        current = state.value();
    }

    if (v.size() < 2) {
        return std::nullopt;
    }

    if (!current.swapped_last) {
        return std::nullopt;
    }

    if (current.pass >= v.size() - 1) {
        return std::nullopt;
    }

    bool swapped = false;
    std::size_t limit = v.size() - current.pass - 1;

    for (std::size_t i = 0; i < limit; ++i) {
        if (v[i] > v[i + 1]) {
            int tmp = v[i];
            v[i] = v[i + 1];
            v[i + 1] = tmp;
            swapped = true;
        }
    }

    BubbleState next;
    next.pass = current.pass + 1;
    next.swapped_last = swapped;

    if (!swapped) {
        return std::nullopt;
    }

    return next;
}

static std::vector<int> sequence;
static float thickness = 0;

int randint(int min, int max) {
    static std::mt19937 rng(std::random_device{}()); 
    std::uniform_int_distribution<int> dist(min, max);
    return dist(rng);
}

void newSequence(size_t size) {
    sequence.clear();
    int biggestNum = 0;
    
    for (int i = 0; i != size; i++) {
        int num = randint(0, 255);
        
        if (num > biggestNum) {
            biggestNum = num;
        }
        sequence.push_back(num);
    }

    float percentToFit = ((height * 2) - startingHeight) / (biggestNum + chartBufferSpace);

    for (int i = 0; i != size; i++) {
        sequence[i] *= percentToFit;
    }

    thickness = (float)width / (float)size;
}

void exchange(size_t indexA, size_t indexB) {
    int temp = sequence[indexA];
    sequence[indexA] = sequence[indexB];
    sequence[indexB] = temp;
}

void shuffleSequence() {
    for (size_t i = 0; i != sequence.size(); i++) {
        exchange(i, randint(0, sequence.size()-1));
    }
}

std::vector<std::wstring> splitlines(const std::wstring& s) {
    std::vector<std::wstring> result;

    std::size_t start = 0;
    std::size_t pos = 0;

    while ((pos = s.find(L'\n', start)) != std::wstring::npos) {
        result.push_back(s.substr(start, pos - start));
        start = pos + 1;
    }

    result.push_back(s.substr(start));
    return result;
}

/*
Surface multiLineText(std::wstring text) {
    std::vector<std::wstring> lines = splitlines(text);

    size_t longest = 0;
    for (size_t i = 0; i < lines.size(); i++) {
        if (lines[i].length() > longest)
            longest = lines[i].length();
    }

    Surface texts({16 * longest, 32 * lines.size()});
    texts.fill({255, 255, 255, 255});

    for (size_t i = 0; i < lines.size(); i++) {
        Surface lineSurface = font::text(lines[i], {0, 0, 0, 255}, {255, 255, 255, 0});
        texts.blit({0, (int)(i * 32)}, lineSurface);
    }

    return texts;
}
*/

int main() {
    newSequence(10);
    display d({width, height}, "Random Sequence");
    const int fontSize = 32;
    const vec2 Line1 = {10, startingHeight+5};
    const vec2 Line2 = {10, startingHeight+5+fontSize};
    const vec2 Line3 = {10, startingHeight+5+(fontSize*2)};
    const vec2 Line4 = {10, startingHeight+5+(fontSize*3)};

    const Surface SpacePressed     = font::text(L"Press (Space) to shuffle sequence"  , {0, 0, 0, 255}, {0, 0, 0, 0}, fontSize);
    const Surface ScrollingUp      = font::text(L"Scroll (Up) to add a number"        , {0, 0, 0, 255}, {0, 0, 0, 0}, fontSize);
    const Surface ScrollingDown    = font::text(L"Scroll (Down) to remove a number"   , {0, 0, 0, 255}, {0, 0, 0, 0}, fontSize);
    const Surface SpaceNotPressed  = font::text(L"Press [[Space]] to shuffle sequence", {0, 0, 0, 255}, {0, 0, 0, 0}, fontSize);
    const Surface NotScrollingUp   = font::text(L"Scroll [[Up]] to add a number"      , {0, 0, 0, 255}, {0, 0, 0, 0}, fontSize);
    const Surface NotScrollingDown = font::text(L"Scroll [[Down]] to remove a number" , {0, 0, 0, 255}, {0, 0, 0, 0}, fontSize);
    const Surface EnterPressed     = font::text(L"Press (Enter) to sort sequence"     , {0, 0, 0, 255}, {0, 0, 0, 0}, fontSize);
    const Surface EnterNotPressed     = font::text(L"Press [[Enter]] to sort sequence"     , {0, 0, 0, 255}, {0, 0, 0, 0}, fontSize);

    bool scrollingUp = false;
    bool scrollingDown = false;
    bool spaceHeld = false;
    bool enterHeld = false;

    bool bubbling = false;
    std::optional<BubbleState> lastState = std::nullopt;


    while (true) {
        d.surface.fill(vec3(255, 255, 255));

        for (auto& e : events::get()) {
            if (e.type == events::eventTypes::quit) {
                d.close();
                return 0;
            }
            if (e.type == events::eventTypes::scroll_wheel_up) {
                newSequence(sequence.size() + 1);
                scrollingUp = true;
            }
            if (e.type == events::eventTypes::scroll_wheel_down) {
                if (sequence.size() > 3) {
                    newSequence(sequence.size() - 1); 
                }
                scrollingDown = true;
            }
            if (e.type == events::eventTypes::key_down) {
                for (winhelp::events::key key : e.keys) {
                    if (key == winhelp::events::key::Space) {
                        spaceHeld = true;
                    }
                    if (key == winhelp::events::key::Enter) {
                        enterHeld = true;
                    }
                }
            }
            if (e.type == events::eventTypes::key_up) {
                for (winhelp::events::key key : e.keys) {
                    if (key == winhelp::events::key::Space) {
                        shuffleSequence();
                        spaceHeld = false;
                    } if (key == winhelp::events::key::Enter) {
                        enterHeld = false;
                        if (not bubbling) bubbling = true;
                    }
                }
            }
        }


        for (int i = 0; i != sequence.size(); i++) {
            draw::rect(d.surface, {((float)i * thickness), startingHeight}, {thickness, sequence[i]}, {255-sequence[i], sequence[i], 0});
        }

        // Line 1 (Space state)
        if (spaceHeld)
            d.surface.blit(Line1, SpacePressed);
        else
            d.surface.blit(Line1, SpaceNotPressed);

        // Line 2 (Scroll up state)
        if (scrollingUp)
            d.surface.blit(Line2, ScrollingUp);
        else
            d.surface.blit(Line2, NotScrollingUp);

        // Line 3 (Scroll down state)
        if (scrollingDown)
            d.surface.blit(Line3, ScrollingDown);
        else
            d.surface.blit(Line3, NotScrollingDown);

        // Line 4 (Enter state)
        if (enterHeld)
            d.surface.blit(Line4, EnterPressed);
        else
            d.surface.blit(Line4, EnterNotPressed);

        scrollingUp = false;
        scrollingDown = false;

        if (bubbling) {
            std::optional<BubbleState> state = bubble_sort_step(sequence, lastState);
            if (!state.has_value()) {
                bubbling = false;
            }
            lastState = state;
        }

        d.flip();
        winhelp::tick();
    }
}