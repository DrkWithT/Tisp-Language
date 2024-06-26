#ifndef EXPRS_HPP
#define EXPRS_HPP

#include <memory>
#include <string>
#include <any>
#include <variant>
#include <vector>
#include "ast/exprbase.hpp"

namespace tisp::ast
{
    enum class OpType
    {
        invoke,
        access,
        plus,
        minus,
        times,
        slash,
        equality,
        inequality,
        greater,
        atmost,
        lesser,
        atleast,
        logic_and,
        logic_or
    };

    enum class DataType
    {
        unknown,
        boolean,
        integer,
        ndouble,
        string,
        sequence,
        nil
    };

    struct Nil {};
    struct Sequence
    {
        std::vector<std::any> items;
        DataType homogen_type;

        Sequence();
        Sequence(std::vector<std::any> item_args, DataType type);
    };

    template <typename Nt>
    constexpr DataType to_lang_type_v = DataType::unknown;

    template <>
    constexpr DataType to_lang_type_v<Nil> = DataType::nil;

    template <>
    constexpr DataType to_lang_type_v<bool> = DataType::boolean;

    template <>
    constexpr DataType to_lang_type_v<int> = DataType::integer;

    template <>
    constexpr DataType to_lang_type_v<double> = DataType::ndouble;

    template <>
    constexpr DataType to_lang_type_v<std::string> = DataType::string;

    template <>
    constexpr DataType to_lang_type_v<Sequence> = DataType::sequence;

    class Literal : public IExpression
    {
    private:
        std::variant<Nil, bool, int, double, std::string, std::any> value;
        DataType data_type;

    public:
        Literal();
        Literal(bool b);
        Literal(int i);
        Literal(double dbl);
        Literal(std::string str);
        Literal(Sequence seq);

        [[nodiscard]] constexpr DataType getDataType() const noexcept
        {
            return data_type;
        }

        template <typename Nt>
        constexpr Nt toNativeType() const noexcept
        {
            constexpr auto vtype = to_lang_type_v<Nt>;

            if constexpr (vtype == DataType::unknown || vtype != getDataType())
                return Nt {};

            constexpr auto variant_idx = static_cast<int>(vtype);

            return std::get<variant_idx>(value);
        }

        [[nodiscard]] std::any acceptVisitor(IExprVisitor<std::any>& visitor) const override;
    };

    class Unary : public IExpression
    {
    private:
        std::unique_ptr<IExpression> inner;
        OpType op;

    public:
        Unary() = delete;
        Unary(std::unique_ptr<IExpression> arg, OpType op_arg);

        [[nodiscard]] constexpr OpType getOpType() const noexcept
        {
            return op;
        }

        [[nodiscard]] std::any acceptVisitor(IExprVisitor<std::any>& visitor) const override;
    };

    class Binary : public IExpression
    {
    private:
        std::unique_ptr<IExpression> left;
        std::unique_ptr<IExpression> right;
        OpType op;

    public:
        Binary() = delete;
        Binary(std::unique_ptr<IExpression> lhs, std::unique_ptr<IExpression> rhs, OpType op_arg);

        [[nodiscard]] constexpr OpType getOpType() const noexcept
        {
            return op;
        }

        [[nodiscard]] std::any acceptVisitor(IExprVisitor<std::any>& visitor) const noexcept;
    };
}

#endif