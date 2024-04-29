add_executable(tipsi main.cpp)

add_subdirectory(frontend) # parsing
add_subdirectory(ast) # AST as IR
add_subdirectory(utils) # AST printer
# add_subdirectory(backend) # codegen
# add_subdirectory(runtime) # VM

target_link_libraries(tipsi PRIVATE frontend PRIVATE ast PRIVATE utils)
