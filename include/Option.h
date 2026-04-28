#ifndef LR2_OPTION_H
#define LR2_OPTION_H

#include <stdexcept>

template <class T>
class Option {
public:
    Option() : hasValue_(false), value_() {}
    explicit Option(const T& value) : hasValue_(true), value_(value) {}

    static Option<T> None() { return Option<T>(); }

    bool HasValue() const { return hasValue_; }

    const T& Value() const {
        if (!hasValue_) {
            throw std::logic_error("option has no value");
        }
        return value_;
    }

    T ValueOr(const T& fallback) const {
        return hasValue_ ? value_ : fallback;
    }

private:
    bool hasValue_;
    T value_;
};

#endif
