#pragma once
#include <cstdlib>
#include <ctime>
#include <limits>

static constexpr int MAX_LEVEL = 16;
static constexpr float SKIPLIST_P = 0.5f;

template<typename T, typename Compare>
class SkipList {
private:
    struct Node {
        T value;
        Node* forward[MAX_LEVEL];
        int node_level;

        Node(const T& val, int lvl) : value(val), node_level(lvl) {
            for (int i = 0; i < MAX_LEVEL; ++i)
                forward[i] = nullptr;
        }
    };

    Node* header;
    int level;
    Compare comp;

    int random_level() {
        int lvl = 1;
        while ((std::rand() / double(RAND_MAX)) < SKIPLIST_P && lvl < MAX_LEVEL)
            lvl++;
        return lvl;
    }

public:
    SkipList() : level(1) {
        header = new Node(T{}, MAX_LEVEL);
        std::srand(static_cast<unsigned>(std::time(nullptr)));
    }

    ~SkipList() {
        Node* node = header->forward[0];
        while (node) {
            Node* next = node->forward[0];
            delete node;
            node = next;
        }
        delete header;
    }

    T* search(const T& value) {
        Node* current = header;
        for (int i = level - 1; i >= 0; --i) {
            while (current->forward[i] &&
                   comp(current->forward[i]->value, value)) {
                current = current->forward[i];
            }
        }
        current = current->forward[0];
        if (current && !comp(value, current->value) && !comp(current->value, value))
            return &current->value;

        return nullptr;
    }

    void insert(const T& value) {
        Node* update[MAX_LEVEL];
        Node* current = header;

        for (int i = level - 1; i >= 0; --i) {
            while (current->forward[i] &&
                   comp(current->forward[i]->value, value)) {
                current = current->forward[i];
            }
            update[i] = current;
        }

        int new_level = random_level();

        if (new_level > level) {
            for (int i = level; i < new_level; ++i)
                update[i] = header;
            level = new_level;
        }

        Node* new_node = new Node(value, new_level);

        for (int i = 0; i < new_level; ++i) {
            new_node->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = new_node;
        }
    }

    void remove(const T& value) {
        Node* update[MAX_LEVEL];
        Node* current = header;

        for (int i = level - 1; i >= 0; --i) {
            while (current->forward[i] &&
                   comp(current->forward[i]->value, value)) {
                current = current->forward[i];
            }
            update[i] = current;
        }

        current = current->forward[0];

        if (current) {
            for (int i = 0; i < level; ++i) {
                if (update[i]->forward[i] != current)
                    break;
                update[i]->forward[i] = current->forward[i];
            }
            delete current;

            while (level > 1 && header->forward[level - 1] == nullptr)
                level--;
        }
    }

    T* best() {
        return header->forward[0] ? &header->forward[0]->value : nullptr;
    }
};
