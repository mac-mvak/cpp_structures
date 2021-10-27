#include <cstddef>
#include <optional>
#include <memory>


template<typename T>
class List {
private:
    struct NodeWithoutValue {
        NodeWithoutValue* next;
        NodeWithoutValue* prev;
        virtual ~NodeWithoutValue() = default;
    };
    struct Node : public NodeWithoutValue {
        Node(const T& c) : container(c) {
        }
        T container;
    };

    NodeWithoutValue* dummy;
    size_t size_ = 0;

public:
    class CIterator {
    private:
        NodeWithoutValue* node_;

    public:
        CIterator(NodeWithoutValue* node) {
            node_ = node;
        }

        CIterator& operator++() {
            node_ = node_->next;
            return *this;
        }

        CIterator operator++(int) {
            CIterator ans(node_);
            node_ = node_->next;
            return ans;
        }

        CIterator& operator--() {
            node_ = node_->prev;
            return *this;
        }

        CIterator operator--(int) {
            CIterator ans(node_);
            node_ = node_->prev;
            return ans;
        }

        const T& operator*() {
            return static_cast<Node*>(node_)->container;
        }

        bool operator==(const CIterator& rhs) const = default;

        bool operator!=(const CIterator& rhs) const = default;
    };

    class Iterator {
    private:
        NodeWithoutValue* node_;

    public:
        Iterator(NodeWithoutValue* node) {
            node_ = node;
        }

        Iterator& operator++() {
            node_ = node_->next;
            return *this;
        }

        Iterator operator++(int) {
            Iterator ans(node_);
            node_ = node_->next;
            return ans;
        }

        Iterator& operator--() {
            node_ = node_->prev;
            return *this;
        }

        Iterator operator--(int) {
            Iterator ans(node_);
            node_ = node_->prev;
            return ans;
        }

        T& operator*() {
            return static_cast<Node*>(node_)->container;
        }

        bool operator==(const Iterator& rhs) const = default;

        bool operator!=(const Iterator& rhs) const = default;
    };

    List() {
        dummy = new NodeWithoutValue;
        dummy->next = dummy;
        dummy->prev = dummy;
    }

    List(const List& lhs) {
        dummy = new Node;
        dummy->next = dummy;
        dummy->prev = dummy;
        for (const auto& n : lhs) {
            this->push_back(n);
        }
    }

    void push_back(const T& c) {
        NodeWithoutValue* pos = new Node{c};
        NodeWithoutValue* last = dummy->prev;
        pos->prev = last;
        pos->next = dummy;
        last->next = pos;
        dummy->prev = pos;
        ++size_;
    }

    void push_front(const T& c) {
        NodeWithoutValue* pos = new Node{c};
        NodeWithoutValue* first = dummy->next;
        pos->prev = dummy;
        pos->next = first;
        first->prev = pos;
        dummy->next = pos;
        ++size_;
    }

    void pop_back() {
        NodeWithoutValue* last = dummy->prev;
        NodeWithoutValue* new_last = last->prev;
        dummy->prev = new_last;
        new_last->next = dummy;
        delete last;
        --size_;
    }

    void pop_front() {
        NodeWithoutValue* first = dummy->next;
        NodeWithoutValue* new_first = first->next;
        dummy->next = new_first;
        new_first->prev = dummy;
        delete first;
        --size_;
    }

    size_t size() const {
        return size_;
    }

    Iterator begin() {
        return Iterator(dummy->next);
    }

    Iterator end() {
        return Iterator(dummy);
    }

    CIterator begin() const {
        return CIterator(dummy->next);
    }

    CIterator end() const {
        return CIterator(dummy);
    }

    ~List() {
        NodeWithoutValue* pos = dummy->next;
        while (pos != dummy) {
            NodeWithoutValue* new_pos = pos->next;
            delete pos;
            pos = new_pos;
        }
        delete dummy;
    }
};

