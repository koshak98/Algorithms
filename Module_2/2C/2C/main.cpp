

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstring>
#include <stdexcept>

constexpr int inf = 0x7fffffff;
const std::string alphabet = "  !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";


int index (char c) {
    for (int i = 0; i < alphabet.size(); ++i) {
        if (c == alphabet[i]) {
            return i;
        }
    }
    throw std::runtime_error("eroor");
}

///////////////////////////////////////////////////////////////////////////////////


std::vector<int> first_step(const std::string& text) {
    std::vector<int> first_permutation(text.size());
    std::vector<int> cnt(alphabet.size(), 0);
    for (int i = 0; i < text.size(); ++i) {
        ++cnt[index(text[i])];
    }
    for (int i = 1; i < alphabet.size(); ++i) {
        cnt[i] += cnt[i - 1];
    }
    for (int i = text.size() - 1; i >= 0; --i) {
        first_permutation[--cnt[index(text[i])]] = i;
    }
    return first_permutation;
}


class suff_array {
private:
    std::vector<int> permutation;
    std::vector<int> classes;
    int count_of_classes;
    
public:
    suff_array(const std::string&);
    std::vector<int> first_step(const std::vector<int>&, const std::vector<int>&, int);
    void next_step(int);
    std::vector<int> get_lcp(const std::string&);
    int get_suffix(int);
};


int suff_array::get_suffix(int index) {
    return permutation[index];
}
std::vector<int> suff_array::first_step(const std::vector<int>& old_array, const std::vector<int>& classes_order, int classes_power) {
    std::vector<int> new_array(old_array.size());
    std::vector<int> cnt(classes_power, 0);
    for (int i = 0; i < old_array.size(); ++i) {
        ++cnt[classes_order[old_array[i]]];
    }
    for (int i = 1; i < classes_power; ++i) {
        cnt[i] += cnt[i - 1];
    }
    for (int i = old_array.size() - 1; i >= 0; --i) {
        new_array[--cnt[classes_order[old_array[i]]]] = i;
    }
    return new_array;
}

suff_array::suff_array(const std::string& text) {
    //step k == 0 step
    std::vector<int> temp_permutation = ::first_step(text);
    permutation.resize(temp_permutation.size() + 1);
    permutation[0] = text.size();
    for (int i = 0; i < temp_permutation.size(); ++i) {
        permutation[i + 1] = temp_permutation[i];
    }
    
    classes.resize(permutation.size());
    classes[permutation[0]] = 0;
    count_of_classes = 1;
    for (int i = 1; i < permutation.size(); ++i) {
        if (text[permutation[i]] != text[permutation[i - 1]]) {
            ++count_of_classes;
        }
        classes[permutation[i]] = count_of_classes - 1;
    }
       
    for (int step = 0; (1u << step) < permutation.size(); ++step) {
        next_step(step);
    }
    
}

void suff_array::next_step(int step) {
    std::vector<int> next_permutation(permutation);
 
    for (int i = 0; i < permutation.size(); ++i) {
        next_permutation[i] = permutation[i] - (1u << step);
        if (next_permutation[i] < 0) {
            next_permutation[i] += permutation.size();
        }
    }
 
    std::vector<int> permutation_temp = first_step(next_permutation, classes, count_of_classes);
    for (int i = 0; i < permutation.size(); ++i) {
        permutation[i] = next_permutation[permutation_temp[i]];
    }
    
    std::vector<int> new_classes(classes.size());
    new_classes[permutation[0]] = 0;
    count_of_classes = 1;
    for (int i = 1; i < permutation.size(); ++i) {
        int mid1 = (permutation[i - 1] + (1u << step)) % permutation.size();
        int mid2 = (permutation[i] + (1u << step)) % permutation.size();
        
        if (classes[permutation[i]] != classes[permutation[i - 1]] || classes[mid1] != classes[mid2]) {
            ++count_of_classes;
        }
        new_classes[permutation[i]] = count_of_classes - 1;
    }
    classes = new_classes;
}
 
std::vector<int> suff_array::get_lcp(const std::string& text) {
    std::vector<int> lcp(permutation.size(), 0);
    std::vector<int> inverse_suff(permutation.size(), 0);
 
    for (int i = 0; i < permutation.size(); ++i) {
        inverse_suff[permutation[i]] = i;
    }
    
    int count_of_lcp = 0;
    for (int i = 0; i < permutation.size() - 1; ++i) {
        if (count_of_lcp) {
            --count_of_lcp;
        }
        if (inverse_suff[i] == permutation.size() - 1) {
            count_of_lcp = 0;
            continue;
        }
        int next = permutation[inverse_suff[i] + 1];
        while (std::max(i + count_of_lcp, next + count_of_lcp) < permutation.size() &&
                text[i + count_of_lcp] == text[next + count_of_lcp]) {
            ++count_of_lcp;
        }
        lcp[inverse_suff[i]] = count_of_lcp;
    }
    return lcp;
}





///////////////////////////////////////////////////////////////////////////////////
std::string find_k_substring(suff_array& array, std::vector<int>& lcp, const std::string& text, long long k) {
    int size = text.size();
    int min_lcp = 0;
    for (int i = 3; i < size; ++i) {
        int curr_num = array.get_suffix(i);
        int next_num = array.get_suffix(i + 1);
        if ((curr_num < array.get_suffix(1) && next_num > array.get_suffix(1)) || ((curr_num > array.get_suffix(1) && next_num < array.get_suffix(1)) && lcp[i] > 0)) {
            if (k <= lcp[i] - min_lcp) {
                std::string answer;
                for (int j = 0; j < min_lcp + k; ++j) {
                    answer += text[curr_num + j];
                }
                return answer;
            }
            else {
                if (min_lcp <= lcp[i]) {
                    k += min_lcp - lcp[i];
                }
                min_lcp = lcp[i];
            }
        }
        if (min_lcp > lcp[i]) {
            min_lcp = lcp[i];
        }
    }
    return "-1";
}


int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    
    std::string first_str;
    std::string second_str;
    uint32_t k = 0;
    std::cin >> first_str >> second_str >> k;
    std::string total_str = first_str + '#' + second_str + '$';
    suff_array array(total_str);
    std::vector<int> lcp = array.get_lcp(total_str);
    
    
    std::cout << find_k_substring(array, lcp, total_str, k) << '\n';
    
    
    return 0;
}

