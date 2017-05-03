add_subdirectory(geometry)
set(GAME_SRC 	${CMAKE_CURRENT_SOURCE_DIR}/Game.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/Renderer.cpp
				${GAME_GEOMETRY_SRC}
				PARENT_SCOPE
    )