#pragma once

#include <functional>

namespace CLI
{
    using TokenID = unsigned int;
    enum class Tokens : TokenID;
    using ArgumentTokens = std::vector<Tokens>;
    using ArgumentViews = std::vector<std::string_view>;
} // namespace CLI


namespace Commands
{
    enum class ReturnCodes : int
    {
        SUCCESS = 0,
        HELP = 1,
        ERROR = 2,
    };

    using CommandFuncSigntature = const std::function<ReturnCodes()>;

    CommandFuncSigntature command_from_cli_args(
            const CLI::ArgumentTokens& arg_tokens,
            const CLI::ArgumentViews& arg_views);
} // namespace Commands
