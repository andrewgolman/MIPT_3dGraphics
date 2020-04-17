#ifndef OBJECTS_HPP
#define OBJECTS_HPP

#include <vector>
#include <cassert>
#include <stdexcept>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Triangle {
    std::vector<glm::vec3> points;

public:
    Triangle(const std::vector<GLfloat>& data) {
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

    void stretch(GLfloat alpha) {
        for (auto& point : points) {
            point *= alpha;
        }
    }

    void turn(const glm::vec3& angle) {

    }

    const std::vector<glm::vec3>& get_points() const {
        return points;
    }
};


class Buffer {
    std::vector<GLfloat> _vertex_data;
    std::vector<GLfloat> _color_data;
public:
    Buffer() {}

    void clear() {
        _vertex_data.clear();
        _color_data.clear();
    }

    const void* vertex_data() {
        return _vertex_data.data();
    }

    const void* color_data() {
        return _color_data.data();
    }

    size_t size() const {
        assert(_vertex_data.size() == _color_data.size());
        return _vertex_data.size();
    }

    size_t add(const std::vector<Triangle>& triangles, const std::vector<GLfloat>& color) {
        assert(color.size() == 3);
        size_t begin = _vertex_data.size();
        for (auto& triangle: triangles) {
            for (const auto& point : triangle.get_points()) {
                _vertex_data.emplace_back(point.x);
                _vertex_data.emplace_back(point.y);
                _vertex_data.emplace_back(point.z);
            }
            for (int i = 0; i < 3; ++i) {
                for (auto comp : color) {
                    _color_data.emplace_back(comp);
                }
            }
        }
        return begin;
    }
};


class Object {
protected:
    std::vector<Triangle> triangles;
    std::vector<GLfloat> color;
    size_t begin;
    Object() = default;
public:
    Object(const std::vector<Triangle>& triangles, const std::vector<GLfloat>& color)
            : triangles(triangles), color(color) {}

    void draw(Buffer& buffer) {
        begin = buffer.add(triangles, color);
    }

    void move(const glm::vec3& shift) {
        for (auto& triangle : triangles) {
            triangle.move(shift);
        }
    }
};

class Floor : public Object {
    static constexpr GLfloat FIELD_SIZE = 5.0f;
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
        color = {0.7, 0.5, 0.2};
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

const std::vector<Triangle> CUBE_TRIANGLES = {
        Triangle({-1.0f, -1.0f,-1.0f,
        -1.0f,-1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f}),
        Triangle({1.0f, 1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f}),
        Triangle({1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        1.0f,-1.0f,-1.0f}),
        Triangle({1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f}),
        Triangle({-1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f}),
        Triangle({1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f}),
        Triangle({-1.0f, 1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        1.0f,-1.0f, 1.0f}),
        Triangle({1.0f, 1.0f, 1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f}),
        Triangle({1.0f,-1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f}),
        Triangle({1.0f, 1.0f, 1.0f,
        1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f}),
        Triangle({1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f}),
        Triangle({1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f})
};


class Target : public Object {
public:
    Target(const glm::vec3& center,
            GLfloat radius,
            const glm::vec3& angle,
            const std::vector<GLfloat>& icolor) {
        triangles = std::vector<Triangle>(CUBE_TRIANGLES);
        color = icolor;
        for (auto& t : triangles) {
            t.stretch(radius);
            t.turn(angle);
            t.move(center);
        }
    }
};

#endif //OBJECTS_HPP
