# include "nsh.h"

void parse_error(const char *token)
{
    const char *prefix;
    const char *suffix;

    prefix = "syntax error near unexpected token `";
    suffix = "'\n";
    write(2, prefix, ft_strlen(prefix));
    write(2, token, ft_strlen(token));
    write(2, suffix, ft_strlen(suffix));
}