#include <random>
#include <vector>
#include <cmath>
#include "../../src/ver2/winhelp.hpp"
using namespace winhelp;

constexpr int width = 900;
constexpr int height = 600;
constexpr int startingHeight = static_cast<int>(height * ((100.0f - 36.0f) / 100.0f));
constexpr int chartBufferSpace = 150;

struct data {
    int number;
    vec3 colour;
};

static std::vector<data> sequence;
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
        int num = randint(0, 100);
        vec3 colour = {randint(0, 255), randint(0, 255), randint(0, 255)};
        if (num > biggestNum) {
            biggestNum = num;
        }
        sequence.push_back({num, colour});
    }

    float percentToFit = ((height * 2) - startingHeight) / (biggestNum + chartBufferSpace);

    for (int i = 0; i != size; i++) {
        sequence[i].number *= percentToFit;
    }

    thickness = (float)width / (float)size;
}

void exchange(size_t indexA, size_t indexB) {
    data temp = sequence[indexA];
    sequence[indexA] = sequence[indexB];
    sequence[indexB] = temp;
}

void shuffleSequence() {
    for (int i = 0; i != sequence.size(); i++) {
        exchange(i, randint(0, sequence.size()));
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

int main() {
    newSequence(10);
    display d({width, height}, "Random Sequence");


    Surface text = multiLineText(
        L"Press [Space] to shuffle sequence\n"
        L"Scroll [Up] to add a number\n"
        L"Scroll [Down] to remove a number\n"
    );

    while (true) {
        for (auto& e : events::get()) {
            if (e.type == events::eventTypes::quit) {
                d.close();
                return 0;
            }
            if (e.type == events::eventTypes::scroll_wheel_up) {
                newSequence(sequence.size() + 1);
            }
            if (e.type == events::eventTypes::scroll_wheel_down) {
                if (not sequence.size() <= 3) {
                    newSequence(sequence.size() - 1);   
                }
            }
            if (e.type == events::eventTypes::key_up) {
                for (winhelp::events::key key : e.keys) {
                    if (key == winhelp::events::key::Space) {
                        shuffleSequence();
                    }
                }
            }
        }

        d.surface.fill(vec3(255, 255, 255));

        for (int i = 0; i != sequence.size(); i++) {
            draw::rect(d.surface, {((float)i * thickness), startingHeight}, {thickness, sequence[i].number}, sequence[i].colour);
        }

        d.surface.blit({0, startingHeight+5}, text);

        d.flip();
        tick(60);
    }
}