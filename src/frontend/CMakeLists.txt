add_library(frontend "")

# todo: add lexer.cpp, exprs.cpp, stmts.cpp, parser.cpp
target_sources(frontend PRIVATE token.cpp PRIVATE lexer.cpp)
