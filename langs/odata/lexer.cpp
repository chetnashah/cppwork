enum class TokenType
{
    IDENTIFIER,  // variable names, function names
    NUMBER,      // numeric literals
    STRING,      // string literals
    OPERATOR,    // +, -, *, /, etc.
    KEYWORD,     // if, while, for, etc.
    WHITESPACE,  // spaces, tabs, newlines
    COMMENT,     // single-line and multi-line comments
    END_OF_FILE, // marks end of input
    PUNCTUATION  // , . : etc.
};
