#ifndef LR2_BIT_H
#define LR2_BIT_H

#include <ostream>

class Bit {
public:
    Bit() : value_(false) {}
    Bit(bool value) : value_(value) {}
    Bit(int value) : value_(value != 0) {}

    bool Value() const { return value_; }

    friend Bit operator&(const Bit& lhs, const Bit& rhs) { return Bit(lhs.value_ && rhs.value_); }
    friend Bit operator|(const Bit& lhs, const Bit& rhs) { return Bit(lhs.value_ || rhs.value_); }
    friend Bit operator^(const Bit& lhs, const Bit& rhs) { return Bit(lhs.value_ != rhs.value_); }
    friend Bit operator~(const Bit& bit) { return Bit(!bit.value_); }
    friend bool operator==(const Bit& lhs, const Bit& rhs) { return lhs.value_ == rhs.value_; }
    friend bool operator!=(const Bit& lhs, const Bit& rhs) { return !(lhs == rhs); }
    friend std::ostream& operator<<(std::ostream& os, const Bit& bit) { os << (bit.value_ ? '1' : '0'); return os; }

private:
    bool value_;
};

#endif
