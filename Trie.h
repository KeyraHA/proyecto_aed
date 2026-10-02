#pragma once
#include <iostream>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <string>

template<class T = char>
struct Node {
    std::unordered_map<T, Node<T> *> children;
    bool endpoint=false;
    Node()=default;
};

template<class T = char>
class Trie {
protected:
    Node<T>* root=nullptr;
    size_t elements=0;
public:
    using Word = std::basic_string<T>;
    Trie(): root(new Node<T>()) {}
    size_t size() const { return elements; }

    Trie (const Trie &other): root(new Node<T>()), elements(other.elements) {
        std::vector<std::pair<const Node<T>*, Node<T>*>> pending {{other.root, root}};
        while (!pending.empty()) {
            auto [src, dest] = pending.back();
            pending.pop_back();
            dest->endpoint = src->endpoint;
            for (const auto& [key, val] : src->children) {
                dest->children[key] = new Node<T>();
                pending.push_back({val, dest->children[key]});
            }
        }
    }

    Trie(Trie&& other): root(other.root), elements(other.elements) {
        other.root=new Node<T>();
        other.elements=0;
    }

    Trie& operator=(Trie other) {
        std::swap(root, other.root);
        std::swap(elements, other.elements);
        return *this;
    }

    ~Trie() {
        std::vector<Node<T>*> pending{root};
        while (!pending.empty()) {
            Node<T>* node=pending.back();
            pending.pop_back();
            for (auto& [key, val] : node->children) {
                pending.push_back(val);
            }
            delete node;
        }
    }

    void insert(const Word& word) {
        Node<T>* current=root;
        for (const T& c : word) {
            auto it = current->children.find(c);
            if (it == current->children.end())
                it = current->children.emplace(c, new Node<T>()).first;
            current=it->second;
        }
        if (!current->endpoint) {
            current->endpoint=true;
            elements++;
        }
    }

    bool search(const Word& word) const {
        const Node<T>* current=root;
        for (const T& c : word) {
            auto it = current->children.find(c);
            if (it == current->children.end()) {
                return false;
            }
            current=it->second;
        }
        return current->endpoint && current!=nullptr;
    }

    void remove(const Word& word) {
        std::vector<Node<T>*> path{root};
        for (const T& c : word) {
            auto it = path.back()->children.find(c);
            if (it == path.back()->children.end())
                return;
            path.push_back(it->second);
        }
        if (!path.back()->endpoint)
            return;
        path.back()->endpoint=false;
        elements--;
        for (size_t i=path.size()-1; i>0; i--) {
            if (!path[i]->children.empty() || path[i]->endpoint)
                break;
            path[i-1]->children.erase(word[i-1]);
            delete path[i];
        }
    }

    bool startWith(const Word& prefix) const {
        const Node<T>* current=root;
        for (const T& c : prefix) {
            auto it = current->children.find(c);
            if (it == current->children.end())
                return false;
            current=it->second;
        }
        return true;
    }

    std::vector<Word> autocomplete(const Word& prefix) const {
        std::vector<Word> out;
        const Node<T>* current=root;
        for (const T& c : prefix) {
            auto it = current->children.find(c);
            if (it == current->children.end())
                return out;
            current=it->second;
        }
        std::vector<std::pair<const Node<T>*, Word>> pending {{current, prefix}};
        while (!pending.empty()) {
            auto [src, dest] = pending.back();
            pending.pop_back();
            if (src->endpoint)
                out.push_back(dest);
            for (const auto& [key, val] : src->children)
                pending.push_back({val, dest+key});
        }
        return out;
    }
};