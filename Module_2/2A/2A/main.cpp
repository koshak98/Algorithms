#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstring>
#include <stdexcept>

const std::string alphabet = "  !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

template <class _SizeType>
_SizeType index (char c) {
    if (c == '\0') {
        return 0;
    }
    for (_SizeType i = 0; i < alphabet.size(); ++i) {
        if (c == alphabet[i]) {
            return i;
        }
    }
    throw std::runtime_error("undefined symbol");
}


template <class _SizeType, class Iter>
std::vector<_SizeType> first_step(Iter begin, Iter end) {
    std::vector<_SizeType> first_permutation(end - begin);
    std::vector<_SizeType> cnt(alphabet.size(), 0);
    Iter i = begin;
    for (;i != end; ++i) {
        ++cnt[index<_SizeType>(*i)];
    }
    
    for (_SizeType i = 1; i < alphabet.size(); ++i) {
        cnt[i] += cnt[i - 1];
    }
    for (; end != begin; --end) {
        first_permutation[--cnt[index<_SizeType>(*end)]] = end - begin;
    }
    return first_permutation;
}

/////////////////////////////////////////////////////////////////////////////////////////////

template <class _SizeType>
class suff_array {
private:
    std::vector<_SizeType> permutation;
    std::vector<_SizeType> classes;
    _SizeType count_of_classes;
    
public:
    template <class InputIter>
    suff_array(InputIter, InputIter);
    std::vector<_SizeType> first_step(const std::vector<_SizeType>&, const std::vector<_SizeType>&, _SizeType);
    void next_step(_SizeType);
    template <class InputIter>
    std::vector<_SizeType> get_lcp(InputIter, InputIter);
    _SizeType get_suffix(_SizeType);
};

template <class _SizeType>
_SizeType suff_array<_SizeType>::get_suffix(_SizeType index) {
    return permutation[index];
}

template <class _SizeType>
std::vector<_SizeType> suff_array<_SizeType>::first_step(const std::vector<_SizeType>& old_array, const std::vector<_SizeType>& classes_order, _SizeType classes_power) {
    std::vector<_SizeType> new_array(old_array.size());
    std::vector<_SizeType> cnt(classes_power, 0);
    for (_SizeType i = 0; i < old_array.size(); ++i) {
        ++cnt[classes_order[old_array[i]]];
    }
    for (_SizeType i = 1; i < classes_power; ++i) {
        cnt[i] += cnt[i - 1];
    }
    for (_SizeType i = old_array.size() - 1; i >= 0; --i) {
        new_array[--cnt[classes_order[old_array[i]]]] = i;
    }
    return new_array;
}

template <class _SizeType>
template <class InputIter>
suff_array<_SizeType>::suff_array(InputIter begin, InputIter end) {
    //step k == 0 step
    std::vector<_SizeType> temp_permutation = ::first_step<_SizeType, InputIter>(begin, end);
    permutation.resize(temp_permutation.size() + 1);
    _SizeType size = end - begin;
    permutation[0] = size;
    for (_SizeType i = 0; i < size; ++i) {
        permutation[i + 1] = temp_permutation[i];
    }
    classes.resize(permutation.size());
    classes[permutation[0]] = 0;
    count_of_classes = 1;
    for (_SizeType i = 1; i < size; ++i) {
        if (*(begin + permutation[i]) != * (begin + permutation[i - 1])) {
            ++count_of_classes;
        }
        classes[permutation[i]] = count_of_classes - 1;
    }
       
    for (_SizeType step = 0; (1u << step) < permutation.size(); ++step) {
        next_step(step);
    }
    
}
template <class _SizeType>
void suff_array<_SizeType>::next_step(_SizeType step) {
    std::vector<_SizeType> next_permutation(permutation);
 
    for (_SizeType i = 0; i < permutation.size(); ++i) {
        next_permutation[i] = permutation[i] - (1u << step);
        if (next_permutation[i] < 0) {
            next_permutation[i] += permutation.size();
        }
    }
 
    std::vector<_SizeType> permutation_temp = first_step(next_permutation, classes, count_of_classes);
    for (_SizeType i = 0; i < permutation.size(); ++i) {
        permutation[i] = next_permutation[permutation_temp[i]];
    }
    
    std::vector<_SizeType> new_classes(classes.size());
    new_classes[permutation[0]] = 0;
    count_of_classes = 1;
    for (_SizeType i = 1; i < permutation.size(); ++i) {
        _SizeType mid1 = (permutation[i - 1] + (1u << step)) % permutation.size();
        _SizeType mid2 = (permutation[i] + (1u << step)) % permutation.size();
        
        if (classes[permutation[i]] != classes[permutation[i - 1]] || classes[mid1] != classes[mid2]) {
            ++count_of_classes;
        }
        new_classes[permutation[i]] = count_of_classes - 1;
    }
    classes = new_classes;
}

template <class _SizeType>
template <class Iter>
std::vector<_SizeType> suff_array<_SizeType>::get_lcp(Iter begin, Iter end) {
    std::vector<_SizeType> lcp(permutation.size(), 0);
    std::vector<_SizeType> inverse_suff(permutation.size(), 0);
 
    for (_SizeType i = 0; i < permutation.size(); ++i) {
        inverse_suff[permutation[i]] = i;
    }
    
    _SizeType count_of_lcp = 0;
    for (_SizeType i = 0; i < permutation.size() - 1; ++i) {
        if (count_of_lcp) {
            --count_of_lcp;
        }
        if (inverse_suff[i] == permutation.size() - 1) {
            count_of_lcp = 0;
            continue;
        }
        _SizeType next = permutation[inverse_suff[i] + 1];
        while (std::max(i + count_of_lcp, next + count_of_lcp) < permutation.size() &&
                *(begin + i + count_of_lcp) == *(begin + next + count_of_lcp)) {
            ++count_of_lcp;
        }
        lcp[inverse_suff[i]] = count_of_lcp;
    }
    return lcp;
}

/////////////////////////////////////////////////////////////////////////////////////////////

template <class _SizeType, class Iter>
_SizeType number_of_substrings(Iter begin, Iter end) {
    
    suff_array<_SizeType> suff(begin, end);
    std::vector<_SizeType> lcp = suff.get_lcp(begin, end);
    _SizeType count = 0;
    for (_SizeType i = 1; i <= end - begin; ++i) {
        count += (end - begin - suff.get_suffix(i)) - lcp[i];
    }
 
    return count;
}



int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    std::string text;
    getline(std::cin, text);
    
    std::cout << number_of_substrings<int>(text.cbegin(), text.cend()) << '\n';
    return 0;
}
