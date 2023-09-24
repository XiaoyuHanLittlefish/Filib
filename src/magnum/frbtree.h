#pragma once

#include <iostream>

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
    Rbtree()
        : root_node(nullptr) { }

    bool find(const T &data) {
        RbtreeNode<T> *now_ptr = root_node;
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
            RbtreeNode<T> *node_ptr = new RbtreeNode<T>(data);
            if (insert_node(node_ptr)) {
                insert_update_pushup(node_ptr);
            }
        } else {
            this->root_node = new RbtreeNode<T>(data);
            change_node_color(root_node);
        }
    }

private:
    bool insert_node(RbtreeNode<T> *node_ptr) {
        RbtreeNode<T> *father_node_ptr = this->root_node;
        RbtreeNode<T> *next_pos        = nullptr;
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

    NodeColor get_node_color(RbtreeNode<T> *node_ptr) {
        if (nullptr == node_ptr) {
            return NodeColor::BLACK;
        }
        return node_ptr->get_color();
    }

    void change_node_color(RbtreeNode<T> *node_ptr) {
        if (nullptr != node_ptr) {
            node_ptr->change_color();
        }
    }

    void rotate_left(RbtreeNode<T> *father_node_ptr) {
        RbtreeNode<T> *right_child_ptr      = father_node_ptr->get_child(NodeType::RIGHT);
        RbtreeNode<T> *grandfather_node_ptr = father_node_ptr->get_father();

        if (nullptr != grandfather_node_ptr) {
            NodeType type = father_node_ptr->get_type();
            grandfather_node_ptr->set_child(right_child_ptr, type);
            right_child_ptr->set_father(grandfather_node_ptr);
        } else {
            this->root_node = right_child_ptr;
            right_child_ptr->set_type(NodeType::ROOT);
        }

        RbtreeNode<T> *left_grandchild_ptr = right_child_ptr->get_child(NodeType::LEFT);
        father_node_ptr->set_child(left_grandchild_ptr, NodeType::RIGHT);
        if (nullptr != left_grandchild_ptr) {
            left_grandchild_ptr->set_father(father_node_ptr);
        }

        right_child_ptr->set_child(father_node_ptr, NodeType::LEFT);
        father_node_ptr->set_father(right_child_ptr);
    }

    void rotate_right(RbtreeNode<T> *father_node_ptr) {
        RbtreeNode<T> *left_child_ptr       = father_node_ptr->get_child(NodeType::LEFT);
        RbtreeNode<T> *grandfather_node_ptr = father_node_ptr->get_father();

        if (nullptr != grandfather_node_ptr) {
            NodeType type = father_node_ptr->get_type();
            grandfather_node_ptr->set_child(left_child_ptr, type);
            left_child_ptr->set_father(grandfather_node_ptr);
        } else {
            this->root_node = left_child_ptr;
            left_child_ptr->set_type(NodeType::ROOT);
        }

        RbtreeNode<T> *right_grandchild_ptr = left_child_ptr->get_child(NodeType::RIGHT);
        father_node_ptr->set_child(right_grandchild_ptr, NodeType::LEFT);
        if (nullptr != right_grandchild_ptr) {
            right_grandchild_ptr->set_father(father_node_ptr);
        }

        left_child_ptr->set_child(father_node_ptr, NodeType::RIGHT);
        father_node_ptr->set_father(left_child_ptr);
    }

    void insert_update_pushup(RbtreeNode<T> *node_ptr) {
        RbtreeNode<T> *father_node_ptr = node_ptr->get_father();
        // 父亲结点为黑色，无影响
        if (NodeColor::BLACK == get_node_color(father_node_ptr)) {
            return;
        }

        // 父亲结点为红色
        RbtreeNode<T> *grandfather_node_ptr = father_node_ptr->get_father();
        RbtreeNode<T> *uncle_node_ptr       = nullptr;
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

private:
    RbtreeNode<T> *root_node;
};
} // namespace frbtree