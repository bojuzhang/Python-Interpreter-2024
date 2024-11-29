#include "Evalvisitor.h"
#include "Scope.h"
#include "int2048.h"
#include "operator.h"
#include <any>
#include <cassert>
#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

Scope scope;

void varBack(std::any &p) {
  if (p.type() != typeid(std::string)) {
    return ;
  }
  auto varname = std::any_cast<std::string>(p);
  if (scope.varFind(varname)) {
    p = scope.varQuery(varname);
  }
} 

std::any EvalVisitor::visitAtom(Python3Parser::AtomContext *ctx) {
  // // std::cerr << "visitatom\n";
  if (ctx->NAME()) {
    auto p = ctx->NAME()->getText();
    return p;
  } else if (ctx->NUMBER()) {
    auto s = ctx->NUMBER()->getText();
    if (s.find('.') != s.npos) {
      return GetFlout(s);
    } else {
      return GetInt(s);
    }
  } else if (ctx->NONE()) {
    return std::any();
  } else if (ctx->TRUE()) {
    return true;
  } else if (ctx->FALSE()) {
    return false;
  } else if (!ctx->STRING().empty()) {
    // TODO
    return ctx->STRING();
  } else if (ctx->test()) {
    return visitTest(ctx->test());
  } else {
    return nullptr;
  }
}

std::any EvalVisitor::visitFactor(Python3Parser::FactorContext *ctx) {
  std::any res;
  if (ctx->factor()) {
    res = visitFactor(ctx->factor());
  } else {
    res = visitAtom_expr(ctx->atom_expr());
  }
  if (ctx->MINUS()) {
    return -res;
  } else {
    return res;
  }
}

std::any EvalVisitor::visitTerm(Python3Parser::TermContext *ctx) {
  auto factorarray = ctx->factor();
  auto res = visit(factorarray[0]);
  if (factorarray.size() == 1u) {
    return res;
  }
  varBack(res);
  auto muldivmod_oparray = ctx->muldivmod_op();
  for (size_t i = 1; i < factorarray.size(); i++) {
    auto x = visit(factorarray[i]);
    varBack(x);
    auto op = std::any_cast<std::string>(visit(muldivmod_oparray[i - 1]));
    if (op == "*") {
      res *= x;
    } else if (op == "/") {
      res /= x;
    } else if (op == "%") {
      res %= x;
    } else if (op == "//") {
      ForceDivEqual(res, x);
    }
  }
  return res;
}

std::any EvalVisitor::visitArith_expr(Python3Parser::Arith_exprContext *ctx) {
  auto termarray = ctx->term();
  auto res = visit(termarray[0]);
  if (termarray.size() == 1u) {
    // std::cerr << "res_tp: " << res.type().name() << "\n";
    if (res.type() == typeid(sjtu::int2048)) {
      std::cerr << "0ans:::" << std::any_cast<sjtu::int2048>(res) << "\n";
    }
    return res;
  }
  varBack(res);
  auto addorsub_oparray = ctx->addorsub_op();
  for (size_t i = 1; i < termarray.size(); i++) {
    auto x = visit(termarray[i]);
    varBack(x);
    auto p = visit(addorsub_oparray[i - 1]);
    // std::cerr << (p.type() == typeid(char*)) << "\n";
    auto op = std::any_cast<std::string>(visit(addorsub_oparray[i - 1]));
    if (op == "+") {
      res += x;
    } else if (op == "-") {
      res -= x;
    }
  }
  std::cerr << "1ans:::" << std::any_cast<sjtu::int2048>(res) << "\n";
  return res;
}

std::any EvalVisitor::visitFile_input(Python3Parser::File_inputContext *ctx) {
  return visitChildren(ctx);
}

std::any EvalVisitor::visitFuncdef(Python3Parser::FuncdefContext *ctx) {
  return visitChildren(ctx);
}

std::any EvalVisitor::visitParameters(Python3Parser::ParametersContext *ctx) {
  return visitChildren(ctx);
}

std::any EvalVisitor::visitTypedargslist(Python3Parser::TypedargslistContext *ctx) {
  return visitChildren(ctx);
}

std::any EvalVisitor::visitTfpdef(Python3Parser::TfpdefContext *ctx) {
  return visitChildren(ctx);
}

std::any EvalVisitor::visitStmt(Python3Parser::StmtContext *ctx) {
  return visitChildren(ctx);
}

std::any EvalVisitor::visitSimple_stmt(Python3Parser::Simple_stmtContext *ctx) {
  return visitChildren(ctx);
}

std::any EvalVisitor::visitSmall_stmt(Python3Parser::Small_stmtContext *ctx) {
  return visitChildren(ctx);
}

std::any EvalVisitor::visitExpr_stmt(Python3Parser::Expr_stmtContext *ctx) {
  // std::cerr << "visitexpr_stmt\n";
  auto testlistarray = ctx->testlist();
  // std::cerr << "arraysize: " << testlistarray.size() << "\n";
  if (testlistarray.size() == 1u) {
    visit(testlistarray[0]);
    return std::any();
  }
  if (!ctx->augassign()) {
    std::vector<std::any> leftarray, rightarray;
    rightarray = std::any_cast<std::vector<std::any>>(visit(*(testlistarray.rbegin())));
    for (int i = static_cast<int>(testlistarray.size()) - 2; i >= 0; i--) {
      leftarray = std::any_cast<std::vector<std::any>>(visit(testlistarray[i]));
      for (size_t j = 0; j < rightarray.size(); j++) {
        auto p = rightarray[j];
        varBack(p);
        // std::cerr << "righttp: " << p.type().name() << "\n" << leftarray[j].type().name() << "\n";
        // std::cerr << i << " " << j << " " << rightarray.size() << "\n";
        scope.varSet(std::any_cast<std::string>(leftarray[j]), p);
      }
      // std::cerr << "end loop j\n";
      rightarray = leftarray;
      // std::cerr << "end loop j2\n";
    }
  } else {
    // TODO augassign
  }
  // std::cerr << "end expr_stmt\n";
  return std::any();
}

std::any EvalVisitor::visitAugassign(Python3Parser::AugassignContext *ctx) {
  return visitChildren(ctx);
}

std::any EvalVisitor::visitFlow_stmt(Python3Parser::Flow_stmtContext *ctx) {
  return visitChildren(ctx);
}

std::any EvalVisitor::visitBreak_stmt(Python3Parser::Break_stmtContext *ctx) {
  return visitChildren(ctx);
}

std::any EvalVisitor::visitContinue_stmt(Python3Parser::Continue_stmtContext *ctx) {
  return visitChildren(ctx);
}

std::any EvalVisitor::visitReturn_stmt(Python3Parser::Return_stmtContext *ctx) {
  return visitChildren(ctx);
}

std::any EvalVisitor::visitCompound_stmt(Python3Parser::Compound_stmtContext *ctx) {
  return visitChildren(ctx);
}

std::any EvalVisitor::visitIf_stmt(Python3Parser::If_stmtContext *ctx) {
  return visitChildren(ctx);
}

std::any EvalVisitor::visitWhile_stmt(Python3Parser::While_stmtContext *ctx) {
  return visitChildren(ctx);
}

std::any EvalVisitor::visitSuite(Python3Parser::SuiteContext *ctx) {
  return visitChildren(ctx);
}

std::any EvalVisitor::visitTest(Python3Parser::TestContext *ctx) {
  auto p = visit(ctx->or_test());
  // std::cerr << "test_tp: " << p.type().name() << "\n";
  return p;
}

std::any EvalVisitor::visitOr_test(Python3Parser::Or_testContext *ctx) {
  auto andtestarray = ctx->and_test();
  auto res = visit(andtestarray[0]);
  if (andtestarray.size() == 1u) {
    return res;
  }
  if (GetBool(res)) {
    return true;
  }
  for (size_t i = 1; i < andtestarray.size(); i++) {
    auto p = visit(andtestarray[i]);
    varBack(p);
    if (GetBool(p)) {
      return true;
    }
  }
  return false;
}

std::any EvalVisitor::visitAnd_test(Python3Parser::And_testContext *ctx) {
  auto nottestarray = ctx->not_test();
  auto res = visit(nottestarray[0]);
  if (nottestarray.size() == 1u) {
    return res;
  }
  if (!GetBool(res)) {
    return false;
  }
  for (size_t i = 1; i < nottestarray.size(); i++) {
    auto p = visit(nottestarray[i]);
    varBack(p);
    if (!GetBool(p)) {
      return false;
    }
  }
  return true;
}

std::any EvalVisitor::visitNot_test(Python3Parser::Not_testContext *ctx) {
  if (ctx->NOT()) {
    auto p = visit(ctx->not_test());
    varBack(p);
    return !GetBool(p);
  } else {
    return visit(ctx->comparison());
  }
}

std::any EvalVisitor::visitComparison(Python3Parser::ComparisonContext *ctx) {
  auto arritharray = ctx->arith_expr();
  auto res = visit(arritharray[0]);
  if (arritharray.size() == 1u) {
    return res;
  }

  //TODO comparison

  return res;
}

std::any EvalVisitor::visitComp_op(Python3Parser::Comp_opContext *ctx) {
  return visitChildren(ctx);
}

std::any EvalVisitor::visitAddorsub_op(Python3Parser::Addorsub_opContext *ctx) {
  if (ctx->ADD()) {
    return std::string("+");
  } else if (ctx->MINUS()) {
    return std::string("-");
  } else {
    assert("???" == 0);
  }
}

std::any EvalVisitor::visitMuldivmod_op(Python3Parser::Muldivmod_opContext *ctx) {
  if (ctx->STAR()) {
    return std::string("*");
  } else if (ctx->DIV()) {
    return std::string("/");
  } else if (ctx->IDIV()) {
    return std::string("//");
  } else if (ctx->MOD()) {
    return std::string("%");
  } else {
    // throw unvalid muldivmod_op
    assert("unvalid muldivmod_op" == 0);
  }
}

std::any EvalVisitor::visitAtom_expr(Python3Parser::Atom_exprContext *ctx) {
  if (ctx->trailer()) {
    // TODO function
    return visitChildren(ctx);
  } else {
    return visit(ctx->atom());
  }
}

std::any EvalVisitor::visitTrailer(Python3Parser::TrailerContext *ctx) {
  return visitChildren(ctx);
}

std::any EvalVisitor::visitFormat_string(Python3Parser::Format_stringContext *ctx) {
  return visitChildren(ctx);
}

std::any EvalVisitor::visitTestlist(Python3Parser::TestlistContext *ctx) {
  // std::cerr << "visittestlist\n";
  std::vector<std::any> ans;
  auto testarray = ctx->test();
  // std::cerr << testarray.size() << "\n";
  for (const auto &p : testarray) {
    auto val = visit(p);
    // std::cerr << "testlisttp: " << val.type().name() << "\n";
    ans.push_back(val);
  }
  return ans;
}

std::any EvalVisitor::visitArglist(Python3Parser::ArglistContext *ctx) {
  return visitChildren(ctx);
}

std::any EvalVisitor::visitArgument(Python3Parser::ArgumentContext *ctx) {
  return visitChildren(ctx);
}