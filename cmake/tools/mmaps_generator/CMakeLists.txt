PROJECT(mmaps_generator CXX)
SET( prefix ${ROOT_PATH}/src/tools/mmaps_generator)
SET( sources
    generator.cpp
	IntermediateValues.cpp
	MapBuilder.cpp
	TerrainBuilder.cpp
	VMapExtensions.cpp
)

SET( headers
   IntermediateValues.h
   MoveMapSharedDefines.h
   MapBuilder.h
   TerrainBuilder.h
   CompilerDefs.h
   Define.h
)

foreach(src IN ITEMS ${sources} )
  SET( SRCS ${SRCS} ${prefix}/${src} )
endforeach(src)
foreach(src IN ITEMS ${headers} )
  SET( HEADERS ${HEADERS} ${prefix}/${src} )
endforeach(src)

include_directories(
  ${DEPENDENCY_INCLUDE_ROOT}/libmpq_new
  ${DEPENDENCY_INCLUDE_ROOT}/dbcfile 
  ${DEPENDENCY_INCLUDE_ROOT}/loadlib
  ${DEPENDENCY_INCLUDE_ROOT}/vmaplib
  ${DEPENDENCY_INCLUDE_ROOT}/recast
  ${DEPENDENCY_INCLUDE_ROOT}/detour
  ${DEPENDENCY_INCLUDE_ROOT}/g3dlite
  )
link_directories( ${DEPENDENCY_LIBS} ${EXTRA_LIBS_PATH} )

ADD_EXECUTABLE( ${PROJECT_NAME} ${SRCS}  ${HEADERS} )

target_link_libraries( ${PROJECT_NAME} shared  g3dlite vmaplib detour recast ${PCRE_LIBRARIES} )

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${FROSTWINGCORE_TOOLS_PATH} )

