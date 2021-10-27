#include <algorithm>
#include <memory>


template <typename T>
class Vector {
private:
    T* data = nullptr;
    size_t sz = 0;
    size_t cp = 0;

    T* Allocate(size_t n) {
        return reinterpret_cast<T*>(operator new (n * sizeof(T)));
    }

    void Dellocate(T* ptr) {
        operator delete (ptr);
    }

public:
    Vector() = default;
    Vector(size_t size) : data(nullptr), sz(size), cp(size) {
        if (size == 0) {
            return;
        }
        data = Allocate(size);
        try {
            std::uninitialized_value_construct_n(data, size);
        } catch (...) {
            Dellocate(data);
            throw;
        }
    }

    Vector(const Vector& other) : sz(other.sz), cp(other.sz) {
        if (sz == 0) {
            return;
        }
        data = Allocate(other.sz);
        try {
            std::uninitialized_copy_n(other.data, other.sz, data);
        } catch(...) {
            Dellocate(data);
            throw;
        }
    }

    Vector& operator=(Vector other) {
        swap(other);
        return *this;
    }



    void reserve(size_t size) {
        if (cp < size) {
            T* new_data = Allocate(size);
            try {
                std::uninitialized_copy_n(data, sz, new_data);
                std::destroy_n(data, sz);
                Dellocate(data);
                cp = size;
                data = new_data;
            } catch (...) {
                Dellocate(new_data);
                throw;
            }
        }
    }

    void resize(size_t size) {
        reserve(size);
        if (sz < size) {
            std::uninitialized_value_construct(data + sz, data + size);
        }  else if (sz > size) {
            std::destroy(data + size, data + sz);
        }
        sz = size;
    }
    void swap(Vector& other) {
        std::swap(data, other.data);
        std::swap(cp, other.cp);
        std::swap(sz, other.sz);
    }

    void clear() {
        std::destroy(data, data + sz);
        sz = 0;
    }

    void push_back(const T& elem) {
        if (sz == cp) {
            reserve((cp == 0) ? 1 : cp * 2);
        }
        new (data + sz) T(elem);
        ++sz;
    }

    void push_back(T&& elem) {
        if (sz == cp) {
            reserve((cp == 0) ? 1 : cp * 2);
        }
        new (data + sz) T(std::move(elem));
        ++sz;
    }

    void pop_back() {
        (data + --sz)->~T();
    }

    size_t size() const {
        return sz;
    }

    size_t capacity() const {
        return cp;
    }

    T& operator[] (size_t i) {
        return data[i];
    }

    const T& operator[] (size_t i) const {
        return data[i];
    }

    T* begin() {
        return data;
    }

    const T* begin() const {
        return data;
    }

    T* end() {
        return data + sz;
    }

    const T* end() const {
        return data + sz;
    }

    ~Vector() {
        std::destroy_n(data, sz);
        operator delete(data);
    }
};
