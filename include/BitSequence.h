#ifndef LR2_BIT_SEQUENCE_H
#define LR2_BIT_SEQUENCE_H

#include <stdexcept>
#include "Bit.h"
#include "MutableArraySequence.h"

class BitSequence : public MutableArraySequence<Bit> {
public:
    using MutableArraySequence<Bit>::MutableArraySequence;

    BitSequence BitwiseAnd(const BitSequence& other) const {
        CheckSameLength(other);
        BitSequence result;
        for (int i = 0; i < this->GetLength(); ++i) {
            result.Append(this->Get(i) & other.Get(i));
        }
        return result;
    }

    BitSequence BitwiseOr(const BitSequence& other) const {
        CheckSameLength(other);
        BitSequence result;
        for (int i = 0; i < this->GetLength(); ++i) {
            result.Append(this->Get(i) | other.Get(i));
        }
        return result;
    }

    BitSequence BitwiseXor(const BitSequence& other) const {
        CheckSameLength(other);
        BitSequence result;
        for (int i = 0; i < this->GetLength(); ++i) {
            result.Append(this->Get(i) ^ other.Get(i));
        }
        return result;
    }

    BitSequence BitwiseNot() const {
        BitSequence result;
        for (int i = 0; i < this->GetLength(); ++i) {
            result.Append(~this->Get(i));
        }
        return result;
    }

private:
    void CheckSameLength(const BitSequence& other) const {
        if (this->GetLength() != other.GetLength()) {
            throw std::invalid_argument("bit sequences must have equal length");
        }
    }
};

#endif
