# Use it like:
# CollectSourceFiles(
#   ${CMAKE_CURRENT_SOURCE_DIR}
#   COMMON_PRIVATE_SOURCES
#   # Exclude
#   ${CMAKE_CURRENT_SOURCE_DIR}/PrecompiledHeaders
#   ${CMAKE_CURRENT_SOURCE_DIR}/Platform)
#
function(CollectSourceFiles current_dir variable)
	list(FIND ARGN "${current_dir}" IS_EXCLUDED)
	if(IS_EXCLUDED EQUAL -1)
		file(GLOB COLLECTED_SOURCES
		${current_dir}/*.c
		${current_dir}/*.cc
		${current_dir}/*.cpp
		${current_dir}/*.inl
		${current_dir}/*.def
		${current_dir}/*.h
		${current_dir}/*.hh
		${current_dir}/*.hpp)
		list(APPEND ${variable} ${COLLECTED_SOURCES})

		file(GLOB SUB_DIRECTORIES ${current_dir}/*)
		foreach(SUB_DIRECTORY ${SUB_DIRECTORIES})
			if(IS_DIRECTORY ${SUB_DIRECTORY})
				CollectSourceFiles("${SUB_DIRECTORY}" "${variable}" "${ARGN}")
			endif()
		endforeach()
		set(${variable} ${${variable}} PARENT_SCOPE)
	endif()
endfunction()

# Collects all subdirectoroies into the given variable,
# which is useful to include all subdirectories.
# Ignores full qualified directories listed in the variadic arguments.
#
# Use it like:
# CollectIncludeDirectories(
#   ${CMAKE_CURRENT_SOURCE_DIR}
#   COMMON_PUBLIC_INCLUDES
#   # Exclude
#   ${CMAKE_CURRENT_SOURCE_DIR}/PrecompiledHeaders
#   ${CMAKE_CURRENT_SOURCE_DIR}/Platform)
#
function(CollectIncludeDirectories current_dir variable)
	list(FIND ARGN "${current_dir}" IS_EXCLUDED)
	if(IS_EXCLUDED EQUAL -1)
		list(APPEND ${variable} ${current_dir})
		file(GLOB SUB_DIRECTORIES ${current_dir}/*)
		foreach(SUB_DIRECTORY ${SUB_DIRECTORIES})
			if(IS_DIRECTORY ${SUB_DIRECTORY})
				CollectIncludeDirectories("${SUB_DIRECTORY}" "${variable}" "${ARGN}")
			endif()
		endforeach()
		set(${variable} ${${variable}} PARENT_SCOPE)
	endif()
endfunction()


function(GroupSources dir)
	# Include all header and c files
	file(GLOB_RECURSE elements RELATIVE ${dir} *.h *.hpp *.c *.cpp *.cc)

	foreach(element ${elements})
		# Extract filename and directory
		get_filename_component(element_name ${element} NAME)
		get_filename_component(element_dir ${element} DIRECTORY)

		if(NOT ${element_dir} STREQUAL "")
			# If the file is in a subdirectory use it as source group.
			# Use the full hierarchical structure to build source_groups.
			string(REPLACE "/" "\\" group_name ${element_dir})
			source_group("${group_name}" FILES ${dir}/${element})
		else()
			# If the file is in the root directory, place it in the root source_group.
			source_group("\\" FILES ${dir}/${element})
		endif()
	endforeach()
endfunction()
