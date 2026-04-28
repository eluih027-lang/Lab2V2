#ifndef LR2_SEQUENCE_H
#define LR2_SEQUENCE_H

#include <functional>
#include <stdexcept>
#include "Option.h"

template <class T>
class Sequence;

template <class T, template <typename> class Derived>
class SequenceFactoryMixin {
protected:
    template <class Other>
    Sequence<Other>* CreateNew() const {
        return new Derived<Other>();
    }

    Derived<T>* CloneSelf(const Derived<T>& self) const {
        return new Derived<T>(self);
    }
};

template <class T>
class Sequence {
public:
    virtual ~Sequence() = default;

    virtual const T& GetFirst() const = 0;
    virtual const T& GetLast() const = 0;
    virtual const T& Get(int index) const = 0;
    virtual Sequence<T>* GetSubsequence(int startIndex, int endIndex) const = 0;
    virtual int GetLength() const = 0;

    virtual Sequence<T>* Append(const T& item) = 0;
    virtual Sequence<T>* Prepend(const T& item) = 0;
    virtual Sequence<T>* InsertAt(const T& item, int index) = 0;
    virtual Sequence<T>* Concat(const Sequence<T>& list) const = 0;
    virtual Sequence<T>* Clone() const = 0;

    template <class TResult>
    Sequence<TResult>* Map(const std::function<TResult(const T&)>& mapper) const {
        Sequence<TResult>* result = CreateMappedEmpty<TResult>();
        ForEach([&](const T& value) {
            result->Append(mapper(value));
        });
        return result;
    }

    Sequence<T>* Where(const std::function<bool(const T&)>& predicate) const {
        Sequence<T>* result = CreateEmptySameKind();
        ForEach([&](const T& value) {
            if (predicate(value)) {
                result->Append(value);
            }
        });
        return result;
    }

    template <class TAcc>
    TAcc Reduce(const std::function<TAcc(TAcc, const T&)>& reducer, TAcc initial) const {
        TAcc acc = initial;
        ForEach([&](const T& value) {
            acc = reducer(acc, value);
        });
        return acc;
    }

    Option<T> TryGetFirst(const std::function<bool(const T&)>& predicate) const {
        Option<T> result;
        bool found = false;
        ForEach([&](const T& value) {
            if (!found && predicate(value)) {
                result = Option<T>(value);
                found = true;
            }
        });
        return result;
    }

protected:
    virtual Sequence<T>* CreateEmptySameKind() const = 0;

public:
    virtual void ForEach(const std::function<void(const T&)>& action) const = 0;

private:
    template <class TResult>
    Sequence<TResult>* CreateMappedEmpty() const;
};

#endif
