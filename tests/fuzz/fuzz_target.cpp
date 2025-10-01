#include "avl_tree.hpp"
#include <cstdint>
#include <cstddef>
#include <vector>

struct DataProvider {
    const uint8_t *data;
    size_t size;
    size_t offset = 0;

    DataProvider(const uint8_t *d, size_t s) : data(d), size(s) {}

    bool CanRead(size_t count) {
        return offset + count <= size;
    }

    template<typename T>
    bool Read(T& value) {
        if (!CanRead(sizeof(T))) {
            return false;
        }
        memcpy(&value, data + offset, sizeof(T));
        offset += sizeof(T);
        return true;
    }
};

extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
    DataProvider provider(Data, Size);
    avl_tree::AVLTree<int> tree;

    while (true) {
        char command;
        if (!provider.Read(command)) break;

        if (command % 2 == 0) { 
            int value;
            if (!provider.Read(value)) break;
            tree.Insert(value);
        } else {
            int a, b;
            if (!provider.Read(a) || !provider.Read(b)) break;
            tree.RangeQuery(std::min(a, b), std::max(a, b));
        }
    }
    return 0;
}