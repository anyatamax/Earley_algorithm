#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <map>
#include <cctype>
#include <algorithm>
#include <stack>
#include <utility>

class EarleyException : public  std::exception {
public:
    EarleyException(std::string err) : error_(err) {};
    const char* what() const noexcept override;

private:
    std::string error_;
};

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
    void InsertGrammar(char left_rule, std::string &&right_rule);
    [[nodiscard]] std::map<char, std::vector<std::string>>&& GetGrammar();
    void SetGrammar(std::map<char, std::vector<std::string>> &gram);
    friend std::istream& operator>>(std::istream& in, Grammar& grammar);

protected:
    std::map<char, std::vector<std::string>> grammar_;
};

class Algo {
public:
    Algo() = default;
    explicit Algo(Grammar &grammar) : grammar_(grammar.GetGrammar()){};
    void PredictWrapper(std::string &str);
    static bool IsNonTerminal(char letter);
    static bool IsTerminal(char letter);

protected:
    std::map<char, std::vector<std::string>> grammar_;
    std::vector<std::set<Rule>> state_table_;
    std::string cur_str_;
    bool FindString(std::string &str);
    std::set<Rule> Scan(int index);
    std::set<Rule> Predict(int index);
    std::set<Rule> Complete(int index, std::set<Rule> &new_situations);
};