add_subdirectory(gameplay)
add_subdirectory(input)

set(CORE_SRC ${CMAKE_CURRENT_SOURCE_DIR}/AndroidGame.cpp 
			 ${CMAKE_CURRENT_SOURCE_DIR}/Geometry.cpp
			 ${CMAKE_CURRENT_SOURCE_DIR}/Shader.cpp
			 ${CORE_GAMEPLAY_SRC}
			 ${CORE_INPUT_SRC}
			 PARENT_SCOPE
    )