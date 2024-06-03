// maps of regular expression error codes to descriptive strings
// -------------------------------------------------------------
const std::map<std::regex_constants::error_type,std::string> regex_err_strings{
	{std::regex_constants::error_collate, "The expression contained an invalid collating element name."},
	{std::regex_constants::error_ctype, "The expression contained an invalid character class name."},
	{std::regex_constants::error_escape, "The expression contained an invalid escaped character, or a trailing escape."},
	{std::regex_constants::error_backref, "The expression contained an invalid back reference."},
	{std::regex_constants::error_brack, "The expression contained mismatched brackets ([ and ])."},
	{std::regex_constants::error_paren, "The expression contained mismatched parentheses (( and ))."},
	{std::regex_constants::error_brace, "The expression contained mismatched braces ({ and })."},
	{std::regex_constants::error_badbrace, "The expression contained an invalid range between braces ({ and })."},
	{std::regex_constants::error_range, "The expression contained an invalid character range."},
	{std::regex_constants::error_space, "There was insufficient memory to convert the expression into a finite state machine."},
	{std::regex_constants::error_badrepeat, "The expression contained a repeat specifier (one of *?+{) that was not preceded by a valid regular expression."},
	{std::regex_constants::error_complexity, "The complexity of an attempted match against a regular expression exceeded a pre-set level."},
	{std::regex_constants::error_stack, "There was insufficient memory to determine whether the regular expression could match the specified character sequence."}
};
