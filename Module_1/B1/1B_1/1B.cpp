#include <iostream>
#include <vector>
#include <string>

const uint32_t alphabet_size = 26;
const uint32_t alphabet_size_full = 97;


template<class TChar>
TChar next_char (const std::vector<uint32_t>& array, std::string& answer) {
    // std::vector<bool> flags(26, false);
    std::bitset<alphabet_size> flags;
    int cur_index = (array.size() > 1) ? array[array.size() - 2] : -1;
    while (cur_index >= 0) {
        flags[int(answer[cur_index]) - 97] = true; //static_cast
        if (cur_index == 0) {
            break;
        }
        else {
            cur_index = array[cur_index - 1];
        }
    }
    for (int i = 0; i < flags.size(); ++i) { //new fun
        if (flags[i] == false) {
            return static_cast<char>(i + 97);
        }
    }
    throw std::runtime_error("error with exit");
}

template<class InputIter, class OutputIter>
void converse_prefix_string (InputIter in, OutputIter out) {
    std::vector<uint32_t> array;
    std::string ans;
    while(auto value = in) {
        array.push_back(value);
        if (value > 0) {
            ans += ans[value -1];
            *out = next_char<char>(array, ans);
        }
        else {
            ans += next_char<char>(array, ans);
            *out = next_char<char>(array, ans);
        }
    }
}




int main() {
    //disabled the synchronization between the C and C++ streams
    std::ios_base::sync_with_stdio(false);
    //flushed buffer
    std::cin.tie(nullptr);
    
    converse_prefix_string(std::istream_iterator<uint32_t>(std::cin), std::ostream_iterator<char>(std::cout));
    return 0;
}


