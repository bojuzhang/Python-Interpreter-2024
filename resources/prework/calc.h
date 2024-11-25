// 你需要提交这份代码
#pragma once

#include "visitor.h"
#include <any>

struct calculator : visitor {
    /// TODO: 完成所有需求
    ~calculator() override = default;
    virtual std::any visit_num(num_node *ctx) override {
        return ctx->number;
    };
    virtual std::any visit_add(add_node *ctx) override {
        auto a = ctx->lnode->accept(this), b = ctx->rnode->accept(this);
        auto along = std::any_cast<long long>(&a), blong = std::any_cast<long long>(&b);
        if (along && blong) {
            return *along + *blong;
        }
        auto adouble = std::any_cast<double>(&a), bdouble = std::any_cast<double>(&b);
        if (adouble && bdouble) {
            return *adouble + *bdouble;
        }
    };
    virtual std::any visit_sub(sub_node *ctx) override {
        auto a = ctx->lnode->accept(this), b = ctx->rnode->accept(this);
        auto along = std::any_cast<long long>(&a), blong = std::any_cast<long long>(&b);
        if (along && blong) {
            return *along - *blong;
        }
        auto adouble = std::any_cast<double>(&a), bdouble = std::any_cast<double>(&b);
        if (adouble && bdouble) {
            return *adouble - *bdouble;
        }
    };
    virtual std::any visit_mul(mul_node *ctx) override {
        auto a = ctx->lnode->accept(this), b = ctx->rnode->accept(this);
        auto along = std::any_cast<long long>(&a), blong = std::any_cast<long long>(&b);
        if (along && blong) {
            return *along * *blong;
        }
        auto adouble = std::any_cast<double>(&a), bdouble = std::any_cast<double>(&b);
        if (adouble && bdouble) {
            return *adouble * *bdouble;
        }
    };
    virtual std::any visit_div(div_node *ctx) override {
        auto a = ctx->lnode->accept(this), b = ctx->rnode->accept(this);
        auto along = std::any_cast<long long>(&a), blong = std::any_cast<long long>(&b);
        if (along && blong) {
            return *along / *blong;
        }
        auto adouble = std::any_cast<double>(&a), bdouble = std::any_cast<double>(&b);
        if (adouble && bdouble) {
            return *adouble / *bdouble;
        }
    };
};
