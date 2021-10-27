#include <algorithm>
#include <cstddef>

template <typename T>
class SharedPtr {
private:
    size_t* counter_ptr_ = nullptr;
    T* ptr_ = nullptr;

    void Deincrement() noexcept {
        if (counter_ptr_) {
            --(*counter_ptr_);
            if (*counter_ptr_ == 0) {
                delete counter_ptr_;
                delete ptr_;
                ptr_ = nullptr;
                counter_ptr_ = nullptr;
            }
        }
    }

public:
    SharedPtr() = default;

    SharedPtr(T* ptr) {
        if (ptr) {
            counter_ptr_ = new size_t(1);
            ptr_ = ptr;
        }
    }

    SharedPtr(const SharedPtr& other) noexcept {
        if (other.ptr_) {
            ptr_ = other.ptr_;
            counter_ptr_ = other.counter_ptr_;
            ++(*counter_ptr_);
        }
    }

    SharedPtr(SharedPtr&& other) noexcept : counter_ptr_(other.counter_ptr_), ptr_(other.ptr_) {
        other.ptr_ = nullptr;
        other.counter_ptr_ = nullptr;
    }

    SharedPtr& operator= (T* ptr) {
        Deincrement();
        ptr_ = ptr;
        counter_ptr_ = new size_t(1);
        return *this;
    }

    SharedPtr& operator= (const SharedPtr& rhs) noexcept {
        if (&rhs == this) {
            return *this;
        }
        Deincrement();
        ptr_ = rhs.ptr_;
        counter_ptr_ = rhs.counter_ptr_;
        if (counter_ptr_) {
            ++(*counter_ptr_);
        }
        return *this;
    }

    SharedPtr& operator= (SharedPtr&& rhs) noexcept {
        if (&rhs == this) {
            return *this;
        }
        Deincrement();
        ptr_ = rhs.ptr_;
        counter_ptr_ = rhs.counter_ptr_;
        rhs.ptr_ = nullptr;
        rhs.counter_ptr_ = nullptr;
        return *this;
    }

    T& operator*() noexcept {
        return *ptr_;
    }

    const T& operator*() const noexcept {
        return *ptr_;
    }

    T* operator->() const noexcept {
        return ptr_;
    }

    void reset(T* ptr) noexcept {
        *this = SharedPtr(ptr);
    }

    void swap(SharedPtr& rhs) noexcept {
        std::swap(rhs.ptr_, ptr_);
        std::swap(rhs.counter_ptr_, counter_ptr_);
    }

    T* get() const noexcept {
        return ptr_;
    }

    explicit operator bool() const noexcept {
        return ptr_;
    }

    ~SharedPtr() noexcept {
        Deincrement();
    }
};
