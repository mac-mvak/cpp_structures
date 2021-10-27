#include <utility>

// Это исключение нужно генерировать в функции value
// при обращении к пустому объекту Optional
struct BadOptionalAccess {
};

template <typename T>
class Optional {
private:
    // alignas нужен для правильного выравнивания блока памяти
    alignas(T) unsigned char data[sizeof(T)];
    bool defined = false;

public:
    Optional() = default;

    Optional(const T& elem) {
        new (data) T(elem);
        defined = true;
    }

    Optional(T && elem) {
        new (data) T(std::move(elem));
        defined = true;
    }

    Optional(const Optional& other) {
        if (other.defined) {
            new (data) T(other.value());
            defined = true;
        }
    }

    Optional& operator=(const Optional& other) {
        if (&other == this) {
            return *this;
        }
        if (defined && other.defined) {
            *reinterpret_cast<T*>(data) = other.value();
        } else if (defined) {
            reset();
        } else if (other.defined) {
            new (data) T(other.value());
        }
        defined = other.defined;
        return *this;
    }
    Optional& operator=(const T& elem) {
        if (!defined) {
            new (data) T(elem);
            defined = true;
        } else {
            *reinterpret_cast<T*>(data) = elem;
        }
        return *this;
    }

    Optional& operator=(T&& elem) {
        if (!defined) {
            new (data) T(std::move(elem));
            defined = true;
        } else {
            *reinterpret_cast<T*>(data) = std::move(elem);
        }
        return *this;
    }

    bool has_value() const {
        return defined;
    }

    T& operator*() {
        return *reinterpret_cast<T*>(data);
    }
    const T& operator*() const {
        return *reinterpret_cast<const T*>(data);
    }

    T* operator->() {
        return reinterpret_cast<T*>(data);
    }
    const T* operator->() const {
        return reinterpret_cast<const T*>(data);
    }

    T& value() {
        if (!defined) {
            throw BadOptionalAccess();
        }
        return *reinterpret_cast<T*>(data);
    }
    const T& value() const {
        if (!defined) {
            throw BadOptionalAccess();
        }
        return *reinterpret_cast<const T*>(data);
    }

    void reset() {
        if (defined) {
            reinterpret_cast<T*>(data)->~T();
            defined = false;
        }
    }

    ~Optional() {
        reset();
    }
};

