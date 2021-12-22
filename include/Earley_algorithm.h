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
    explicit Algo(Grammar &grammar) : grammar_(grammar.GetGrammar()){};
    void PredictWrapper(std::string &str);
    bool FindString(std::string &str);
    std::set<Rule> Scan(int index);
    std::set<Rule> Predict(int index);
    std::set<Rule> Complete(int index, std::set<Rule> &new_situations);
    static bool IsNonTerminal(char letter);
    static bool IsTerminal(char letter);

private:
    std::map<char, std::vector<std::string>> grammar_;
    std::vector<std::set<Rule>> state_table_;
    std::string cur_str_;
};