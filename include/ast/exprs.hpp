#ifndef EXPRS_HPP
#define EXPRS_HPP

#include <memory>
#include <string>
#include <any>
#include <variant>
#include <vector>
#include "ast/exprbase.hpp"
#include "frontend/token.hpp"

namespace tisp::ast
{
    using TispToken = tisp::frontend::Token;

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
        logic_or,
        nop
    };

    enum class DataType
    {
        unknown,
        boolean,
        integer,
        ndouble,
        string,
        sequence,
        nil,
        tbd       // variable identifier
    };

    struct FullDataType
    {
        DataType outer;
        DataType inner;
    };

    struct Nil {};
    struct Sequence
    {
        std::vector<std::unique_ptr<IExpression>> items;
        DataType homogen_type;

        Sequence();
        Sequence(std::vector<std::unique_ptr<IExpression>> item_args, DataType type);
    };

    template <typename Nt>
    static constexpr DataType to_lang_type_v = DataType::nil;

    template <>
    static constexpr DataType to_lang_type_v<Nil> = DataType::nil;

    template <>
    static constexpr DataType to_lang_type_v<bool> = DataType::boolean;

    template <>
    static constexpr DataType to_lang_type_v<int> = DataType::integer;

    template <>
    static constexpr DataType to_lang_type_v<double> = DataType::ndouble;

    template <>
    static constexpr DataType to_lang_type_v<std::string> = DataType::string;

    template <>
    static constexpr DataType to_lang_type_v<Sequence> = DataType::sequence;

    template <>
    static constexpr DataType to_lang_type_v<TispToken> = DataType::tbd;

    class Literal : public IExpression
    {
    private:
        std::variant<Nil, bool, int, double, std::string, Sequence, TispToken> value;
        FullDataType data_type;

    public:
        Literal();
        Literal(bool b);
        Literal(int i);
        Literal(double dbl);
        Literal(std::string str);
        Literal(Sequence seq);
        Literal(TispToken tok);

        [[nodiscard]] constexpr FullDataType getDataType() const noexcept
        {
            return data_type;
        }

        template <typename Nt>
        constexpr Nt toNativeType() const
        {
            constexpr auto vtype = to_lang_type_v<Nt>;

            if constexpr (vtype == DataType::unknown || vtype == DataType::nil || vtype != getDataType().outer)
                return Nt {};

            constexpr auto variant_idx = static_cast<int>(vtype);

            return std::get<variant_idx>(value);
        }

        [[maybe_unused]] std::any acceptVisitor(IExprVisitor<std::any>& visitor) const override;
    };

    class Unary : public IExpression
    {
    private:
        std::vector<std::unique_ptr<IExpression>> argv;
        std::string identifier;
        OpType op;

    public:
        Unary() = delete;
        Unary(std::vector<std::unique_ptr<IExpression>> argv_arg, std::string identifier_arg, OpType op_arg);

        [[nodiscard]] constexpr OpType getOpType() const noexcept
        {
            return op;
        }

        [[maybe_unused]] std::any acceptVisitor(IExprVisitor<std::any>& visitor) const override;
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

        [[maybe_unused]] std::any acceptVisitor(IExprVisitor<std::any>& visitor) const;
    };
}

#endif