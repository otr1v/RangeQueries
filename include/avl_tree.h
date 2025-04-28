#include <iostream>
#include <memory>
#include <vector>
#include <functional>
#include <stack>
#include <algorithm>

namespace avl_tree
{

    template <typename T, typename Compare = std::less<T>>
    class AVLTree
    {
    private:
        struct Node
        {
            T key_;
            std::unique_ptr<Node> left_;
            std::unique_ptr<Node> right_;
            int height_ = 1;

            Node(const T &key) : key_(key) {}
        };

        std::unique_ptr<Node> root_;

        int GetHeight(const std::unique_ptr<Node> &node) const
        {
            return node ? node->height_ : 0;
        }

        int GetBalance(const std::unique_ptr<Node> &node) const
        {
            return node ? GetHeight(node->left_) - GetHeight(node->right_) : 0;
        }

        void UpdateHeight(const std::unique_ptr<Node> &node)
        {
            if (node)
            {
                node->height_ = 1 + std::max(GetHeight(node->left_), GetHeight(node->right_));
            }
        }

        std::unique_ptr<Node> RotateRight(std::unique_ptr<Node> y)
        {
            auto x = std::move(y->left_);
            auto T2 = std::move(x->right_);

            x->right_ = std::move(y);
            x->right_->left_ = std::move(T2);

            UpdateHeight(x->right_);
            UpdateHeight(x);

            return x;
        }

        std::unique_ptr<Node> RotateLeft(std::unique_ptr<Node> x)
        {
            auto y = std::move(x->right_);
            auto T2 = std::move(y->left_);

            y->left_ = std::move(x);
            y->left_->right_ = std::move(T2);

            UpdateHeight(y->left_);
            UpdateHeight(y);

            return y;
        }

        std::unique_ptr<Node> Balance(std::unique_ptr<Node> node)
        {
            if (!node)
                return nullptr;

            UpdateHeight(node);
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

            Compare comp;
            if (comp(key, node->key_))
            {
                node->left_ = InsertNode(std::move(node->left_), key);
            }
            else if (comp(node->key_, key))
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

       
        class PreOrderStrategy : public TraversalStrategy
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

        
        class InOrderStrategy : public TraversalStrategy
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

        
        class PostOrderStrategy : public TraversalStrategy
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
        class AVLIterator
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

            AVLIterator& operator++()
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

            const T& operator*() const
            {
                return strategy_->Current(stack_)->key_;
            }

            bool operator!=(const AVLIterator &other) const
            {
                return isEnd_ != other.isEnd_ || stack_ != other.stack_;
            }
        };

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

        
        void RangeQuery(const std::unique_ptr<Node> &node, const T &min, const T &max, std::function<void(const T &)> callback) const
        {
            if (!node)
                return;
            const T &key = node->key_;
            Compare comp;
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

        void RangeQuery(const T &min, const T &max, std::function<void(const T &)> callback) const
        {
            RangeQuery(root_, min, max, callback);
        }

        
        AVLTree(AVLTree &&) = default;
        AVLTree &operator=(AVLTree &&) = default;

        
        AVLTree(const AVLTree &) = delete;
        AVLTree &operator=(const AVLTree &) = delete;
    }; // class AVLTree

} // namespace avl_tree
