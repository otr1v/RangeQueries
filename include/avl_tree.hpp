#include <iostream>
#include <memory>
#include <vector>
#include <functional>
#include <stack>
#include <algorithm>
#include <glog/logging.h>

#include "tree_exceptions.hpp"
namespace avl_tree
{
    template<typename T, typename Container = std::deque<T>>
    void printStack(std::stack<T, Container> stackToPrint) { 
        LOG(INFO) << "Stack from top to down: ";
        while (!stackToPrint.empty()) {
            LOG(INFO) << (*stackToPrint.top()).key_ << " ";
            stackToPrint.pop();
        }
        //LOG(INFO) << std::endl;
    }



    using avl_tree::NodeNullException;
    template <typename T>
    class AVLTree final
    {
    private:
        struct Node final
        {
            T key_;
            std::unique_ptr<Node> left_;
            std::unique_ptr<Node> right_;
            int height_ = 1;
            size_t desc_size = 1; // Size of the subtree rooted at this node
            explicit Node(const T &key) : key_(key) {}
        };

        std::unique_ptr<Node> root_;

        // Итеративная очистка дерева
        void Clear()
        {
            if (!root_)
                return;

            std::stack<std::unique_ptr<Node>> stack;
            stack.push(std::move(root_));

            while (!stack.empty())
            {
                std::unique_ptr<Node> node = std::move(stack.top());
                stack.pop();

                if (node->right_)
                {
                    stack.push(std::move(node->right_));
                }
                if (node->left_)
                {
                    stack.push(std::move(node->left_));
                }
            }
        }
        void UpdateSize(const std::unique_ptr<Node>& node)
        {
            if (!node)
                throw NodeNullException();
            node->desc_size = 1 + (node->left_ ? node->left_->desc_size : 0) + (node->right_ ? node->right_->desc_size : 0);
        }


        int GetHeight(const std::unique_ptr<Node> &node) const
        {
            return node ? node->height_ : 0;
        }

        T GetKey(const std::unique_ptr<Node> &node) const
        {
            if (!node)
                throw NodeNullException();
            return node->key_;
        }

        int GetBalance(const std::unique_ptr<Node> &node) const
        {
            return node ? GetHeight(node->left_) - GetHeight(node->right_) : 0;
        }

        bool VerifyBalance(const std::unique_ptr<Node> &node) const
        {
            if (!node)
                throw NodeNullException();
            return std::abs(GetBalance(node)) <= 1;
        }

        void UpdateHeight(const std::unique_ptr<Node> &node)
        {
            if (!node)
                throw NodeNullException();

            node->height_ = 1 + std::max(GetHeight(node->left_), GetHeight(node->right_));
        }

        std::unique_ptr<Node> RotateRight(std::unique_ptr<Node> y)
        {
            auto x = std::move(y->left_);
            auto T2 = std::move(x->right_);

            x->right_ = std::move(y);
            x->right_->left_ = std::move(T2);

            UpdateHeight(x->right_);
            UpdateSize(x->right_);
            UpdateHeight(x);
            UpdateSize(x);

            return x;
        }

        std::unique_ptr<Node> RotateLeft(std::unique_ptr<Node> x)
        {
            auto y = std::move(x->right_);
            auto T2 = std::move(y->left_);

            y->left_ = std::move(x);
            y->left_->right_ = std::move(T2);

            UpdateHeight(y->left_);
            UpdateSize(y->left_);
            UpdateHeight(y);
            UpdateSize(y);

            return y;
        }

        std::unique_ptr<Node> Balance(std::unique_ptr<Node> node)
        {
            if (!node)
                throw NodeNullException();

            UpdateHeight(node);
            UpdateSize(node);
            int balance = GetBalance(node);

            if (balance > 1)
            {
                if (GetBalance(node->left_) < 0)
                {
                    node->left_ = RotateLeft(std::move(node->left_));
                }
                return RotateRight(std::move(node));
            }

            if (balance < -1)
            {
                if (GetBalance(node->right_) > 0)
                {
                    node->right_ = RotateRight(std::move(node->right_));
                }
                return RotateLeft(std::move(node));
            }

            return node;
        }

        std::unique_ptr<Node> InsertNode(std::unique_ptr<Node> node, const T &key)
        {
            if (!node)
            {
                return std::make_unique<Node>(key);
            }

            if (key < node->key_)
            {
                node->left_ = InsertNode(std::move(node->left_), key);
            }
            else if (node->key_ < key)
            {
                node->right_ = InsertNode(std::move(node->right_), key);
            }
            else
            {
                return node; // Нельзя дублировать ключи
            }

            return Balance(std::move(node));
        }

    public:
        AVLTree() = default;
        
        ~AVLTree()
        {
            Clear();
        }
        void Insert(const T &key)
        {
            root_ = InsertNode(std::move(root_), key);
        }

        // Интерфейс для итератора
        class AVLIterator;

        // Абстрактная стратегия обхода
        class TraversalStrategy
        {
        public:
            virtual void Init(const Node *root, std::stack<const Node *> &stack) = 0;
            virtual void Next(std::stack<const Node *> &stack) = 0;
            virtual const Node *Current(const std::stack<const Node *> &stack) const = 0;
            virtual bool IsDone(const std::stack<const Node *> &stack) const = 0;
            virtual ~TraversalStrategy() = default;
        };

        class PreOrderStrategy final : public TraversalStrategy
        {
        public:
            void Init(const Node *root, std::stack<const Node *> &stack) override
            {
                if (root)
                {
                    stack.push(root);
                }
            }

            void Next(std::stack<const Node *> &stack) override
            {
                if (!stack.empty())
                {
                    const Node *current = stack.top();
                    stack.pop();
                    if (current->right_)
                        stack.push(current->right_.get());
                    if (current->left_)
                        stack.push(current->left_.get());
                }
            }

            const Node *Current(const std::stack<const Node *> &stack) const override
            {
                return stack.empty() ? nullptr : stack.top();
            }

            bool IsDone(const std::stack<const Node *> &stack) const override
            {
                return stack.empty();
            }
        };

        class InOrderStrategy final : public TraversalStrategy
        {
        public:
            void Init(const Node *root, std::stack<const Node *> &stack) override
            {
                PushLeftmost(root, stack);
            }

            void Next(std::stack<const Node *> &stack) override
            {
                if (!stack.empty())
                {
                    const Node *current = stack.top();
                    stack.pop();
                    PushLeftmost(current->right_.get(), stack);
                }
            }

            const Node *Current(const std::stack<const Node *> &stack) const override
            {
                return stack.empty() ? nullptr : stack.top();
            }

            bool IsDone(const std::stack<const Node *> &stack) const override
            {
                return stack.empty();
            }

        private:
            void PushLeftmost(const Node *node, std::stack<const Node *> &stack)
            {
                while (node)
                {
                    stack.push(node);
                    node = node->left_.get();
                }
            }
        };

        class PostOrderStrategy final : public TraversalStrategy
        {
        public:
            void Init(const Node *root, std::stack<const Node *> &stack) override
            {
                if (root)
                {
                    stack.push(root);
                    PushChildren(root, stack);
                }
            }

            void Next(std::stack<const Node *> &stack) override
            {
                if (!stack.empty())
                {
                    stack.pop();
                    if (!stack.empty())
                    {
                        PushChildren(stack.top(), stack);
                    }
                }
            }

            const Node *Current(const std::stack<const Node *> &stack) const override
            {
                return stack.empty() ? nullptr : stack.top();
            }

            bool IsDone(const std::stack<const Node *> &stack) const override
            {
                return stack.empty();
            }

        private:
            void PushChildren(const Node *node, std::stack<const Node *> &stack)
            {
                if (node->right_)
                {
                    stack.push(node->right_.get());
                    PushChildren(node->right_.get(), stack);
                }
                if (node->left_)
                {
                    stack.push(node->left_.get());
                    PushChildren(node->left_.get(), stack);
                }
            }
        };

        // Итератор
        class AVLIterator final
        {
        private:
            std::stack<const Node *> stack_;
            std::unique_ptr<TraversalStrategy> strategy_;
            bool isEnd_;

        public:
            AVLIterator(const Node *root, std::unique_ptr<TraversalStrategy> strategy)
                : strategy_(std::move(strategy)), isEnd_(root == nullptr)
            {
                if (!isEnd_)
                {
                    strategy_->Init(root, stack_);
                }
            }

            AVLIterator &operator++()
            {
                if (!isEnd_)
                {
                    strategy_->Next(stack_);
                    if (strategy_->IsDone(stack_))
                    {
                        isEnd_ = true;
                    }
                }
                return *this;
            }

            const T &operator*() const
            {
                return strategy_->Current(stack_)->key_;
            }

            bool operator!=(const AVLIterator &other) const
            {
                return isEnd_ != other.isEnd_ || stack_ != other.stack_;
            }

            bool operator==(const AVLIterator &other) const
            {
                return isEnd_ == other.isEnd_ && stack_ == other.stack_;
            }
        }; // class AVLIterator

        // Методы для создания итераторов
        AVLIterator BeginPreOrder() const
        {
            return AVLIterator(root_.get(), std::make_unique<PreOrderStrategy>());
        }

        AVLIterator EndPreOrder() const
        {
            return AVLIterator(nullptr, std::make_unique<PreOrderStrategy>());
        }

        AVLIterator BeginInOrder() const
        {
            return AVLIterator(root_.get(), std::make_unique<InOrderStrategy>());
        }

        AVLIterator EndInOrder() const
        {
            return AVLIterator(nullptr, std::make_unique<InOrderStrategy>());
        }

        AVLIterator BeginPostOrder() const
        {
            return AVLIterator(root_.get(), std::make_unique<PostOrderStrategy>());
        }

        AVLIterator EndPostOrder() const
        {
            return AVLIterator(nullptr, std::make_unique<PostOrderStrategy>());
        }

#ifdef NOT_VALID
        void RangeQuery(const std::unique_ptr<Node> &node, const T &min, const T &max, std::function<void(const T &)> callback) const
        {
            if (!node)
                throw NodeNullException();

            // FIXME exceptions needed
            const T &key = node->key_;
            //Compare comp;
            if (comp(min, key))
            {
                RangeQuery(node->left_, min, max, callback);
            }
            if (!comp(key, min) && !comp(max, key))
            {
                callback(key);
            }
            if (comp(key, max))
            {
                RangeQuery(node->right_, min, max, callback);
            }
        }

       
        #endif /* NOT_VALID */

        [[nodiscard]] size_t RangeQuery(const T& min, const T& max) const
        {
            if (!root_)
                throw EmptyTreeException();
            return RangeQueryHelper(root_.get(), min, max);
        }

        [[nodiscard]] size_t RangeQueryHelper(const Node* node, const T& min, const T& max) const
        {
            if (!node)
                return 0;
            LOG(INFO) << "Node key: " << node->key_;
            if (node->key_ < min)
                return RangeQueryHelper(node->right_.get(), min, max);

            if (node->key_ > max)
                return RangeQueryHelper(node->left_.get(), min, max);

                

            size_t left_count = node->left_ ? node->left_->desc_size : 0;
            size_t right_count = node->right_ ? node->right_->desc_size : 0;
            LOG(INFO) << ", Left count: " << left_count << ", Right count: " << right_count;
            return 1 + left_count + right_count;
        }
        AVLTree(AVLTree &&) = default;
        AVLTree &operator=(AVLTree &&) = default;

        AVLTree(const AVLTree &) = delete;
        AVLTree &operator=(const AVLTree &) = delete;
    }; // class AVLTree

} // namespace avl_tree
