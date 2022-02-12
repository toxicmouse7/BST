//
// Created by Алексей Гладков on 11.12.2021.
//

#ifndef TREEITER_BST_HPP
#define TREEITER_BST_HPP

#include <utility>
#include <map>
#include <stack>

template<class Key, class Value>
class BST
{
private:
    struct Node
    {
        Key key;
        Value value;

        Node* parent = nullptr;
        Node* left = nullptr;
        Node* right = nullptr;
    };

    Node* root = nullptr;
    Node* most_left = nullptr;
    Node* most_right = nullptr;

    size_t count = 0;

    Node** _find(const Key& key)
    {
        auto p = &root;
        while (*p && (*p)->key != key)
            p = (*p)->key < key ? &((*p)->right) : &((*p)->left);
        return p;
    }

    std::pair<Node**, Node*> _find_with_parent(const Key& key)
    {
        Node** p = &root;
        Node* parent = nullptr;

        while (*p && (*p)->key != key)
        {
            parent = *p;
            p = (*p)->key < key ? &((*p)->right) : &((*p)->left);
        }

        return {p, parent};
    }

    void update_edge_nodes(Node** p)
    {
        if (p == &root)
        {
            most_left = root;
            most_right = root;
        }
        else
        {
            most_left = most_left->left ? most_left->left : most_left;
            most_right = most_right->right ? most_right->right : most_right;
        }
    }

    void _visitor(std::function<void(Node* const)> visit) const
    {
        auto cur = root;
        std::stack<Node*> s;
        while (cur || !s.empty())
        {
            if (cur)
            {
                s.push(cur);
                cur = cur->left;
            }
            else
            {
                cur = s.top();
                visit(cur);
                s.pop();
                cur = cur->right;
            }
        }
    }

public:
    struct Iterator
    {
        using iterator_category = std::forward_iterator_tag;
        using difference_type = std::ptrdiff_t;
        using value_type = Value;
        using key_type = Key;
        using pointer = value_type*;
        using reference = value_type&;

        Iterator(Node** ptr) : m_ptr(ptr)
        {

        };

        reference operator*() const
        {
            return (*m_ptr)->value;
        }

        pointer operator->()
        {
            return &((*m_ptr)->value);
        }

        Iterator& operator++()
        {
            if (!(*m_ptr)->right)
            {
                m_ptr = &((*m_ptr)->parent);
                while ((*m_ptr)->left == nullptr)
                    m_ptr = &((*m_ptr)->parent);
            }
            else
            {
                m_ptr = &((*m_ptr)->right);
                while ((*m_ptr)->left)
                    m_ptr = &((*m_ptr)->left);
            }
            return *this;
        };

        Iterator operator++(int)
        {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        };

        friend bool operator==(const Iterator& a, const Iterator& b)
        {
            return a.m_ptr == b.m_ptr;
        };

        friend bool operator!=(const Iterator& a, const Iterator& b)
        {
            return a.m_ptr != b.m_ptr;
        };

    private:
        Node** m_ptr;
    };

    Iterator begin()
    {
        return Iterator(&most_left);
    }

    Iterator end()
    {
        return Iterator(&(most_right->right));
    }

    std::pair<Iterator, bool> insert(const Key& key, const Value& value)
    {
        this->insert(std::move(const_cast<Key&>(key)), std::move(const_cast<Key&>(value)));
    }

    std::pair<Iterator, bool> insert(Key&& key, Value&& value)
    {
        auto[p, parent] = this->_find_with_parent(key);
        if (*p)
        {
            (*p)->value = value;
            return {Iterator(p), false};
        }

        ++(this->count);
        (*p) = new Node{key, value, parent};

        this->update_edge_nodes(p);

        return {Iterator(p), true};
    }

    std::pair<Iterator, bool> insert(const Key& key, Value&& value)
    {
        this->insert(key, value);
    }

    std::pair<Iterator, bool> insert(Key&& key, const Value& value)
    {
        this->insert(key, value);
    }

    void erase(const Key& key)
    {
        auto[p, parent] = this->_find_with_parent(key);

        if (!(*p))
            return;

        if (!(*p)->left && !(*p)->right)
        {
            delete *p;
            *p = nullptr;
        }
        else if ((*p)->left && (*p)->right)
        {
            Node** min_node = &((*p)->right);
            while ((*min_node)->left)
                min_node = &((*min_node)->left);

            (*p)->key = (*min_node)->key;
            (*p)->value = (*min_node)->value;

            auto to_delete = *min_node;
            (*min_node)->right->parent = (*min_node)->parent;
            *min_node = (*min_node)->right;
            delete to_delete;
        }
        else
        {
            auto to_delete = *p;
            *p = (*p)->right ? (*p)->right : (*p)->left;
            delete to_delete;
        }

        --(this->count);
    }

    void clear()
    {
        std::stack<Node*> s;
        Node* cur = this->root;

        while (!s.empty() || cur)
        {
            if (cur)
            {
                s.push(cur->right);
                auto left_son = cur->left;
                delete cur;
                cur = left_son;
            }
            else
            {
                cur = s.top();
                s.pop();
            }
        }

        this->root = nullptr;
        this->count = 0;
    }

    Iterator next(const Key& key)
    {
        return this->next(key);
    }

    Iterator next(Key&& key)
    {
        Node** last_left_turn = nullptr;

        auto p = &root;
        while (*p && (*p)->key <= key)
            p = (*p)->key < key ? &((*p)->right) : (&((*p)->left), last_left_turn = p);

        if (!(*p)) return this->most_right;

        while ((*p)->left)
            p = &((*p)->left);

        return *p;
    }

    ~BST()
    {
        this->clear();
    }

    void agg(std::function<void(const Key&, const Value&)> f)
    {
        auto cur = root;
        std::stack<Node*> s;
        while (cur || !s.empty())
        {
            if (cur)
            {
                s.push(cur);
                cur = cur->left;
            }
            else
            {
                cur = s.top();
                f(cur->key, cur->value);
                s.pop();
                cur = cur->right;
            }
        }
    }

    int height()
    {
        int res = 0;
        std::stack<std::pair<Node*, int>> s = {root, 0};
        while (!s.empty())
        {
            auto[current, level] = s.top();

            if (current)
            {
                s.push({current->left, level + 1});
                s.push({current->right, level + 1});
                res = std::max(res, level);
            }
        }

        return res;
    }

    int weight()
    {
        int max = 0, min = 0;
        std::stack<std::pair<Node*, int>> s = {root, 0};
        while (!s.empty())
        {
            auto[current, level] = s.top();

            if (current)
            {
                s.push({current->left, level - 1});
                s.push({current->right, level + 1});
                max = std::max(max, level);
                min = std::min(min, level);
            }
        }

        return max - min;
    }

    [[nodiscard]] bool isRBT() const
    {
        using namespace std;
        bool result = true;
        _visitor([&result](Node* const n)
                 {
                     if (n->value == 1)
                     {
                         result = result && (!(n->left) || n->left->value == 0)
                                 && (!(n->right) || n->right->value == 0);
                     }
                 });

        if (!result) return false;

        int BH = -1;
        stack<pair<Node*, int>> s;
        s.push({root, 0});
        auto current = root->left;
        int bh = root->value == 0;
        while (!s.empty())
        {
            if (current)
            {
                s.push({current, bh});
                bh += current->value == 0;
                current = current->left;
            }
            else
            {
                if (BH == -1)
                    BH = bh;
                else if (BH != bh) return false;
                current = s.top().first->right;
                bh = s.top().second + (s.top().first->value == 0);
                s.pop();
            }
        }

        return true;
    }
};


#endif //TREEITER_BST_HPP