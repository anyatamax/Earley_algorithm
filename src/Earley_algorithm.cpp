#include "Earley_algorithm.h"

const char* EarleyException::what() const noexcept {
    return error_.c_str();
}

bool operator<(const Rule &lhs, const Rule &rhs) {
    if (lhs.left_path == rhs.left_path) {
        if (rhs.right_path == lhs.right_path) {
            if (lhs.index_parent == rhs.index_parent) {
                return lhs.dot_position < rhs.dot_position;
            }
            return lhs.index_parent < rhs.index_parent;
        }
        return rhs.right_path < lhs.right_path;
    }
    return lhs.left_path < rhs.left_path;
}

bool operator==(const Rule &lhs, const Rule &rhs) {
    return (lhs.left_path == rhs.left_path &&
            lhs.right_path == rhs.right_path &&
            lhs.index_parent == rhs.index_parent &&
            lhs.dot_position == rhs.dot_position);
}

void Grammar::InsertGrammar(char left_rule, std::string &&right_rule) {
    if (right_rule == ".") {
        grammar_[left_rule].push_back("");
    } else {
        grammar_[left_rule].push_back(right_rule);
    }
}

std::map<char, std::vector<std::string>>&& Grammar::GetGrammar() {
    return std::move(grammar_);
}

void Grammar::SetGrammar(std::map<char, std::vector<std::string>> &gram) {
    grammar_ = gram;
}

std::istream& operator>>(std::istream& in, Grammar& grammar) {
    std::string rule;
    std::getline(in, rule);
    int index = 0;
    if (!Algo::IsNonTerminal(rule[index])) {
        throw EarleyException("Invalid input");
    }
    ++index;
    if (rule[index] != ' ' && rule[index] != '-') {
        throw EarleyException("Invalid input");
    }
    while (index < rule.size() && rule[index] == ' ') {
        ++index;
    }
    if (index >= rule.size() - 1) {
        throw EarleyException("Invalid input");
    }
    if (rule[index] != '-' || rule[index + 1] != '>') {
        throw EarleyException("Invalid input");
    }
    index += 2;
    if (index > rule.size() - 1) {
        throw EarleyException("Invalid input");
    }
    while (index < rule.size() && rule[index] == ' ') {
        ++index;
    }
    if (index == rule.size()) {
        throw EarleyException("Invalid input");
    }
    grammar.InsertGrammar(rule[0], rule.substr(index, rule.size()));
    return in;
}

void Algo::PredictWrapper(std::string &str) {
    if (FindString(str)) {
        std::cout << "YES\n";
    } else {
        std::cout << "NO\n";
    }
}
bool Algo::FindString(std::string &str) {
    cur_str_ = str;
    int len_str = str.size();
    state_table_ = std::vector<std::set<Rule>>(len_str + 1);
    std::set<Rule> start_rule;
    start_rule.insert({'!', "S", 0, 0});
    state_table_[0] = start_rule;
    for (int i = 0; i <= len_str; ++i) {
        std::set<Rule> new_sets = Scan(i);
        std::set<Rule> new_sets_complete;
        for (auto &rule : new_sets) {
            state_table_[i].insert(rule);
        }
        bool change = false;
        bool flag_first = true;
        do {
            change = false;
            if (flag_first || new_sets_complete.empty()) {
                new_sets_complete = Complete(i, state_table_[i]);
            } else {
                new_sets_complete = Complete(i, new_sets_complete);
            }
            for (auto &rule : new_sets_complete) {
                if (state_table_[i].find(rule) == state_table_[i].end()) {
                    state_table_[i].insert(rule);
                    change = true;
                }
            }
            new_sets = Predict(i);
            for (auto &rule : new_sets) {
                if (state_table_[i].find(rule) == state_table_[i].end()) {
                    state_table_[i].insert(rule);
                    change = true;
                }
            }
            flag_first = false;
        } while (change);
    }
    Rule finish_rule = {'!', "S", 0, 1};
    return (state_table_[len_str].find(finish_rule) != state_table_[len_str].end());
}
std::set<Rule> Algo::Scan(int index) {
    std::set<Rule> find_sets;
    if (index == 0) {
        return find_sets;
    }
    for (auto &situation : state_table_[index - 1]) {
        if (situation.dot_position < situation.right_path.size()) {
            char symbol = situation.right_path[situation.dot_position];
            if (IsTerminal(symbol) && symbol == cur_str_[index - 1]) {
                Rule new_rule = situation;
                ++new_rule.dot_position;
                find_sets.insert(new_rule);
            }
        }
    }
    return find_sets;
}
std::set<Rule> Algo::Predict(int index) {
    std::set<Rule> find_sets;
    for (auto &situation : state_table_[index]) {
        if (situation.dot_position < situation.right_path.size()) {
            char symbol = situation.right_path[situation.dot_position];
            if (IsNonTerminal(symbol)) {
                for (auto &grammar_rule : grammar_[symbol]) {
                    Rule new_rule = {symbol, grammar_rule, index, 0};
                    find_sets.insert(new_rule);
                }
            }
        }
    }
    return find_sets;
}
std::set<Rule> Algo::Complete(int index, std::set<Rule> &new_situations) {
    std::set<Rule> find_sets;
    for (auto &situation : new_situations) {
        if (situation.dot_position == situation.right_path.size()) {
            int parent_id = situation.index_parent;
            char transition = situation.left_path;
            for (auto &complete_situation : state_table_[parent_id]) {
                if (complete_situation.dot_position < complete_situation.right_path.size()) {
                    if (complete_situation.right_path[complete_situation.dot_position] == transition) {
                        Rule new_rule = complete_situation;
                        ++new_rule.dot_position;
                        find_sets.insert(new_rule);
                    }
                }
            }
        }
    }
    return find_sets;
}
bool Algo::IsNonTerminal(char letter) {
    return std::isupper(letter);
}

bool Algo::IsTerminal(char letter) {
    return std::islower(letter);
}
