#include "test_earley.h"
#include <sstream>

TEST_F(TestAlgorithm, GrammarTest) {
    Grammar grammar;
    char left_first = 'S';
    char left_second = 'C';
    std::string right_first = "CC";
    std::string right_second = "dd";
    grammar.InsertGrammar(left_first, std::move(right_first));
    grammar.InsertGrammar(left_second, std::move(right_second));
    std::map<char, std::vector<std::string>> gram_cur = grammar.GetGrammar();
    EXPECT_EQ(gram_cur['S'].size(), 1);
    EXPECT_EQ(gram_cur['C'].size(), 1);
    EXPECT_EQ(gram_cur['S'][0], "CC");
    EXPECT_EQ(gram_cur['C'][0], "dd");
}

TEST_F(TestAlgorithm, TestConstructorAlgo) {
    Grammar grammar_first;
    char left_first = 'S';
    char left_second = 'C';
    std::string right_first = "CC";
    std::string right_second = "dd";
    grammar_first.InsertGrammar(left_first, std::move(right_first));
    grammar_first.InsertGrammar(left_second, std::move(right_second));
    Grammar grammar_second = grammar_first;
    MockAlgo algo_example(grammar_first);
    std::map<char, std::vector<std::string>> gram_cur = grammar_second.GetGrammar();
    moc_algo.SetGrammar(gram_cur);
    EXPECT_TRUE(algo_example.GetGrammar()['S'][0] == moc_algo.GetGrammar()['S'][0]);
    EXPECT_TRUE(algo_example.GetGrammar()['C'][0] == moc_algo.GetGrammar()['C'][0]);
}

