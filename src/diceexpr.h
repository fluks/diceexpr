#ifndef DICEEXPR_H
    #define DICEEXPR_H

/** de_parse() return values on error.
 */
enum parse_error {
    DE_MEMORY = 1,
	DE_INVALID_CHARACTER,
	DE_SYNTAX_ERROR,
    DE_NROLLS,              // Number of rolls is not positive.
    DE_DICE,                // Number of sides in a dice is not positive.
    DE_IGNORE               // Number of ignores for a dice is too large.
};

/** Parse dice expression.
 * Caller must call srand() once before using this function.
 * @param expr Dice expression, can't be NULL.
 * @param value Used to store evaluated value.
 * @param rolled_expr Used to store dice expression after rolling dices.
 * @return Zero on success, enum parse_error otherwise.
 */
enum parse_error
de_parse(const char *expr, int *value, char **rolled_expression);

#endif
