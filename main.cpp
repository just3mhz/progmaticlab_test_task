#include <iostream>
#include <string>
#include <vector>
#include <stack>

using TVecIterator = std::vector<int>::const_iterator;

struct TExpression {
    std::vector <int> operands;
    std::vector <char> operators;
};

bool GetFirstExpression(TVecIterator left_it, 
               TVecIterator right_it, 
               int needed,
               char last_op,
               TExpression &expr) {
    if (left_it == right_it) {
        return needed != 0 ? false : true;
    }
    int lhs = 0;
    int lhs_mult = (last_op == '+' ? +1 : -1);
    while (left_it != right_it) {
        lhs = (lhs * 10) + (*left_it);
        int new_needed = needed - (lhs_mult * lhs);
        ++left_it;
        if (GetFirstExpression(left_it, right_it, new_needed, '+', expr)) {
            expr.operands.push_back(lhs);
            expr.operators.push_back('+');
            return true;
        }
        if (GetFirstExpression(left_it, right_it, new_needed, '-', expr)) {
            expr.operands.push_back(lhs);
            expr.operators.push_back('-');
            return true;
        }
    };
    return false;
}

TExpression Solve(const std::vector<int> &nums, int needed = 200) {
    TExpression expr;
    if (!GetFirstExpression(nums.begin(), nums.end(), needed, '+', expr)) {
        throw std::invalid_argument("No expression found");
    }
    return expr;
}

std::ostream& operator << (std::ostream &os, const TExpression &expr) {
    auto operands_it = expr.operands.rbegin();
    auto operators_it = expr.operators.rbegin();
    bool first = true;
    while ( operands_it != expr.operands.rend()) {
        if (!first) {
            os << *operators_it;
            ++operators_it;
        }
        else {
            first = false;
        }
        os << *operands_it;
        ++operands_it;
    }
    return os;
}


// Вычисление выражения. Необходимо для тестирования
int Evaluate(const TExpression &expr) {
    auto operands_it = expr.operands.rbegin();
    auto operators_it = expr.operators.rbegin();
    int result = *operands_it;
    ++operands_it;
    while( operands_it != expr.operands.rend() ) {
       switch( *operators_it) {
           case '+':
               result += *operands_it;
               break;
           case '-':
               result -= *operands_it;
               break;
       }
       ++operators_it;
       ++operands_it;
    }
    return result;
}


// Простой юнит тест
void TestSolve() {
    {
        int expected = 200;
        std::vector <int> nums = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
        if ( Evaluate(Solve(nums, expected)) != expected ) {
            std::cerr << "TestSolve failed!";
            std::exit(1);
        }
    }
}


int main() {
    TestSolve();
    
    std::vector <int> nums = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
    TExpression expr = Solve(nums);
    std::cout << expr << std::endl;
}
