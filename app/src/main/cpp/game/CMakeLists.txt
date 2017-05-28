add_subdirectory(geometry)
set(GAME_SRC 	${CMAKE_CURRENT_SOURCE_DIR}/Game.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/Renderer.cpp
				${CMAKE_CURRENT_SOURCE_DIR}/PhysicsSystem.cpp
				${GAME_GEOMETRY_SRC}
				PARENT_SCOPE
    )