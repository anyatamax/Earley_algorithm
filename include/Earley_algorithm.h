#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <cctype>
#include <algorithm>
#include <stack>
#include <utility>

struct Rule {
    char left_path;
    std::string right_path;
    int index_parent = 0;
    int dot_position = 0;
    friend bool operator<(const Rule &lhs, const Rule &rhs);
    friend bool operator==(const Rule &lhs, const Rule &rhs);
};

class Grammar {
public:
    Grammar() {
        alphabet_.resize(2);
    }
    void InsertGrammar(char left_rule, std::string &&right_rule);
    [[nodiscard]] std::map<char, std::vector<std::string>>&& GetGrammar();
    [[nodiscard]] std::vector<std::set<char>>&& GetAlphabet();
    void SetGrammar(std::map<char, std::vector<std::string>> &gram, std::vector<std::set<char>> &alph);
    friend std::istream& operator>>(std::istream& in, Grammar& grammar);

protected:
    std::map<char, std::vector<std::string>> grammar_;
    std::vector<std::set<char>> alphabet_;
};

class Algo {
public:
    void PredictWrapper(Grammar &grammar, std::string &str) {
        if (FindString(grammar, str)) {
            std::cout << "YES\n";
        } else {
            std::cout << "NO\n";
        }
    }
    bool FindString(Grammar &grammar, std::string &str) {
        grammar_ = grammar.GetGrammar();
        cur_str_ = str;
        int len_str = str.size();
        state_table_.resize(len_str + 1);
        std::set<Rule> start_rule;
        start_rule.insert({'!', "S", 0, 0});
        state_table_[0] = start_rule;
        for (int i = 0; i <= len_str; ++i) {
            std::set<Rule> new_sets = Scan(i);
            for (auto &rule : new_sets) {
                state_table_[i].insert(rule);
            }
            bool change = false;
            do {
                change = false;
                new_sets = Complete(i);
                for (auto &rule : new_sets) {
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
            } while (change);
        }
        Rule finish_rule = {'!', "S", 0, 1};
        return (state_table_[len_str].find(finish_rule) != state_table_[len_str].end());
    }
    std::set<Rule> Scan(int index) {
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
    std::set<Rule> Predict(int index) {
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
    std::set<Rule> Complete(int index) {
        std::set<Rule> find_sets;
        for (auto &situation : state_table_[index]) {
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
    static bool IsNonTerminal(char letter) {
        return std::isupper(letter);
    }

    static bool IsTerminal(char letter) {
        return std::islower(letter);
    }

private:
    std::map<char, std::vector<std::string>> grammar_;
    std::vector<std::set<Rule>> state_table_;
    std::string cur_str_;
};