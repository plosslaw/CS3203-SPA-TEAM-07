#include "MockAST.h"

/*
The basic AST used for testing here is equivalent to the following code:
procedure main {
1.    read a;
2.    print b;
3.    while (x < y) {
4.      x = x - 1;
..    }

5.    if (z + y >= x) then {
6.        w = y * 2;
      } else {
7.        x = 42;
8.        y = 2;
9.        while (v != 2) {
10.           v = v - 1;
          }
      }
11.   print x;
}
*/
TNode getMockAST() {
    TNode var_v = TNode("v", VARIABLE);
    TNode var_w = TNode("w", VARIABLE);
    TNode var_x = TNode("x", VARIABLE);
    TNode var_y = TNode("y", VARIABLE);
    TNode var_z = TNode("z", VARIABLE);
    TNode var_a = TNode("a", VARIABLE);
    TNode var_b = TNode("b", VARIABLE);

    TNode const_1 = TNode("1", CONSTANT);
    TNode const_2 = TNode("2", CONSTANT);
    TNode const_42 = TNode("42", CONSTANT);

    TNode stmt_3_cond_expr = TNode("<", OPERATOR);
    TNode stmt_4_rhs_expr = TNode("-", OPERATOR);
    TNode stmt_5_cond_expr = TNode(">=", OPERATOR);
    TNode stmt_5_lhs_expr = TNode("+", OPERATOR);
    TNode stmt_6_rhs_expr = TNode("*", OPERATOR);
    TNode stmt_9_cond_expr = TNode("!=", OPERATOR);
    TNode stmt_10_rhs_expr = TNode("-", OPERATOR);

    stmt_3_cond_expr.addChild(var_x);
    stmt_3_cond_expr.addChild(var_y);

    stmt_4_rhs_expr.addChild(var_x);
    stmt_4_rhs_expr.addChild(const_1);

    stmt_5_lhs_expr.addChild(var_z);
    stmt_5_lhs_expr.addChild(var_y);

    stmt_5_cond_expr.addChild(stmt_5_lhs_expr);
    stmt_5_cond_expr.addChild(var_x);

    stmt_6_rhs_expr.addChild(var_y);
    stmt_6_rhs_expr.addChild(const_2);

    stmt_9_cond_expr.addChild(var_v);
    stmt_9_cond_expr.addChild(const_2);

    stmt_10_rhs_expr.addChild(var_v);
    stmt_10_rhs_expr.addChild(const_1);

    TNode stmt_1 = TNode(1, "", READ);
    TNode stmt_2 = TNode(2, "", PRINT);
    TNode stmt_3 = TNode(3, "", WHILE);
    TNode stmt_4 = TNode(4, "", ASSIGN);
    TNode stmt_5 = TNode(5, "", IF);
    TNode stmt_6 = TNode(6, "", ASSIGN);
    TNode stmt_7 = TNode(7, "", ASSIGN);
    TNode stmt_8 = TNode(8, "", ASSIGN);
    TNode stmt_9 = TNode(9, "", WHILE);
    TNode stmt_10 = TNode(10, "", ASSIGN);
    TNode stmt_11 = TNode(11, "", PRINT);

    stmt_1.addChild(var_a);
    stmt_2.addChild(var_b);

    stmt_4.addChild(var_x);
    stmt_4.addChild(stmt_4_rhs_expr);

    stmt_6.addChild(var_w);
    stmt_6.addChild(stmt_6_rhs_expr);

    stmt_7.addChild(var_x);
    stmt_7.addChild(const_42);

    stmt_8.addChild(var_y);
    stmt_8.addChild(const_2);

    stmt_10.addChild(var_v);
    stmt_10.addChild(stmt_10_rhs_expr);

    TNode while_stmt_list = TNode("", STATEMENTLIST);
    while_stmt_list.addChild(stmt_4);

    TNode while_stmt_list_2 = TNode("", STATEMENTLIST);
    while_stmt_list_2.addChild(stmt_10);

    TNode then_stmt_list = TNode("", STATEMENTLIST);
    then_stmt_list.addChild(stmt_6);

    stmt_9.addChild(stmt_9_cond_expr);
    stmt_9.addChild(while_stmt_list_2);

    TNode else_stmt_list = TNode("", STATEMENTLIST);
    else_stmt_list.addChild(stmt_7);
    else_stmt_list.addChild(stmt_8);
    else_stmt_list.addChild(stmt_9);

    stmt_3.addChild(stmt_3_cond_expr);
    stmt_3.addChild(while_stmt_list);

    stmt_5.addChild(stmt_5_cond_expr);
    stmt_5.addChild(then_stmt_list);
    stmt_5.addChild(else_stmt_list);


    stmt_11.addChild(var_x);

    TNode main_stmt_list = TNode("", STATEMENTLIST);
    main_stmt_list.addChild(stmt_1);
    main_stmt_list.addChild(stmt_2);
    main_stmt_list.addChild(stmt_3);
    main_stmt_list.addChild(stmt_5);
    main_stmt_list.addChild(stmt_9);
    main_stmt_list.addChild(stmt_11);

    TNode proc = TNode("main", PROCEDURE);
    proc.addChild(main_stmt_list);

    TNode prog = TNode("", PROGRAM);
    prog.addChild(proc);

    return prog;
}