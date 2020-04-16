#ifndef OBJECTS_HPP
#define OBJECTS_HPP

#include <vector>
#include <cassert>
#include <stdexcept>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Triangle {
    std::vector<glm::vec3> points;

public:
    Triangle(const std::vector<float>& data) {
        assert(data.size() == 9);
        for (size_t i = 0; i < 3; ++i) {
            auto iter = data.begin() + 3 * i;
            points.emplace_back(
                    glm::vec3(*iter, *(iter+1), *(iter+2))
            );
        }
    }

    Triangle(const std::vector<glm::vec3>& points) : points(points) {
        assert(points.size() == 3);
    }

    Triangle(std::vector<glm::vec3>&& points) : points(std::move(points)) {
        assert(points.size() == 3);
    }

    void move(const glm::vec3& shift) {
        for (auto& point : points) {
            point += shift;
        }
    }

    const std::vector<glm::vec3>& get_points() const {
        return points;
    }
};


class Buffer {
    std::vector<GLfloat> _vertex_data;
public:
    Buffer() {}

    const void* vertex_data() {
        return _vertex_data.data();
    }

    const void* color_data() {
        throw;
    }

    size_t size() const {
        return _vertex_data.size();
    }

    size_t add(const std::vector<Triangle>& triangles) {
        size_t begin = _vertex_data.size();
        for (auto& triangle: triangles) {
            for (const auto& point : triangle.get_points()) {
                    _vertex_data.emplace_back(point.x);
                    _vertex_data.emplace_back(point.y);
                    _vertex_data.emplace_back(point.z);
            }
        }
        return begin;
    }

    void remove(size_t begin, size_t length) {
        _vertex_data.erase(_vertex_data.begin() + begin, _vertex_data.begin() + begin + 9 * length);
    };
};


class Object {
protected:
    std::vector<Triangle> triangles;
    size_t begin;
public:
    Object() = default;

    Object(const std::vector<Triangle>& triangles) : triangles(triangles) {}

    void draw(Buffer& buffer) {
        begin = buffer.add(triangles);
    }

    void undraw(Buffer& buffer) const {
        buffer.remove(begin, triangles.size());
    }

    void move(const glm::vec3& shift) {
        for (auto& triangle : triangles) {
            triangle.move(shift);
        }
    }
};

class Floor : public Object {
    static constexpr float FIELD_SIZE = 5.0f;
public:
    Floor() {
        auto t1 = Triangle({
                   -FIELD_SIZE, 0.0f, -FIELD_SIZE,
                   FIELD_SIZE, 0.0f,  FIELD_SIZE,
                   FIELD_SIZE, 0.0f, -FIELD_SIZE,
        });
        auto t2 = Triangle({
                   FIELD_SIZE, 0.0f,  FIELD_SIZE,
                   -FIELD_SIZE, 0.0f, -FIELD_SIZE,
                   -FIELD_SIZE, 0.0f,  FIELD_SIZE,
        });
        triangles = {t1, t2};
    }
};


class Fireball : public Object {
public:
    Fireball(double radius, size_t triangles_count) {
        const size_t squares_count = triangles_count / 2;
        for (size_t i = 0; i < squares_count; ++i) {
            double theta = (double)glm::pi<double>() * i / squares_count;
            double theta1 = (double)glm::pi<double>() * (i + 1) / squares_count;

            for (size_t j = 0; j < triangles_count; ++j)
            {
                double phi = 2.0f * (double)glm::pi<double>() * j / triangles_count
                    + (double)glm::pi<double>();
                double phi1 = 2.0f * (double)glm::pi<double>() * (j + 1) / triangles_count
                    + (double)glm::pi<double>();

                //Первый треугольник
                triangles.emplace_back(Triangle(std::vector<glm::vec3>{
                    glm::vec3(
                        cos(phi) * sin(theta) * radius,
                        sin(phi) * sin(theta) * radius,
                        cos(theta) * radius
                        ),
                    glm::vec3(
                        cos(phi1) * sin(theta1) * radius,
                        sin(phi1) * sin(theta1) * radius,
                        cos(theta1) * radius
                        ),
                    glm::vec3(
                        cos(phi1) * sin(theta) * radius,
                        sin(phi1) * sin(theta) * radius,
                        cos(theta) * radius
                        )
                }));

                //Второй треугольник
                triangles.emplace_back(Triangle(std::vector<glm::vec3>{
                    glm::vec3(
                        cos(phi) * sin(theta) * radius,
                        sin(phi) * sin(theta) * radius,
                        cos(theta) * radius
                        ),
                    glm::vec3(
                        cos(phi) * sin(theta1) * radius,
                        sin(phi) * sin(theta1) * radius,
                        cos(theta1) * radius
                        ),
                    glm::vec3(
                        cos(phi1) * sin(theta1) * radius,
                        sin(phi1) * sin(theta1) * radius,
                        cos(theta1) * radius
                        )
                }));
            }
        }
    }
};


class Target : public Object {
};

#endif //OBJECTS_HPP
