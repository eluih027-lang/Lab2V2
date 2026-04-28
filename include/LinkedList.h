#ifndef LR2_LINKED_LIST_H
#define LR2_LINKED_LIST_H

#include <stdexcept>
#include <utility>
template <class T>
class LinkedList {


public:
    class Iterator;
    class ConstIterator;

    LinkedList() : head_(nullptr), tail_(nullptr), length_(0) {}

    LinkedList(const T* items, int count) : LinkedList() {
        if (count < 0) {
            throw std::invalid_argument("count must be non-negative");
        }
        if (count > 0 && items == nullptr) {
            throw std::invalid_argument("items must not be null when count is positive");
        }
        for (int i = 0; i < count; ++i) {
            Append(items[i]);
        }
    }

    LinkedList(const LinkedList<T>& other) : LinkedList() {
        for (ConstIterator it = other.begin(); it != other.end(); ++it) {
            Append(*it);
        }
    }

    LinkedList<T>& operator=(const LinkedList<T>& other) {
        if (this == &other) {
            return *this;
        }
        LinkedList<T> copy(other);
        Swap(copy);
        return *this;
    }

    ~LinkedList() {
        Clear();
    }

    const T& GetFirst() const {
        if (length_ == 0) {
            throw std::out_of_range("linked list is empty");
        }
        return head_->value;
    }

    const T& GetLast() const {
        if (length_ == 0) {
            throw std::out_of_range("linked list is empty");
        }
        return tail_->value;
    }

    const T& Get(int index) const {
        return GetNode(index)->value;
    }

    LinkedList<T> GetSubList(int startIndex, int endIndex) const {
        CheckRange(startIndex, endIndex);
        LinkedList<T> result;
        Node* current = GetNode(startIndex);
        for (int i = startIndex; i <= endIndex; ++i) {
            result.Append(current->value);
            current = current->next;
        }
        return result;
    }

    int GetLength() const {
        return length_;
    }

    void Append(const T& item) {
        Node* node = new Node(item);
        if (length_ == 0) {
            head_ = tail_ = node;
        } else {
            tail_->next = node;
            node->prev = tail_;
            tail_ = node;
        }
        ++length_;
    }

    void Prepend(const T& item) {
        Node* node = new Node(item);
        if (length_ == 0) {
            head_ = tail_ = node;
        } else {
            node->next = head_;
            head_->prev = node;
            head_ = node;
        }
        ++length_;
    }

    void InsertAt(const T& item, int index) {
        if (index < 0 || index > length_) {
            throw std::out_of_range("linked list index out of range");
        }
        if (index == 0) {
            Prepend(item);
            return;
        }
        if (index == length_) {
            Append(item);
            return;
        }
        Node* nextNode = GetNode(index);
        Node* prevNode = nextNode->prev;
        Node* node = new Node(item);
        node->next = nextNode;
        node->prev = prevNode;
        prevNode->next = node;
        nextNode->prev = node;
        ++length_;
    }

    LinkedList<T> Concat(const LinkedList<T>& list) const {
        LinkedList<T> result(*this);
        for (ConstIterator it = list.begin(); it != list.end(); ++it) {
            result.Append(*it);
        }
        return result;
    }

    void RemoveAt(int index) {
        Node* node = GetNode(index);
        if (node->prev != nullptr) {
            node->prev->next = node->next;
        } else {
            head_ = node->next;
        }
        if (node->next != nullptr) {
            node->next->prev = node->prev;
        } else {
            tail_ = node->prev;
        }
        delete node;
        --length_;
    }

    class Iterator {
    public:
        explicit Iterator(Node* node) : current_(node) {}

        T& operator*() const { return current_->value; }
        Iterator& operator++() {
            current_ = current_->next;
            return *this;
        }
        bool operator!=(const Iterator& other) const { return current_ != other.current_; }
        bool operator==(const Iterator& other) const { return current_ == other.current_; }

    private:
        Node* current_;
    };

    class ConstIterator {
    public:
        explicit ConstIterator(const Node* node) : current_(node) {}

        const T& operator*() const { return current_->value; }
        ConstIterator& operator++() {
            current_ = current_->next;
            return *this;
        }
        bool operator!=(const ConstIterator& other) const { return current_ != other.current_; }
        bool operator==(const ConstIterator& other) const { return current_ == other.current_; }

    private:
        const Node* current_;
    };
    private:
        struct Node;
    Iterator begin() { return Iterator(head_); }
    Iterator end() { return Iterator(nullptr); }
    ConstIterator begin() const { return ConstIterator(head_); }
    ConstIterator end() const { return ConstIterator(nullptr); }
    ConstIterator cbegin() const { return ConstIterator(head_); }
    ConstIterator cend() const { return ConstIterator(nullptr); }

private:
    struct Node {
        T value;
        Node* next;
        Node* prev;

        explicit Node(const T& value) : value(value), next(nullptr), prev(nullptr) {}
    };

    Node* head_;
    Node* tail_;
    int length_;

    void Clear() {
        Node* current = head_;
        while (current != nullptr) {
            Node* next = current->next;
            delete current;
            current = next;
        }
        head_ = nullptr;
        tail_ = nullptr;
        length_ = 0;
    }

    Node* GetNode(int index) const {
        if (index < 0 || index >= length_) {
            throw std::out_of_range("linked list index out of range");
        }

        Node* current = nullptr;
        if (index < length_ / 2) {
            current = head_;
            for (int i = 0; i < index; ++i) {
                current = current->next;
            }
        } else {
            current = tail_;
            for (int i = length_ - 1; i > index; --i) {
                current = current->prev;
            }
        }
        return current;
    }

    void CheckRange(int startIndex, int endIndex) const {
        if (startIndex < 0 || endIndex < 0 || startIndex > endIndex || endIndex >= length_) {
            throw std::out_of_range("linked list range out of range");
        }
    }

    void Swap(LinkedList<T>& other) noexcept {
        std::swap(head_, other.head_);
        std::swap(tail_, other.tail_);
        std::swap(length_, other.length_);
    }
};

#endif
