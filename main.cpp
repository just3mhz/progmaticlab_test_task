#include <iostream>
#include <string>
#include <vector>
#include <deque>

using TVecIterator = std::vector<int>::const_iterator;

struct TExpression {
    std::deque <int> operands;
    std::deque <char> operators;
};

void PushPart(TExpression &expr, char op, int value) {
    expr.operators.push_back(op);
    expr.operands.push_back(value);
}

void PopPart(TExpression &expr) {
    expr.operators.pop_back();
    expr.operands.pop_back();
}

void GetExpressions(TVecIterator left_it, TVecIterator right_it,
        TExpression current_expr, char prev_op, int goal,
        std::vector<TExpression> &exprs) {
    if (left_it == right_it) {
        // prev_op == '+' необходимо, чтобы избежать дублирования
        if ( goal == 0 && prev_op == '+') {
            exprs.push_back(current_expr);
            // Удаляем лишний "+" в начале
            exprs.back().operators.pop_front();
        }
        return;
    }
    int lhs = 0;
    int lhs_mult = (prev_op == '+') ? +1 : -1;
    while ( left_it != right_it ) {
        lhs = lhs * 10 + (*left_it);
        int new_goal = goal - (lhs_mult * lhs);
        ++left_it;
        PushPart(current_expr, prev_op, lhs);
        GetExpressions(left_it, right_it, current_expr, '+', new_goal, exprs);
        GetExpressions(left_it, right_it, current_expr, '-', new_goal, exprs);
        PopPart(current_expr);
    }
}

std::vector<TExpression> Solve(const std::vector<int> &nums, int goal = 200) {
    std::vector<TExpression> exprs;
    TExpression tmp_expr;
    GetExpressions(nums.begin(), nums.end(), tmp_expr, '+', goal, exprs);
    return exprs;
}

std::ostream& operator << (std::ostream &os, const TExpression &expr) {
    auto operands_it = expr.operands.begin();
    auto operators_it = expr.operators.begin();
    bool first = true;
    while ( operands_it != expr.operands.end()) {
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
    auto operands_it = expr.operands.begin();
    auto operators_it = expr.operators.begin();
    int result = 0;
    bool first = true;
    while( operands_it != expr.operands.end() ) {
        if (first) {
            result = *operands_it;
            ++operands_it;
            first = false;
            continue;
        }
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
        int expected = 4;
        std::vector <int> nums = {3, 2, 1, 0};
        if ( Evaluate(Solve(nums, expected)[0]) != expected ) {
            std::cerr << Solve(nums, expected).size() << '\n';
            std::cerr << "TestSolve failed!";
            std::exit(1);
        }
    }
}


int main() {
    TestSolve();
    
    std::vector <int> nums = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
    std::vector <TExpression> exprs = Solve(nums, 200);
    for (const auto &expr: exprs) {
        std::cout << expr << std::endl;
    }
}
