FILE (GLOB_RECURSE project_SRCS *.cpp *.cxx *.cc *.C *.c *.h *.hpp)
SET (project_LIBS  ${SFML_LIBRARIES} ${MKL_LIBRARIES})
SET (project_BIN ${PROJECT_NAME})

ADD_EXECUTABLE(${project_BIN} ${project_SRCS})
TARGET_LINK_LIBRARIES(${project_BIN} ${project_LIBS})

SET_TARGET_PROPERTIES(${project_BIN} PROPERTIES VERSION "${APPLICATION_VERSION_MAJOR}.${APPLICATION_VERSION_MINOR}" OUTPUT_NAME ${project_BIN} CLEAN_DIRECT_OUTPUT 1)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp -xHOST -O3 -ipo -no-prec-div -fp-model fast=2 -std=c++14 -static-intel -parallel")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp") 
TARGET_COMPILE_FEATURES(${PROJECT_NAME} PRIVATE cxx_constexpr)

INSTALL(TARGETS ${project_BIN} DESTINATION bin)
