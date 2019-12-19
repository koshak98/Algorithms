#include <iostream>
#include <vector>
#include <string>

//constexpr

char Jump (const std::vector<int>& array, std::string& answer) {
    std::vector<bool> flags(26, false); //std::bitset<26>
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
    return '\n'; // thorow std::runtime_error("jfsdkf");
}


void From_Prefics_To_String (std::string& answer) { // prefix
    std::vector<int> array; // bad name
    int value;
    while (std::cin >> value) {
        if (value == -1) {
            break;
        }
        array.push_back(value);
        if (value > 0) {
            answer += answer[value - 1];
        }
        else {
            answer += Jump(array, answer); //jump is bad name / next_char
        }
    }
}




int main() {
    std::string ans;
    From_Prefics_To_String(ans); // better string
    std::cout << ans;
    return 0;
}


