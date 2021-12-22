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

TEST_F(TestAlgorithm, TestStream) {
    Grammar grammar;
    std::stringstream str;
    str << "S -> CC\n";
    str >> grammar;
    std::map<char, std::vector<std::string>> gram_cur = grammar.GetGrammar();
    std::map<char, std::vector<std::string>> grammar_second;
    grammar_second['S'].push_back("CC");
    EXPECT_EQ(gram_cur, grammar_second);
    std::stringstream str_fail;
    try {
        str_fail << "c -> Cd\n";
        str_fail >> grammar;
        FAIL();
    }
    catch (const EarleyException &expected) {
        ASSERT_STREQ("Invalid input", expected.what());
    }
}

TEST_F(TestAlgorithm, TestStream1) {
    Grammar grammar;
    std::stringstream str_fail;
    try {
        str_fail << "Cd -> Cd\n";
        str_fail >> grammar;
        FAIL();
    }
    catch (const EarleyException &expected) {
        ASSERT_STREQ("Invalid input", expected.what());
    }
}

TEST_F(TestAlgorithm, TestStream2) {
    Grammar grammar;
    std::stringstream str_fail;
    try {
        str_fail << "C\n";
        str_fail >> grammar;
        FAIL();
    }
    catch (const EarleyException &expected) {
        ASSERT_STREQ("Invalid input", expected.what());
    }
}

TEST_F(TestAlgorithm, TestStream3) {
    Grammar grammar;
    std::stringstream str_fail;
    try {
        str_fail << "C --> D\n";
        str_fail >> grammar;
        FAIL();
    }
    catch(const EarleyException& expected) {
        ASSERT_STREQ("Invalid input", expected.what());
    }
}

TEST_F(TestAlgorithm, TestStream4) {
    Grammar grammar;
    std::stringstream str_fail;
    try {
        str_fail << "C ->\n";
        str_fail >> grammar;
        FAIL();
    }
    catch(const EarleyException& expected) {
        ASSERT_STREQ("Invalid input", expected.what());
    }
}

TEST_F(TestAlgorithm, TestStream5) {
    Grammar grammar;
    std::stringstream str_fail;
    try {
        str_fail << "C ->  \n";
        str_fail >> grammar;
        FAIL();
    }
    catch(const EarleyException& expected) {
        ASSERT_STREQ("Invalid input", expected.what());
    }
}

TEST_F(TestAlgorithm, TestScan) {
    std::map<char, std::vector<std::string>> grammar;
    grammar['S'].push_back("aSbS");
    grammar['S'].push_back("a");
    moc_algo.SetGrammar(grammar);
    std::string str_find = "aabs";
    moc_algo.SetStr(str_find);
    std::vector<std::set<Rule>> D(str_find.size() + 1);
    std::set<Rule> start_rule;
    start_rule.insert({'!', "S", 0, 0});
    D[0] = start_rule;
    moc_algo.SetStateTable(D);
    std::set<Rule> new_rules = moc_algo.TestScan(0);
    EXPECT_TRUE(new_rules.empty());
    D[0].insert({'S', "aSbS", 0, 0});
    D[0].insert({'S', "a", 0, 0});
    moc_algo.SetStateTable(D);
    new_rules = moc_algo.TestScan(1);
    std::set<Rule> cur_rules;
    cur_rules.insert({'S', "aSbS", 0, 1});
    cur_rules.insert({'S', "a", 0, 1});
    for (auto &rule : new_rules) {
        EXPECT_TRUE(cur_rules.find(rule) != cur_rules.end());
    }
    EXPECT_TRUE(cur_rules.size() == new_rules.size());
}

TEST_F(TestAlgorithm, TestPredict) {
    std::map<char, std::vector<std::string>> grammar;
    grammar['S'].push_back("aSbS");
    grammar['S'].push_back("a");
    moc_algo.SetGrammar(grammar);
    std::string str_find = "aabs";
    moc_algo.SetStr(str_find);
    std::vector<std::set<Rule>> D(str_find.size() + 1);
    std::set<Rule> start_rule;
    start_rule.insert({'!', "S", 0, 0});
    D[0] = start_rule;
    moc_algo.SetStateTable(D);
    std::set<Rule> new_rules = moc_algo.TestPredict(0);
    std::set<Rule> cur_rules;
    cur_rules.insert({'S', "aSbS", 0, 0});
    cur_rules.insert({'S', "a", 0, 0});
    for (auto &rule : new_rules) {
        EXPECT_TRUE(cur_rules.find(rule) != cur_rules.end());
    }
    EXPECT_TRUE(cur_rules.size() == new_rules.size());
}

TEST_F(TestAlgorithm, TestComplete) {
    std::map<char, std::vector<std::string>> grammar;
    grammar['S'].push_back("aSbS");
    grammar['S'].push_back("a");
    moc_algo.SetGrammar(grammar);
    std::string str_find = "aabs";
    moc_algo.SetStr(str_find);
    std::vector<std::set<Rule>> D(str_find.size() + 1);
    std::set<Rule> start_rule;
    start_rule.insert({'S', "aSbS", 0, 1});
    start_rule.insert({'S', "a", 0, 1});
    D[1] = start_rule;
    start_rule.clear();
    start_rule.insert({'!', "S", 0, 0});
    D[0] = start_rule;
    moc_algo.SetStateTable(D);
    std::set<Rule> new_rules = moc_algo.TestComplete(0, D[1]);
    std::set<Rule> cur_rules;
    cur_rules.insert({'!', "S", 0, 1});
    for (auto &rule : new_rules) {
        EXPECT_TRUE(cur_rules.find(rule) != cur_rules.end());
    }
    EXPECT_TRUE(cur_rules.size() == new_rules.size());
}

TEST_F(TestAlgorithm, TestFindStr1) {
    std::map<char, std::vector<std::string>> grammar;
    grammar['S'].push_back("aSbS");
    grammar['S'].push_back("a");
    moc_algo.SetGrammar(grammar);
    EXPECT_TRUE(moc_algo.TestAlgorithm("aaba"));
    EXPECT_TRUE(moc_algo.TestAlgorithm("aabaaba"));
    EXPECT_FALSE(moc_algo.TestAlgorithm("ab"));
    EXPECT_FALSE(moc_algo.TestAlgorithm("abbababaaab"));
}

TEST_F(TestAlgorithm, TestPredictFirst) {
    std::map<char, std::vector<std::string>> grammar;
    grammar['S'].push_back("Cc");
    grammar['C'].push_back("d");
    Grammar grammar_result;
    moc_algo.SetGrammar(grammar);
    std::string str_first = "dc";
    std::string str_second = "dd";
    std::ostringstream oss;
    std::streambuf* p_cout_streambuf = std::cout.rdbuf();
    std::cout.rdbuf(oss.rdbuf());
    moc_algo.PredictWrapper(str_second);
    std::cout.rdbuf(p_cout_streambuf);
    EXPECT_TRUE(oss && oss.str() == "NO\n");
}

TEST_F(TestAlgorithm, TestPredictSecond) {
    std::map<char, std::vector<std::string>> grammar;
    grammar['S'].push_back("aSbS");
    grammar['S'].push_back("");
    Grammar grammar_result;
    moc_algo.SetGrammar(grammar);
    std::string str_first = "aabbab";
    std::ostringstream oss;
    std::streambuf* p_cout_streambuf = std::cout.rdbuf();
    std::cout.rdbuf(oss.rdbuf());
    moc_algo.PredictWrapper(str_first);
    std::cout.rdbuf(p_cout_streambuf);
    EXPECT_TRUE(oss && oss.str() == "YES\n");
}

