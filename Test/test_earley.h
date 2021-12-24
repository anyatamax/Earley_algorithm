#pragma once

#include "Earley_algorithm.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

class MockAlgo : public Algo {
public:
    MockAlgo() : Algo() {};
    explicit MockAlgo(Grammar &grammar) : Algo(grammar) {};
    void SetGrammar(std::map<char, std::vector<std::string>> &gram) {
        grammar_ = gram;
    }
    void SetStr(std::string &str) {
        cur_str_ = str;
    }
    void SetStateTable(std::vector<std::set<Rule>> &table) {
        state_table_ = table;
    }
    std::map<char, std::vector<std::string>> GetGrammar() {
        return grammar_;
    }
    std::set<Rule> TestScan(int index) {
        return Scan(index);
    }
    std::set<Rule> TestPredict(int index) {
        return Predict(index);
    }
    std::set<Rule> TestComplete(int index, std::set<Rule> &set) {
        return Complete(index, set);
    }

    bool TestAlgorithm(std::string str) {
        return FindString(str);
    }

private:
};

class TestAlgorithm : public ::testing::Test {
public:
    MockAlgo moc_algo;

private:
};

