add_executable(run main.cpp driver.hpp bc_extension.hpp)


dune_symlink_to_source_files(FILES params.input cavity.msh nozzle.msh)

# target_compile_definitions(adaptive PRIVATE EXACT)
