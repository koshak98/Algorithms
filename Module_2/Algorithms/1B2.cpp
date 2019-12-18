#include <iostream>
#include <vector>
#include <string>
#include <unordered_set>
#include <algorithm>

const std::string Dict = "abcdefghijklmnopqrstuvwxyz";



void Change (char& a, char& b) {
    char& c = a;
    b = c;
}
class Cell { // easy
public:
    char value;
    std::vector<int> not_avalible;
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

std::vector<int> Build_Array () noexcept {
    int value;
    std::vector<int> array;
    std::cin >> value;
    array.push_back(value);
    if (value == 0) {
        int temp;
        while (std::cin >> temp && temp != -1) {
            array.push_back(temp);
        }
    }
    for (int i = 1; i < value; ++i) {
        int temp;
        std::cin >> temp;
        array.push_back(temp);
    }
    return array;
}

std::string From_Z_to_String (const std::vector<int>& array) noexcept {
    std::vector<Cell> Struct(array.size()); //bad name
    std::vector<std::unordered_set<char>> not_char(array.size());
    std::vector<int> index(array.size(), 0);
    Struct[0].value = 'a';
    for (int i = 1; i < array.size(); ++i) {
        if (array[i] != 0) {
            int count = array[i];
            int head = 0;
            if (array[i] + i < array.size()) {
                Struct[array[i] + i].not_avalible.push_back(array[i]);
            }
            while ((count != 0 && array[i + head] < count) || head == 0) {
                if (Struct[head].value != '$') {
                    Struct[i + head].value = Struct[head].value;
                }
                else {
                    Struct[i + head].value = '%';
                    index[i + head] = head;
                }
                --count;
                ++head;
            }
            i = i + head - 1;
        }
        else {
            not_char[i].insert('a');
            Struct[i].value = '$';
        }
    }
    for (int i = 0; i < Struct.size(); ++i) {
        if (Struct[i].value == '$') {
            for (auto number: Struct[i].not_avalible) {
                not_char[i].insert(Struct[number].value);
            }
            for (int j = 0; j < Dict.size(); ++j) {
                if (not_char[i].find(Dict[j]) == not_char[i].end()) {
                    Struct[i].value = Dict[j];
                    break;
                }
            }
        }
        else if (Struct[i].value == '%') {
            Struct[i].value = Struct[index[i]].value;
        }
    }
    std::string answer;
    for (auto cell: Struct) {
        answer += cell.value;
    }
    return answer;
}

int main() {
    
    std::vector<int> array = Build_Array();

    std::cout << From_Z_to_String(array) << '\n';
    
    return 0;
}


