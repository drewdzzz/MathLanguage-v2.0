#ifndef FREE_ACC_STACK_H
#define FREE_ACC_STACK_H

namespace FreeAccessStack_values {
    const size_t PRIMARY_SIZE = 10;
};

template<typename DataType>
class FreeAccessStack {
public:

    FreeAccessStack() {
        data = (DataType*)calloc(FreeAccessStack_values::PRIMARY_SIZE, sizeof(DataType));
        allocated_size = FreeAccessStack_values::PRIMARY_SIZE;
        available_size = 0;
    }

    ~FreeAccessStack() {
        free(data);
    }

    void push_back(DataType new_val) {
        if (allocated_size == available_size) {
            available_size *= 2;
            data = (DataType*)realloc(data, available_size * sizeof(DataType));
        }

        data[available_size++] = new_val;
    }

    void resize(size_t new_size) {
        allocated_size = new_size;
        data = (DataType*)realloc(data, new_size * sizeof(DataType));

        available_size = new_size;        
    }

    DataType& operator[](size_t pos) {
        return data[pos];
    }

    size_t size() {
        return available_size;
    }

private:
    DataType* data;
    size_t allocated_size;
    size_t available_size;
};

#endif