#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <set>
#include <queue>

#define neutral 0
#define win 1
#define lose -1

const uint32_t inf = 1e9;
const uint32_t desk_positions = 4096;
typedef std::pair<uint8_t, uint8_t> Position;

//star position
Position KW = std::make_pair(3, 3);

//all correct directions
const std::vector<std::pair<int, int>> directions {
        std::make_pair(0, 1),
        std::make_pair(0, -1),
        std::make_pair(1, 0),
        std::make_pair(-1, 0),
        std::make_pair(1, 1),
        std::make_pair(1, -1),
        std::make_pair(-1, -1),
        std::make_pair(-1, 1)
};

bool CorrectPos(const Position &position) {
    return (position.first >= 1 && position.first <= 8 && position.second >= 1 && position.second <= 8);
}

bool kingThreat(const Position &King, const Position &position) {
    if (std::abs(King.second - position.second) == 1) {

        return std::abs(King.first - position.first) == 1 || std::abs(King.first - position.first) == 0;
    }
    if (std::abs(King.second - position.second) == 0) {
        return std::abs(King.first - position.first) == 1;
    }
    return false;
}

struct State {
    Position QW;
    Position KB;
    bool white_turn;
    uint32_t count;
    uint8_t flag = neutral;
    State(Position QW, Position KB, bool white_turn, int32_t count, uint8_t flag):QW(QW), KB(KB), count(count),
    flag(flag) {}
    int toInt() const {
        return ((white_turn ? 0 : desk_positions) + 64 * (8 * (QW.first - 1) + QW.second - 1) + 8 * (KB.first - 1) + KB.second - 1);
    }
    explicit State(int32_t pos): QW(((pos >= desk_positions)? pos - desk_positions : pos) / 64 / 8 + 1, ((pos >= desk_positions)? pos - desk_positions : pos) / 64 % 8 + 1)
            , KB(((pos >= desk_positions)? pos - desk_positions : pos) % 64 / 8 + 1, ((pos >= desk_positions)? pos - desk_positions : pos) % 64 % 8 + 1)
            , white_turn (pos < desk_positions) {}
    
    bool queenThreat() {
        if (QW == KB) return false;
        for (auto direction : directions) {
            for (int i = 1; i <= 8; i++) {
                Position position(QW.first + direction.first * i, QW.second + direction.second * i);
                if (!CorrectPos(position)) {
                    continue;
                }
                if (position == KW) {
                    break;
                }
                if (position == KB) {
                    return true;
                }
            }
        }
        return false;
    }

    bool mate() {
        if (!queenThreat()) {
            return false;
        }
        for (auto direction : directions) {
            Position position(KB.first + direction.first, KB.second + direction.second);
            if (!CorrectPos(position)) {
                continue;
            }
            State newDesk(QW, position, false, inf, neutral);
            if (!newDesk.queenThreat() && !kingThreat(KW, position)) {
                return false;
            }
        }
        return true;
    }

    bool check() {
        return queenThreat() || kingThreat(KW, KB);
    }

};

class EndgameSolver {
public:
    EndgameSolver() {
        states_.resize(10000);
        for (int p1 = 1; p1 <= 8; p1++) {
            for (int p2 = 1; p2 <= 8; p2++) {
                for (int q1 = 1; q1 <= 8; q1++) {
                    for (int q2 = 1; q2 <= 8; q2++) {
                        State desk1(Position(p1, p2), Position(q1, q2), true, inf, neutral);
                        State desk2(Position(p1, p2), Position(q1, q2), false, inf, neutral);
                        states_[desk1.toInt()] = inf;
                        states_[desk2.toInt()] = inf;
                    }
                }
            }
        }
    }
    int operator() (State start) {
        std::vector<int> mates = findMates();
        for (int ch : mates) {
            addQueenMoves(State(ch));
        }
        while (!queue_.empty() && states_[start.toInt()] == inf) {
            int cur_id = queue_.front();
            State cur_state(cur_id);
            queue_.pop();
            if (cur_state.white_turn) {
                int min_moves = inf;
                if (cur_state.check()) continue;
                for (auto direction : directions) {
                    for (int i = 1; i <= 8; i++) {
                        Position position(cur_state.QW.first + direction.first * i, cur_state.QW.second + direction.second * i);
                        if (!CorrectPos(position)) {
                            continue;
                        }
                        if (position == cur_state.KB || position == KW) {
                            break;
                        }
                        State new_state(position, cur_state.KB, !cur_state.white_turn, inf, neutral);
                        if (kingThreat(cur_state.KB, position) && !kingThreat(KW, position)) {
                            continue;
                        }
                        min_moves = std::min(min_moves, states_[new_state.toInt()]);
                    }
                }
                if (states_[cur_id] > min_moves + 1) {
                    states_[cur_id] = min_moves + 1;
                    addKingMoves(cur_state);
                }
            } else {
                int max_moves = -inf;
                for (auto direction: directions) {
                    Position position(cur_state.KB.first + direction.first, cur_state.KB.second + direction.second);
                    State new_state(cur_state.QW, position, !cur_state.white_turn, inf, neutral);
                    if (!CorrectPos(position)) {
                        continue;
                    }
                    if (position == cur_state.QW || position == KW) {
                        continue;
                    }
                    if (new_state.check()) {
                        continue;
                    }
                    max_moves = std::max(max_moves, states_[new_state.toInt()]);
                }
                if (max_moves != inf) {
                    if (states_[cur_id] == inf || states_[cur_id] < max_moves + 1) {
                        states_[cur_id] = max_moves + 1;
                        addQueenMoves(cur_state);
                    }
                }

            }
        }
        return states_[start.toInt()];
    }

private:

    std::vector<int> findMates() {
        std::vector<int> checkmates;
        for (int i = 1; i <= 5; i++) {
            for (int p1 = 1; p1 <= 8; p1++) {
                for (int p2 = 1; p2 <= 8; p2++) {
                    if (std::make_pair(p1, p2) == std::make_pair(i, 1)) {
                        continue;
                    }
                    if (std::make_pair(p1, p2) == std::make_pair(1, i)) {
                        continue;
                    }
                    if (p1 == KW.first && p2 == KW.first) {
                        continue;
                    }
                    State desk1(Position(p1, p2), Position(1, i), false, inf, neutral);
                    if (desk1.mate()) {
                        states_[desk1.toInt()] = 0;
                        checkmates.push_back(desk1.toInt());
                    }
                    if (i != 1) {
                        State desk2(Position(p1, p2), Position(i, 1), false, inf, neutral);
                        if (desk2.mate()) {
                            states_[desk2.toInt()] = 0;
                            checkmates.push_back(desk2.toInt());
                        }
                    }
                }
            }
        }
        return checkmates;
    }

    void addQueenMoves(State state) {
        if (state.white_turn) {
            return;
        }
        std::vector<std::pair<int, int>> directions {
        std::make_pair(0, 1),
        std::make_pair(0, -1),
        std::make_pair(1, 0),
        std::make_pair(-1, 0),
        std::make_pair(1, 1),
        std::make_pair(1, -1),
        std::make_pair(-1, -1),
        std::make_pair(-1, 1)
        };
        for (auto direction : directions) {
            for (int i = 1; i <= 8; i++) {
                Position position(state.QW.first + direction.first * i, state.QW.second + direction.second * i);
                if (!CorrectPos(position)) {
                    continue;
                }
                if (position == state.KB || position == KW) {
                    break;
                }
                State newDesk(Position(position.first, position.second), state.KB, !state.white_turn, inf, neutral);
                if (newDesk.check()) {
                    continue;
                }
                if (states_[newDesk.toInt()] != inf && states_[newDesk.toInt()] <= states_[state.toInt()] + 1) {
                    continue;
                }
                queue_.push(newDesk.toInt());
            }
        }
    }

    void addKingMoves(State state) {
        if (!state.white_turn) {
            return;
        }
        std::vector<std::pair<int, int>> directions {
        std::make_pair(0, 1),
        std::make_pair(0, -1),
        std::make_pair(1, 0),
        std::make_pair(-1, 0),
        std::make_pair(1, 1),
        std::make_pair(1, -1),
        std::make_pair(-1, -1),
        std::make_pair(-1, 1)
        };
        for (auto direction: directions) {
            Position position(state.KB.first + direction.first, state.KB.second + direction.second);
            State newDesk(state.QW, position, !state.white_turn, inf, neutral);
            if (!CorrectPos(position)) {
                continue;
            }
            if (position == state.QW || position == KW) {
                continue;
            }
            if (kingThreat(KW, position)) {
                continue;
            }
            if (states_[newDesk.toInt()] != inf && states_[newDesk.toInt()] >= states_[state.toInt()] + 1) {
                continue;
            }
            queue_.push(newDesk.toInt());
        }
    }


    std::vector<int> states_;
    std::queue<int> queue_;
};

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    char qx, kx;
    uint32_t qy, ky;
    std::cin >> qx >> qy >> kx >> ky;
    State start(Position(qx - 'a' + 1, qy), Position(kx - 'a' + 1, ky), true, inf, neutral);
    EndgameSolver solve;
    int result = solve(start);
    if (result == inf) {
        std::cout << "IMPOSSIBLE";
    } else {
        std::cout << result;
    }
    return 0;
}
