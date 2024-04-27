# Grammar

### "New" Features
 - All typenames are uppercase to reduce ambiguity in parsing.
 - "$" operator is for invoking functions.
 - "@" operator is for accessing something in a sequence.
 - Negation operator prohibits additional args after the identifier!

### Rules: (expr, stmts)
```bnf
comment ::= "#" ... "#"

literal ::= Boolean | Integer | Double | String | Sequence | Nil | identifier
unary ::= ("$" | "@") "(" identifier (expr)* ")" | ("-")? identifier
factor ::= unary (("*" | "/") unary)*
term ::= factor (("+" | "-") factor)*
compare ::= term (("==" | "!=" | ">" | ">=" | "<" | "<=") term)
conditional ::= compare (("&&" | "||") compare)*
expr = conditional

typename = ("Boolean" | "Integer" | "Double" | "String" | "Seq" | "Nil" | adt) ( "(" typename ")" )?
adt = ... ; identifier with uppercase first letter!

variable ::= constant | mutable
constant ::= "const" identifier ":" typename expr
mutable ::= "var" identifier ":" typename expr
mutation ::= identifier "=" expr
defun ::= "defun" identifier "(" (param ("," param)? )* ")" "->" typename block
param ::= identifier ":" typename
block ::= "{" (inner)+ "}"
inner ::= variable | mutation | defun | match | while | return
match ::= "match" identifier "{" (case)+ ("default" block) "}"
case ::= "case" expr block
return ::= "return" expr
while ::= "while" expr block
outer ::= variable | defun | generic | import
generic ::= "generic" "(" (identifier)* ")" defun
substitution ::= identifier "(" (typename)* ")"
import ::= "use" identifier ("." identifier)
```