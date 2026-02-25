#include <vector>
#include <array>
#include <cmath>
#include "../../src/winhelp.hpp"

namespace render3d {

    using winhelp::vec2;
    using winhelp::vec3;
    using winhelp::Surface;

    using Points3D = std::vector<vec3>;
    using Lines2D  = std::vector<std::array<vec2, 2>>;

    struct Face {
        Points3D points;
        vec3 colour;
        float avgZ;

        Face() : points{}, colour{255, 255, 255}, avgZ(0.0f) {}

        Face(const Points3D &pts, const vec3 &col)
            : points(pts), colour(col), avgZ(0.0f) {
            recalcAvgZ();
        }

        inline void recalcAvgZ() {
            if (points.empty()) {
                avgZ = 0.0f;
                return;
            }
            float sum = 0.0f;
            for (const auto &p : points) sum += p.z;
            avgZ = sum / points.size();
        }
    };

    struct Object {
        std::vector<Face> faces;

        Object() : faces{} {}
        Object(const std::vector<Face> &f) : faces(f) {}
    };

    struct Renderer {
        std::vector<Object> objects;
        vec3 cameraPos;
        vec3 cameraRot;
        float fov;

        Renderer(float fieldOfView = 500.0f)
            : objects{}, cameraPos{0, 0, 0}, cameraRot{0, 0, 0}, fov(fieldOfView)
            {}

        vec3 rotateEuler(const vec3 &p) const {
            float cx = std::cos(cameraRot.x);
            float sx = std::sin(cameraRot.x);
            float cy = std::cos(cameraRot.y);
            float sy = std::sin(cameraRot.y);
            float cz = std::cos(cameraRot.z);
            float sz = std::sin(cameraRot.z);

            vec3 r = p;

            r = {
                r.x,
                r.y * cx - r.z * sx,
                r.y * sx + r.z * cx
            };

            r = {
                r.x * cy + r.z * sy,
                r.y,
                -r.x * sy + r.z * cy
            };

            r = {
                r.x * cz - r.y * sz,
                r.x * sz + r.y * cz,
                r.z
            };

            return r;
        }

        vec2 to2D(const vec3 &p) const {
            vec3 r = {
                p.x - cameraPos.x,
                p.y - cameraPos.y,
                p.z - cameraPos.z
            };

            r = rotateEuler(r);

            if (r.z == 0.0f) r.z = 0.0001f;

            return {
                (r.x / r.z) * fov,
                (r.y / r.z) * fov
            };
        }

        Lines2D getFaceLines2D(const Face &face) const {
            Lines2D lines;
            if (face.points.size() < 2) return lines;

            for (size_t i = 0; i < face.points.size(); i++) {
                const vec3 &p1 = face.points[i];
                const vec3 &p2 = face.points[(i + 1) % face.points.size()];
                lines.push_back({ to2D(p1), to2D(p2) });
            }
            return lines;
        }

        void render(Surface &surface) {
            std::vector<const Face *> zSortedFaces;

            for (const Object &obj : objects) {
                for (const Face &face : obj.faces) {
                    bool inserted = false;
                    for (size_t i = 0; i < zSortedFaces.size(); i++) {
                        if (face.avgZ > zSortedFaces[i]->avgZ) {
                            zSortedFaces.insert(zSortedFaces.begin() + i, &face);
                            inserted = true;
                            break;
                        }
                    }
                    if (!inserted) {
                        zSortedFaces.push_back(&face);
                    }
                }
            }

            for (const Face *face : zSortedFaces) {
                Lines2D lines = getFaceLines2D(*face);
                winhelp::draw::polygon(surface, lines, face->colour);
            }
        }

        inline void addObject(const Object &obj) {
            objects.push_back(obj);
        }
    };

    namespace create {

        inline Object cube(float size, const vec3 &colour) {
            std::vector<Face> faces = {
                Face({{-size,-size,-size},{ size,-size,-size},{ size, size,-size},{-size, size,-size}}, colour),
                Face({{-size,-size, size},{ size,-size, size},{ size, size, size},{-size, size, size}}, colour),
                Face({{-size,-size,-size},{-size, size,-size},{-size, size, size},{-size,-size, size}}, colour),
                Face({{ size,-size,-size},{ size, size,-size},{ size, size, size},{ size,-size, size}}, colour),
                Face({{-size,-size,-size},{ size,-size,-size},{ size,-size, size},{-size,-size, size}}, colour),
                Face({{-size, size,-size},{ size, size,-size},{ size, size, size},{-size, size, size}}, colour)
            };
            return Object(faces);
        }

    }

}
