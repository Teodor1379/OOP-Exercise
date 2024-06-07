#ifndef VECTOR_H
#define VECTOR_H

#include <exception>
#include <stdexcept>

template<typename T>
class Vector {
public:
    Vector();
    Vector(const Vector<T>& instance);

    ~Vector();

    Vector<T>& operator=(const Vector<T>& instance);

    T& operator[](const std::size_t index)              { return this->data[index]; } // No-throw guarantee and undefined behaviour in case of invalid index.
    const T& operator[](const std::size_t index) const  { return this->data[index]; } // No-throw guarantee and undefined behaviour in case of invalid index.

    std::size_t getSize()       const noexcept { return this->size;     }
    std::size_t getCapacity()   const noexcept { return this->capacity; }

    void push_back(const T& value);
    void pop_back();

private:
    void allocate(const Vector<T>& instance);
    void deallocate();
    
    void resize();

    T*          data;
    std::size_t size;
    std::size_t capacity;
};

template<typename T>
inline Vector<T>::Vector() {
    this->data      = new T[2];
    this->size      = 0;
    this->capacity  = 2;
}

template<typename T>
inline Vector<T>::Vector(const Vector<T>& instance) {
    this->allocate(instance);

    this->size      = instance.size;
    this->capacity  = instance.capacity;
}

template<typename T>
inline Vector<T>::~Vector() {
    this->deallocate();
}

template<typename T>
inline Vector<T>& Vector<T>::operator=(const Vector<T>& instance) {
    if (this != &instance) {
        T* temporary = new T[instance.capacity];

        try {
            for (std::size_t i = 0; i < instance.size; ++i) {
                temporary[i] = instance.data[i];
            }
        } catch (std::exception& exception) {
            delete[] temporary;
            throw exception;
        }

        delete[] this->data;
        this->data = temporary;

        this->size      = instance.size;
        this->capacity  = instance.capacity;
    }

    return *this;
}

template<typename T>
inline void Vector<T>::push_back(const T& value) {
    if (this->size == this->capacity) {
        this->resize();
    }

    this->data[this->size] = value;
    this->size = this->size + 1;
}

template<typename T>
inline void Vector<T>::pop_back() {
    if (this->size == 0) {
        throw std::runtime_error("The vector is empty!");
    }

    this->size = this->size - 1;
}

template<typename T>
inline void Vector<T>::allocate(const Vector<T>& instance) {
    this->data = new T[instance.capacity];

    try {
        for (std::size_t i = 0; i < instance.size; ++i) {
            this->data[i] = instance.data[i];
        }
    } catch (std::exception& exception) {
        delete[] this->data;
        this->data = nullptr;
        throw exception;
    }
}

template<typename T>
inline void Vector<T>::deallocate() {
    delete[] this->data;
}

template<typename T>
inline void Vector<T>::resize() {
    T*  temporary = new T[this->capacity * 2];

    try {
        for (std::size_t i = 0; i < this->size; ++i) {
            temporary[i] = this->data[i];
        }
    } catch (std::exception& exception) {
        delete[] temporary;
        temporary = nullptr;
        throw exception;
    }

    delete[] this->data;
    this->data = temporary;

    this->capacity = this->capacity * 2;
}

#endif
