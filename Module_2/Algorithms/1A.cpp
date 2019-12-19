#include <iostream>
#include <string>
#include <vector>

std::vector<int> Z_function1(const std::string& s) {
    std::vector<int> Z(s.size());
    Z[0] = 0;
    int left = 0;
    int right = 1;
    for (int curr = 1; curr < s.size(); ++curr) {
        // Наивный пробег
        if (curr > right) {
            int i = 0;
            while (curr + i < s.size() && s[curr + i] == s[i]) {
                ++i;
            }
            Z[curr] = i;
            right = curr + i;
            left = curr;
        }
        // O(n)
        else {
            if (Z[curr - left] < right - curr) {
                Z[curr] = Z[curr - left];
            }
            else {
                int i = 0;
                while (right + i < s.size() && s[right - curr + i] == s[right + i]) {
                    ++i;
                }
                Z[curr] = right - curr + i;
                right += i;
                left = curr;
            }
        }
    }
    return Z;
}

void Algorighm_KNT(const std::string& patern,const std::string& text) {
    std::string str = patern + '$' + text;
    std::vector<int> Z = Z_function1(str);
    for (int i = patern.size() + 1; i < Z.size(); ++i) {
        if (Z[i] == patern.size()) {
            std::cout << i - patern.size() - 1 << " ";
        }
    }
}
int main() {
    std::string text;
    std::cin >> text;
    std::vector<int> Z = Z_function1(text);
    std::cout << Z.size() << " ";
    for (auto s: Z) {
        std::cout << s << " ";
    }
    return 0;
}
