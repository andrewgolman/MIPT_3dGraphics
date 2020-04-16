#ifndef OBJECTS_HPP
#define OBJECTS_HPP

#include <vector>

class Buffer {
    std::vector<GLfloat> data;
    std::vector<size_t> offset;
    size_t next_id = 0;
public:
    Buffer() {}
    std::vector<GLfloat> get_data() const {
        return data;
    }
    size_t size() const {
        return data.size();
    }
    size_t add() {

    }
    void remove(size_t id) {

    };
};

class Target {
    Target() {}
    void add(Buffer* buffer) {

    }
    void remove(Buffer* buffer) {
        
    }
};

#endif //OBJECTS_HPP
