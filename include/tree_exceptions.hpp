#pragma once

#include <stdexcept>


namespace avl_tree
{

    class AVLException : public std::exception
    {
    public:
        explicit AVLException(const char *msg) : message_(msg) {}
        const char *what() const noexcept override { return message_; }

    private:
        const char *message_;
    };

    class NodeNullException : public AVLException
    {
    public:
        NodeNullException()
            : AVLException("\n Internal error: encountered null node") {}
    };

   

} // namespace avl_tree