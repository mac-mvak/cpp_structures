#include <algorithm>
#include <cstddef>
#include <memory>
#include <tuple>
#include <utility>

template <typename T, typename D = std::default_delete<T>>
class UniquePtr {
private:
    std::tuple<T*, D> data;

public:
    UniquePtr(T* ptr = nullptr, const D& d = D()) noexcept {
        data = std::tie(ptr, d);
    }

    UniquePtr(UniquePtr&) = delete;

    UniquePtr& operator= (UniquePtr&) = delete;

    UniquePtr(UniquePtr&& other) noexcept {
        data = std::move(other.data);
        other.data = {nullptr, D()};
    }

    UniquePtr& operator= (UniquePtr&& other) noexcept {
        std::swap(data, other.data);
        return *this;
    }

    // UniquePtr(std::nullptr_t ptr) noexcept : ptr_(ptr) {}

    T& operator*() const noexcept {
        return *(std::get<0>(data));
    }

    T* operator->() const noexcept {
        return std::get<0>(data);
    }

    T* release() noexcept {
        T* ans = std::get<0>(data);
        std::get<0>(data) = nullptr;
        return ans;
    }

    void reset(T* ptr) noexcept {
        if (std::get<0>(data)) {
            std::get<1>(data)(std::get<0>(data));
        }
        std::get<0>(data) = ptr;
    }

    void swap(UniquePtr& other) {
        std::swap(data, other.data);
    }

    T* get() const noexcept {
        return std::get<0>(data);
    }

    explicit operator bool() const {
        return static_cast<bool>(std::get<0>(data));
    }

    D& get_deleter() noexcept {
        return std::get<1>(data);
    }

    const D& get_deleter() const noexcept {
        return std::get<1>(data);
    }

    ~UniquePtr() noexcept {
        if (std::get<0>(data)) {
            std::get<1>(data)(std::get<0>(data));
        }
    }
};

