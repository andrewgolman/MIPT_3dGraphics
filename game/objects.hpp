#ifndef OBJECTS_HPP
#define OBJECTS_HPP

#include <vector>

class Buffer {
    std::vector<GLfloat> data;
    std::vector<size_t> offset;
    std::vector<size_t> size;
    size_t next_id = 0;
public:
    Buffer() {}
    std::vector<GLfloat>& get_data() const {
        return data;
    }
    size_t size() const {
        return data.size();
    }
    size_t add(vector<Triangle>& triangles) {

    }
    void update(vector<Triangle>& triangles) {

    }
    void remove(size_t id) {

    };
};

class Object {
    size_t id;
public:
    void draw(Buffer* buffer); // pass triangles to buffer
    void undraw(Buffer* buffer); // remove from buffer
    void rotate(); // e.g. by angle
    void move(vec3);
};

class Target : public Object {
    size_t id;
public:
    void add(Buffer* buffer) {

    }
    void remove(Buffer* buffer) {

    }
};

#endif //OBJECTS_HPP
