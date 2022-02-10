#include <iostream>

#pragma once

namespace ra
{
    class not_implemented_exception : public std::logic_error
    {
    public:
        not_implemented_exception(
            std::string function = __builtin_FUNCTION()) : std::logic_error("`" + function + "` not implemented!"){};
    };

    class invalid_transaction_error : public std::logic_error
    {
    public:
        invalid_transaction_error(std::string transaction) : std::logic_error("Invalid Transaction!" + transaction){};
    };

    class public_key_mismatch : public std::invalid_argument
    {
    public:
        public_key_mismatch(
            std::string addr, std::string pk) : std::invalid_argument("from_addr: '" + addr + "' do not match public_key: '" + pk + "'"){};
    };

    class resign_transaction_exception : public std::logic_error
    {
    public:
        resign_transaction_exception(std::string transaction) : std::logic_error("Resigning Transaction!" + transaction){};
    };
}
