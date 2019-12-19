#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <algorithm>

const std::string Dict = "abcdefghijklmnopqrstuvwxyz";


template<class TChar>
void Change (TChar& a, TChar& b) {
    TChar& c = a;
    b = c;
}

class Cell { 
public:
    char value;
    std::vector<int32_t> not_avalible;
public:
    Cell() = default;
    Cell(const Cell& cell): value(cell.value), not_avalible(cell.not_avalible) {}
    Cell& operator = (const Cell& cell) {
        value = cell.value;
        not_avalible.clear();
        for (auto s: cell.not_avalible) {
            not_avalible.push_back(s);
        }
        return *this;
    }
    ~Cell() = default;
};

template<class InputIter>
std::vector<int32_t> BuildArray (InputIter in) {
    int32_t value;
    std::vector<int32_t> Zarray;
    value = in;
    Zarray.push_back(value);
    if (value == 0) {
        int32_t temp;
        while (temp = in && temp != -1) {
            Zarray.push_back(temp);
        }
    }
    for (int32_t i = 1; i < value; ++i) {
        int32_t temp;
        temp = in;
        Zarray.push_back(temp);
    }
    return array;
}
template<class OutPutIter>
void FromZtoString (const std::vector<int32_t>& array, OutPutIter out) {
    std::vector<Cell> cell(array.size());

    std::vector<std::unordered_set<char> > not_char(array.size());

    std::vector<int32_t> index(array.size(), 0);
    cell[0].value = 'a';
    for (int32_t i = 1; i < array.size(); ++i) {
        if (array[i] != 0) {
            int32_t count = array[i];
            int32_t head = 0;
            if (array[i] + i < array.size()) {
                cell[array[i] + i].not_avalible.push_back(array[i]);
            }
            while ((count != 0 && array[i + head] < count) || head == 0) {
                if (cell[head].value != '$') {
                    cell[i + head].value = cell[head].value;
                }
                else {
                    cell[i + head].value = '%';
                    index[i + head] = head;
                }
                --count;
                ++head;
            }
            i = i + head - 1;
        }
        else {
            not_char[i].insert('a');
            cell[i].value = '$';
        }
    }
    for (int32_t i = 0; i < cell.size(); ++i) {
        if (cell[i].value == '$') {
            for (auto number: cell[i].not_avalible) {
                not_char[i].insert(cell[number].value);
            }
            for (int32_t j = 0; j < Dict.size(); ++j) {
                if (not_char[i].find(Dict[j]) == not_char[i].end()) {
                    cell[i].value = Dict[j];
                    break;
                }
            }
        }
        else if (cell[i].value == '%') {
            cell[i].value = cell[index[i]].value;
        }
    }
    for (auto c: cell) {
        *out = c.value;
        ++out;  
    }
}

int32_t main() {
    //disabled the synchronization between the C and C++ streams
    std::ios_base::sync_with_stdio(false);
    //flushed buffer
    std::cin.tie(nullptr);
    
    std::vector<int32_t> Zarray = BuildArray(std::istream_iterator<int32_t>(std::cin));
    FromZtoString(Zarray, std::ostream_iterator<char>(std::cout));
    
    return 0;
}


