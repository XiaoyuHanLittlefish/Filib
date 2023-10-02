#pragma once

#include <iostream>
#include <fstream>

namespace frbtree {

namespace {
enum class NodeColor {
    BLACK,
    RED,
};

enum class NodeType {
    LEFT,
    RIGHT,
    ROOT,
};

template <typename T> class RbtreeNode {
public:
    explicit RbtreeNode(const T &data_)
        : data(data_)
        , left_child_ptr(nullptr)
        , right_child_ptr(nullptr)
        , father_ptr(nullptr)
        , color(NodeColor::RED)
        , type(NodeType::ROOT) { }

    void set_data(const T &data_) {
        this->data = data_;
    }
    T get_data() {
        return this->data;
    }

    void set_child(RbtreeNode<T> *ptr, NodeType child_type) {
        if (NodeType::LEFT == child_type) {
            this->set_left_child(ptr);
            return;
        }
        this->set_right_child(ptr);
    }
    RbtreeNode<T> *get_child(NodeType child_type) {
        if (NodeType::LEFT == child_type) {
            return this->get_left_child();
        }
        return this->get_right_child();
    }

    void set_father(RbtreeNode<T> *ptr) {
        this->father_ptr = ptr;
    }
    RbtreeNode<T> *get_father() {
        return this->father_ptr;
    }

    NodeColor get_color() {
        return this->color;
    }
    void set_color(NodeColor color) {
        this->color = color;
    }
    void change_color() {
        switch (this->color) {
        case NodeColor::BLACK:
            this->color = NodeColor::RED;
            break;
        case NodeColor::RED:
            this->color = NodeColor::BLACK;
            break;
        default:
            // TODO: throw something or do any other thing
            this->color = NodeColor::RED;
        }
    }

    void set_type(NodeType type_) {
        this->type = type_;
    }
    NodeType get_type() {
        return this->type;
    }

private:
    void set_left_child(RbtreeNode<T> *ptr) {
        this->left_child_ptr = ptr;
        if (nullptr != ptr) {
            ptr->type = NodeType::LEFT;
        }
    }
    RbtreeNode<T> *get_left_child() {
        return this->left_child_ptr;
    }
    void set_right_child(RbtreeNode<T> *ptr) {
        this->right_child_ptr = ptr;
        if (nullptr != ptr) {
            ptr->type = NodeType::RIGHT;
        }
    }
    RbtreeNode<T> *get_right_child() {
        return this->right_child_ptr;
    }

private:
    T data;

    RbtreeNode<T> *left_child_ptr;
    RbtreeNode<T> *right_child_ptr;
    RbtreeNode<T> *father_ptr;

    NodeColor color;
    NodeType  type;
};
} // namespace

template <typename T> class Rbtree {
public:
    using Node = RbtreeNode<T>;

public:
    Rbtree()
        : root_node(nullptr) { }

    bool find(const T &data) {
        Node *now_ptr = root_node;
        while (nullptr != now_ptr) {
            if (now_ptr->get_data() == data) {
                return true;
            } else if (data < now_ptr->get_data()) {
                now_ptr = now_ptr->get_child(NodeType::LEFT);
            } else {
                now_ptr = now_ptr->get_child(NodeType::RIGHT);
            }
        }
        return false;
    }

    void push(const T &data) {
        if (nullptr != this->root_node) {
            Node *node_ptr = new RbtreeNode<T>(data);
            if (insert_node(node_ptr)) {
                insert_update_pushup(node_ptr);
            }
        } else {
            this->root_node = new RbtreeNode<T>(data);
            this->root_node->set_type(NodeType::ROOT);
            change_node_color(root_node);
        }
    }

    bool erase(const T &data) {
        Node *erase_ptr = find_node(data);
        if (nullptr == erase_ptr) {
            return false;
        }

        if (2 == child_count(erase_ptr)) {
            Node *next_ptr = find_next_node(erase_ptr);
            if (nullptr == next_ptr) {
                return false;
            }
            erase_ptr->set_data(next_ptr->get_data());
            erase_ptr = next_ptr;
        }

        remove_node(erase_ptr);
        return true;
    }

private:
    bool insert_node(Node *node_ptr) {
        Node *father_node_ptr = this->root_node;
        Node *next_pos        = nullptr;

        while (true) {
            if (node_ptr->get_data() == father_node_ptr->get_data()) {
                return false;
            } else if (node_ptr->get_data() < father_node_ptr->get_data()) {
                next_pos = father_node_ptr->get_child(NodeType::LEFT);
                if (nullptr == next_pos) {
                    father_node_ptr->set_child(node_ptr, NodeType::LEFT);
                    break;
                }
            } else {
                next_pos = father_node_ptr->get_child(NodeType::RIGHT);
                if (nullptr == next_pos) {
                    father_node_ptr->set_child(node_ptr, NodeType::RIGHT);
                    break;
                }
            }

            father_node_ptr = next_pos;
        }
        node_ptr->set_father(father_node_ptr);
        return true;
    }

    NodeColor get_node_color(Node *node_ptr) {
        if (nullptr == node_ptr) {
            return NodeColor::BLACK;
        }
        return node_ptr->get_color();
    }

    void change_node_color(Node *node_ptr) {
        if (nullptr != node_ptr) {
            node_ptr->change_color();
        }
    }

    void exchange_two_nodes_color(Node *lhs_ptr, Node *rhs_ptr) {
        if (get_node_color(lhs_ptr) != get_node_color(rhs_ptr)) {
            change_node_color(lhs_ptr);
            change_node_color(rhs_ptr);
        }
    }

    void rotate_left(Node *father_node_ptr) {
        Node *right_child_ptr      = father_node_ptr->get_child(NodeType::RIGHT);
        Node *grandfather_node_ptr = father_node_ptr->get_father();

        if (nullptr != grandfather_node_ptr) {
            NodeType type = father_node_ptr->get_type();
            grandfather_node_ptr->set_child(right_child_ptr, type);
            right_child_ptr->set_father(grandfather_node_ptr);
        } else {
            right_child_ptr->set_father(nullptr);
            this->root_node = right_child_ptr;
            right_child_ptr->set_type(NodeType::ROOT);
        }

        Node *left_grandchild_ptr = right_child_ptr->get_child(NodeType::LEFT);
        father_node_ptr->set_child(left_grandchild_ptr, NodeType::RIGHT);
        if (nullptr != left_grandchild_ptr) {
            left_grandchild_ptr->set_father(father_node_ptr);
        }

        right_child_ptr->set_child(father_node_ptr, NodeType::LEFT);
        father_node_ptr->set_father(right_child_ptr);
    }

    void rotate_right(Node *father_node_ptr) {
        Node *left_child_ptr       = father_node_ptr->get_child(NodeType::LEFT);
        Node *grandfather_node_ptr = father_node_ptr->get_father();

        if (nullptr != grandfather_node_ptr) {
            NodeType type = father_node_ptr->get_type();
            grandfather_node_ptr->set_child(left_child_ptr, type);
            left_child_ptr->set_father(grandfather_node_ptr);
        } else {
            left_child_ptr->set_father(nullptr);
            this->root_node = left_child_ptr;
            left_child_ptr->set_type(NodeType::ROOT);
        }

        Node *right_grandchild_ptr = left_child_ptr->get_child(NodeType::RIGHT);
        father_node_ptr->set_child(right_grandchild_ptr, NodeType::LEFT);
        if (nullptr != right_grandchild_ptr) {
            right_grandchild_ptr->set_father(father_node_ptr);
        }

        left_child_ptr->set_child(father_node_ptr, NodeType::RIGHT);
        father_node_ptr->set_father(left_child_ptr);
    }

    void insert_update_pushup(Node *node_ptr) {
        Node *father_node_ptr = node_ptr->get_father();
        // 父亲结点为黑色，无影响
        if (NodeColor::BLACK == get_node_color(father_node_ptr)) {
            return;
        }

        // 父亲结点为红色
        Node *grandfather_node_ptr = father_node_ptr->get_father();
        Node *uncle_node_ptr       = nullptr;
        if (NodeType::LEFT == father_node_ptr->get_type()) {
            uncle_node_ptr = grandfather_node_ptr->get_child(NodeType::RIGHT);
        } else {
            uncle_node_ptr = grandfather_node_ptr->get_child(NodeType::LEFT);
        }

        if (NodeColor::RED == get_node_color(uncle_node_ptr)) {
            // 叔叔结点为红色
            change_node_color(father_node_ptr);
            change_node_color(uncle_node_ptr);
            change_node_color(grandfather_node_ptr);
            if (NodeType::ROOT == grandfather_node_ptr->get_type()) {
                change_node_color(grandfather_node_ptr);
                return;
            } else {
                insert_update_pushup(grandfather_node_ptr);
            }
        } else if (NodeColor::BLACK == get_node_color(uncle_node_ptr)) {
            // 叔叔结点为黑色
            if (NodeType::LEFT == father_node_ptr->get_type()) {
                if (NodeType::RIGHT == node_ptr->get_type()) {
                    rotate_left(father_node_ptr);
                }
                rotate_right(grandfather_node_ptr);
                change_node_color(grandfather_node_ptr);
                change_node_color(grandfather_node_ptr->get_father());
            } else {
                if (NodeType::LEFT == node_ptr->get_type()) {
                    rotate_right(father_node_ptr);
                }
                rotate_left(grandfather_node_ptr);
                change_node_color(grandfather_node_ptr);
                change_node_color(grandfather_node_ptr->get_father());
            }
        }
    }

    Node *find_node(const T &data) {
        Node *now_ptr = root_node;
        while (nullptr != now_ptr) {
            if (now_ptr->get_data() == data) {
                return now_ptr;
            } else if (data < now_ptr->get_data()) {
                now_ptr = now_ptr->get_child(NodeType::LEFT);
            } else {
                now_ptr = now_ptr->get_child(NodeType::RIGHT);
            }
        }
        return nullptr;
    }

    int child_count(Node *ptr) {
        int cnt = 0;
        if (nullptr != ptr->get_child(NodeType::LEFT)) {
            cnt++;
        }
        if (nullptr != ptr->get_child(NodeType::RIGHT)) {
            cnt++;
        }
        return cnt;
    }

    Node *find_next_node(Node *ptr) {
        Node *now_ptr = ptr->get_child(NodeType::RIGHT);
        if (nullptr == now_ptr) {
            return nullptr;
        }

        while (nullptr != now_ptr->get_child(NodeType::LEFT)) {
            now_ptr = now_ptr->get_child(NodeType::LEFT);
        }
        return now_ptr;
    }

    NodeType get_another_type(NodeType type) {
        if (NodeType::LEFT == type) {
            return NodeType::RIGHT;
        }
        return NodeType::LEFT;
    }

    void erase_fixup_red_black(Node *node_ptr) {
        if (nullptr != node_ptr) {
            node_ptr->set_color(NodeColor::BLACK);
        }
    }

    void erase_fixup_root(Node *node_ptr) {
        if (nullptr != node_ptr) {
            node_ptr->set_color(NodeColor::BLACK);
        }
    }

    void erase_fixup_black_black(Node *father_ptr, NodeType type) {
        Node *brother_node_ptr = father_ptr->get_child(get_another_type(type));
        Node *near_nephew      = brother_node_ptr->get_child(type);
        Node *dstt_nephew      = brother_node_ptr->get_child(get_another_type(type));

        if (NodeColor::RED == get_node_color(brother_node_ptr)) {
            if (NodeType::LEFT == type) {
                rotate_left(father_ptr);

            } else {
                rotate_right(father_ptr);
            }
            exchange_two_nodes_color(father_ptr, brother_node_ptr);
            erase_fixup_black_black(father_ptr, type);
        } else if (NodeColor::BLACK == get_node_color(near_nephew) && NodeColor::BLACK == get_node_color(dstt_nephew)) {
            if (NodeColor::RED == get_node_color(father_ptr)) {
                exchange_two_nodes_color(father_ptr, brother_node_ptr);
            } else {
                brother_node_ptr->set_color(NodeColor::RED);
                if (NodeType::ROOT != father_ptr->get_type()) {
                    erase_fixup_black_black(father_ptr->get_father(), father_ptr->get_type());
                }
            }
        } else if (NodeColor::RED == get_node_color(dstt_nephew)) {
            exchange_two_nodes_color(father_ptr, brother_node_ptr);
            if (NodeType::LEFT == type) {
                rotate_left(father_ptr);
            } else {
                rotate_right(father_ptr);
            }
            if (nullptr != dstt_nephew) {
                dstt_nephew->set_color(NodeColor::BLACK);
            }
        } else {
            if (NodeType::LEFT == type) {
                rotate_right(brother_node_ptr);
            } else {
                rotate_left(brother_node_ptr);
            }
            exchange_two_nodes_color(brother_node_ptr, near_nephew);
            exchange_two_nodes_color(father_ptr, near_nephew);
            if (NodeType::LEFT == type) {
                rotate_left(father_ptr);
            } else {
                rotate_right(father_ptr);
            }
            if (nullptr != brother_node_ptr) {
                brother_node_ptr->set_color(NodeColor::BLACK);
            }
        }
    }

    void remove_node(Node *erase_ptr) {
        NodeType type       = erase_ptr->get_type();
        Node    *father_ptr = erase_ptr->get_father();
        Node    *child_ptr  = erase_ptr->get_child(NodeType::LEFT);
        if (nullptr == child_ptr) {
            child_ptr = erase_ptr->get_child(NodeType::RIGHT);
        }

        if (nullptr != father_ptr) {
            father_ptr->set_child(child_ptr, type);
        }

        if (NodeType::ROOT == type) {
            this->root_node = child_ptr;
            if (nullptr != child_ptr) {
                child_ptr->set_father(nullptr);
                child_ptr->set_type(NodeType::ROOT);
            }
        } else if (nullptr != child_ptr) {
            child_ptr->set_father(father_ptr);
        }

        if (NodeColor::RED == get_node_color(erase_ptr)) {
            erase_fixup_red_black(child_ptr);
        } else if (NodeColor::RED == get_node_color(child_ptr)) {
            erase_fixup_red_black(child_ptr);
        } else if (NodeType::ROOT == type) {
            erase_fixup_root(child_ptr);
        } else {
            erase_fixup_black_black(father_ptr, type);
        }
        delete erase_ptr;
    }

private:
    Node *root_node;
};
} // namespace frbtree