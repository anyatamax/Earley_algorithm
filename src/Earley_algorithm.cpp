#include "Earley_algorithm.h"

bool operator<(const Rule &lhs, const Rule &rhs) {
    if (lhs.left_path == rhs.left_path) {
        if (rhs.right_path == lhs.right_path) {
            if (lhs.next == rhs.next) {
                return lhs.dot_position < rhs.dot_position;
            }
            return lhs.next < rhs.next;
        }
        return rhs.right_path < lhs.right_path;
    }
    return lhs.left_path < rhs.left_path;
}

bool operator==(const Rule &lhs, const Rule &rhs) {
    return (lhs.left_path == rhs.left_path &&
            lhs.right_path == rhs.right_path &&
            lhs.next == rhs.next &&
            lhs.dot_position == rhs.dot_position);
}

void Grammar::InsertGrammar(char left_rule, std::string &&right_rule) {
    grammar_[left_rule].push_back(right_rule);
}

std::vector<std::set<char>>&& Grammar::GetAlphabet() {
    return std::move(alphabet_);
}

std::map<char, std::vector<std::string>>&& Grammar::GetGrammar() {
    return std::move(grammar_);
}

void Grammar::SetGrammar(std::map<char, std::vector<std::string>> &gram, std::vector<std::set<char>> &alph) {
    grammar_ = gram;
    alphabet_ = alph;
}

std::istream& operator>>(std::istream& in, Grammar& grammar) {
    std::string rule;
    std::getline(in, rule);
    int index = 0;
    if (!Algo::IsNonTerminal(rule[index])) {
        throw LRException("Invalid input");
    }
    grammar.alphabet_[1].insert(rule[index]);
    ++index;
    if (rule[index] != ' ' && rule[index] != '-') {
        throw LRException("Invalid input");
    }
    while (index < rule.size() && rule[index] == ' ') {
        ++index;
    }
    if (index >= rule.size() - 1) {
        throw LRException("Invalid input");
    }
    if (rule[index] != '-' || rule[index + 1] != '>') {
        throw LRException("Invalid input");
    }
    index += 2;
    if (index > rule.size() - 1) {
        throw LRException("Invalid input");
    }
    while (index < rule.size() && rule[index] == ' ') {
        ++index;
    }
    if (index == rule.size()) {
        throw LRException("Invalid input");
    }
    grammar.InsertGrammar(rule[0], rule.substr(index, rule.size()));
    for (int i = index; i < rule.size(); ++i) {
        if (std::isalpha(rule[i])) {
            if (Algo::IsTerminal(rule[i])) {
                grammar.alphabet_[0].insert(rule[i]);
            }
            if (Algo::IsNonTerminal(rule[i])) {
                grammar.alphabet_[1].insert(rule[i]);
            }
        } else if (rule[i] != '$') {
            grammar.alphabet_[0].insert(rule[i]);
        } else {
            throw LRException("Invalid input");
        }
    }
    return in;
}
