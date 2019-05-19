target_sources(
	${TARGET_NAME}
PRIVATE
	assertions.hpp
	debug.cpp
	debug.hpp
	file.cpp
	file.hpp
	LogChannel.hpp
	main.cpp
	MeshData.hpp
	shader.cpp
	shader.hpp
	tmpl.hpp
)

add_subdirectory(maths)
