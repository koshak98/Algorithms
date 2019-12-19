#include <iostream>
#include <vector>
#include <string>
#include <map>



const std::string Dict = "abcdefghijklmnopqrstuvwxyz";


int index (char c) {
    int count = 0;
    for (auto s: Dict) {
        if (c == s) {
            break;
        }
        ++count;
    }
    return count;
}
//class Trie {
//cur_Node*
//Node*root

struct Node {
    std::map<char,Node*> son; //without *
    Node* go[26];
    Node* parent;
    Node* suffix;
    Node* up;
    Node* root;
    char char_to_parent;
    bool terminal;
    bool is_root () {
        if (parent == this) {
            return true;
        }
        else {
            return false;
        }
    }
    std::vector<int> terminal_patter_number;
    friend Node* get_suffix(Node*);
    friend Node* get_link(Node*, char);
    friend Node* get_up(Node*);
    void add_pattern(std::string, int);
    void process_text(std::string, const std::vector<int>&, std::string);

};

Node* get_suffix(Node* v) {
    if (v->suffix == nullptr) {
        if (v->is_root() || v->parent->is_root()) {
            v->suffix = v->root;
        }
        else {
            v->suffix = get_link(get_suffix(v->parent), v->char_to_parent);
        }
    }
    return v->suffix;
}

Node* get_link(Node* v, char c) {
    if (v->go[index(c)] == nullptr) {
        if (v->son.find(c) != v->son.end()) {
            v->go[index(c)] = v->son[c];
        }
        else if (v->is_root()) {
            v->go[index(c)] = v->root;
        }
        else {
            v->go[index(c)] = get_link(get_suffix(v), c);
        }
    }
    return v->go[index(c)];
}

Node* get_up(Node* v) {
    if (v->up == nullptr) {

        if (get_suffix(v) == v->root) {
            v->up = v->root;
        }
        else if (get_suffix(v)->terminal) {
            v->up = get_suffix(v);
        }
        else {
            v->up = get_up(get_suffix(v));
        }
    }
    return v->up;
}

void Node::add_pattern(std::string pattern, int pattern_number) {
    Node* cur = root;
    for (int i = 0; i < pattern.size(); ++i) {
        char c = pattern[i];
        if (cur->son[c] == nullptr) {
            cur->son[c] = new Node();
            cur->son[c]->suffix = nullptr;
            cur->son[c]->up = nullptr;
            cur->son[c]->parent = cur;
            cur->son[c]->char_to_parent = c;
            cur->son[c]->terminal = false;
            cur->son[c]->root = cur->root;
        }
        cur = cur->son[c];
    }
    cur->terminal = true;
    cur->terminal_patter_number.push_back(pattern_number);
}


void Node::process_text(std::string text, const std::vector<int>& patterns, std::string pattern) {
    std::vector<int> C(text.size());
    Node* cur = root;
    for (int i = 0; i < text.size(); ++i) {
        char c = text[i];
        Node* next = get_link(cur, c);
        cur = next;
        do {
            if (!cur->terminal_patter_number.empty()) {
                for (auto s: cur->terminal_patter_number) {
                    if (i >= patterns[s]) {
                        ++C[i - patterns[s]];
                    }
                }
            }
            cur = get_up(cur);
        } while(cur != root);
        cur = next;
    }
    for (int i = 0; i < C.size(); ++i) {
        if (C[i] == patterns.size() && i <= text.size() - pattern.size()) {
            std::cout << i << " ";
        }
    }
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);


    std::string text;
    std::string pattern;
    std::cin >> pattern >> text; //string view
    std::vector<int> number_of_faced_patterns;
    for (int i = 1; i <= pattern.size(); ++i) {
        if (pattern[i] == '?' && pattern[i - 1] != '?') {
            number_of_faced_patterns.push_back(i - 1);
        }
    }
    if (pattern[pattern.size() - 1] != '?') {
        number_of_faced_patterns.push_back(pattern.size() - 1);
    }
    std::vector<std::string> faced_patterns(number_of_faced_patterns.size());
    int count = 0;
    for (int i = 0; i < pattern.size(); ++i) {
        if (pattern[i] != '?') {
            for (; i <= number_of_faced_patterns[count]; ++i) {
                faced_patterns[count] += pattern[i];
            }
            ++count;
            --i;
        }
    }
    Node root = *new Node(); // Trie trie(patterns);
    root.parent = &root;
    root.root = &root;
    for (int i = 0; i < faced_patterns.size(); ++i) {
        root.add_pattern(faced_patterns[i], i);
    }
    root.process_text(text, number_of_faced_patterns, pattern);
    return 0;
}

