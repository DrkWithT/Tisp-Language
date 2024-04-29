add_library(frontend "")

target_sources(frontend PRIVATE token.cpp PRIVATE lexer.cpp PRIVATE info.cpp PRIVATE parser.cpp)
