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

template<class TChar = char>
struct Node;


template<class TChar = char>
class Trie {
private:
    Node<TChar> cur_node;
public:
    Trie() = default;
    Trie(Node<TChar> node):cur_node(node) {}
    Node<TChar>* get_suffix();
    Node<TChar>* get_link(TChar);
    Node<TChar>* get_up();
    void add_pattern(std::string, int);
    void process_text(std::string, const std::vector<int>&, std::string);

};


template<class TChar>
struct Node {
    std::map<TChar,Node*> son;
    Node<TChar>* go[26];
    Node<TChar>* parent;
    Node<TChar>* suffix;
    Node<TChar>* up;
    Node<TChar>* root;
    TChar char_to_parent;
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

    ~Node() = default;

};
template<class TChar>
Node<TChar>* Trie<TChar>::get_suffix() {
    if (cur_node.suffix == nullptr) {
        if (cur_node.is_root() || cur_node.parent->is_root()) {
            cur_node.suffix = cur_node.root;
        }
        else {
            cur_node.suffix = get_link(cur_node.char_to_parent);
        }
    }
    return cur_node.suffix;
}
template<class TChar>
Node<TChar>* Trie<TChar>::get_link(TChar c) {
    if (cur_node.go[index(c)] == nullptr) {
        if (cur_node.son.find(c) != cur_node.son.end()) {
            cur_node.go[index(c)] = cur_node.son[c];
        }
        else if (cur_node.is_root()) {
            cur_node.go[index(c)] = cur_node.root;
        }
        else {
            cur_node.go[index(c)] = get_link(c);
        }
    }
    return cur_node.go[index(c)];
}
template<class TChar>
Node<TChar>* Trie<TChar>::get_up () {
    if (cur_node.up == nullptr) {

        if (get_suffix() == cur_node.root) {
            cur_node.up = cur_node.root;
        }
        else if (get_suffix()->terminal) {
            cur_node.up = get_suffix();
        }
        else {
            cur_node.up = get_suffix();
        }
    }
    return cur_node.up;
}
template<class TChar>
void Trie<TChar>::add_pattern(std::string pattern, int pattern_number) {
    Node<TChar>* cur = cur_node.root;
    for (int i = 0; i < pattern.size(); ++i) {
        char c = pattern[i];
        if (cur->son[c] == nullptr) {
            cur->son[c] = new Node<TChar>();
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

template<class TChar>
void Trie<TChar>::process_text(std::string text, const std::vector<int>& patterns, std::string pattern) {
    std::vector<int> C(text.size());
    Node<TChar>* cur = cur_node.root;
    for (int i = 0; i < text.size(); ++i) {
        char c = text[i];
        Node<TChar>* next = get_link(c);
        cur = next;
        do {
            if (!cur->terminal_patter_number.empty()) {
                for (auto s: cur->terminal_patter_number) {
                    if (i >= patterns[s]) {
                        ++C[i - patterns[s]];
                    }
                }
            }
            cur = get_up();
        } while(cur != cur_node.root);
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
    std::cin >> pattern >> text;
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
    Node<char> root;
    root.parent = &root;
    root.root = &root;
    Trie<char> trie(root);
    for (int i = 0; i < faced_patterns.size(); ++i) {
        trie.add_pattern(faced_patterns[i], i);
    }
    trie.process_text(text, number_of_faced_patterns, pattern);
    return 0;
}
