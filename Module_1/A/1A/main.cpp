#include <iostream>
#include <vector>
#include <iterator>
#include <string>


template<class TChar, class Iter>
uint32_t PrefixFuntionValue(TChar ch, uint32_t cur_value, Iter str_begin, std::vector<TChar> prefix_fun) {
    while(true) {
        // str_begin + cur_value
        if (ch == *std::next(str_begin, cur_value)) {
            return cur_value + 1;
        }
        if (cur_value == 0) {
            break;
        }
        cur_value = prefix_fun[cur_value - 1];
    }
    return 0;

}

template<class Iter, class TChar = char>
std::vector<TChar> PrefixFunction(Iter begin, Iter end) {
    std::vector<TChar> prefix_fun;
    if (std::distance(begin, end) == 0) {
        return prefix_fun;
    }
    prefix_fun.push_back(0);
    for (Iter it = begin + 1; it != end; ++it) {
        prefix_fun.push_back(PrefixFuntionValue(*it, prefix_fun.back(), begin, prefix_fun));
    }
    return prefix_fun;
}

template<class Iter, class InputIter, class OutputIter>
void AlgorithmKNT(Iter pattern_begin, Iter pattern_end, InputIter text_begin, InputIter text_end, OutputIter out) {
    auto prefix_fun = PrefixFunction(pattern_begin, pattern_end);
    uint32_t size = prefix_fun.size();
    uint32_t length = 0;
    for (int i = 0; text_begin != text_end; ++i) {
        length = PrefixFuntionValue(*text_begin, length, pattern_begin, prefix_fun);
        if (length == size) {
            *out = i + 1 - size;
        }
        ++text_begin;
    }
}


int main() {
    //disabled the synchronization between the C and C++ streams
    std::ios_base::sync_with_stdio(false);
    //flushed buffer
    std::cin.tie(nullptr);
    
    //O(p) - memory
    std::string pattern;
    std::cin >> pattern;
    //O(p + t) - time
    AlgorithmKNT(pattern.cbegin(), pattern.cend(), std::istream_iterator<char>(std::cin), std::istream_iterator<char>(), std::ostream_iterator<uint32_t>(std::cout, " "));

    return 0;
}
