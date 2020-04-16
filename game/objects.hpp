#ifndef OBJECTS_HPP
#define OBJECTS_HPP

#include <vector>
#include <cassert>
#include <stdexcept>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Triangle {
    std::vector<glm::vec3>& points;

public:
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

    std::vector<glm::vec3>& get_points() const {
        return points;
    }
};


class Buffer {
    std::vector<GLfloat> data;
public:
    Buffer() {}

    std::vector<GLfloat>& data() const {
        return data;
    }

    size_t size() const {
        return data.size();
    }

    size_t add(const std::vector<Triangle>& triangles) {
        size_t begin = data.size();
        for (const auto& triangle: triangles) {
            for (const auto& point : triangle) {
                for (const auto& coordinate : point) {
                    data.emplace_back(coordinate);
                }
            }
        }
        return begin;
    }

    void remove(size_t begin, size_t length) {
        data.erase(data.bebin() + begin, data.begin() + begin + 9 * length);
    };
};


class Object {
    std::vector<Triangle> triangles;
    size_t begin;
public:
    Object() {}

    Object(const std::vector<Triangle>& triangles) : triangles(triangles) {}

    void draw(Buffer& buffer) const {
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
                }))
            }
        }
    }
};


class Target : public Object {
};

#endif //OBJECTS_HPP
