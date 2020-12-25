aux_source_directory(./ utilities_src)
include_directories(./)

add_library(utilities STATIC ${utilities_src})
target_link_libraries(utilities pthread)