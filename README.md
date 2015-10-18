[![Build Status](https://travis-ci.org/fluks/diceexpr.svg?branch=master)
](https://travis-ci.org/fluks/diceexpr)

# Description

A dice expression parser in C to use for rolling dices in RPGs etc.

Here's a grammar for a dice expression.

 ```
 s      ::= expr
 expr   ::= INTEGER | ('-'|'+') expr | expr '-' expr | expr '+' expr |
            [INTEGER] ('d'|'D') INTEGER ignore
 ignore ::= ('<' | '>' [INTEGER])*
 ```

Example expression:

 `3d6< + 3d4>2 + d2 - 1`

Roll d6 three times, discard smallest value, plus roll d4 three times,
discard two largest values, plus roll d2 once, minus 1.

Example program

 ```
 make
 make example
 LD_LIBRARY_PATH=lib example/example
 ```
