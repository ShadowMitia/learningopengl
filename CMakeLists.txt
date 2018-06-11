cmake_minimum_required(VERSION 3.8)

# References:
# https://pabloariasal.github.io/2018/02/19/its-time-to-do-cmake-right/
# https://github.com/Polytonic/Glitter/blob/master/CMakeLists.txt
# https://github.com/lefticus/cpp_starter_project
# https://github.com/JoeyDeVries/LearnOpenGL/blob/master/CMakeLists.txt


project(learnopengl LANGUAGES C CXX)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()


if(CMAKE_COMPILER_IS_GNUCC)
  option(ENABLE_COVERAGE "Enable coverage reporting for gcc/clang" FALSE)

  if(ENABLE_COVERAGE)
    add_compile_options(--coverage -O0)
  endif()
endif()

# Link this 'library' to use the following warnings
add_library(project_warnings INTERFACE)

if(MSVC)
  target_compile_options(project_warnings INTERFACE /W4)
else()
  target_compile_options(project_warnings
                         INTERFACE
                         -Wall
                         -Wextra # reasonable and standard
                         -Wshadow # warn the user if a variable declaration shadows one from a
                                  # parent context
                         -Wnon-virtual-dtor # warn the user if a class with virtual functions has a
                                            # non-virtual destructor. This helps catch hard to
                                            # track down memory errors
                         -Wold-style-cast # warn for c-style casts
                         -Wcast-align # warn for potential performance problem casts
                         -Wunused # warn on anything being unused
                         -Woverloaded-virtual # warn if you overload (not override) a virtual
                                              # function
                         -Wpedantic # warn if non-standard C++ is used
                         -Wconversion # warn on type conversions that may lose data
                         -Wsign-conversion # warn on sign conversions
                         -Wmisleading-indentation # warn if identation implies blocks where blocks
                                                  # do not exist
                         -Wduplicated-cond # warn if if / else chain has duplicated conditions
                         -Wduplicated-branches # warn if if / else branches have duplicated code
                         -Wlogical-op # warn about logical operations being used where bitwise were
                                      # probably wanted
                         -Wnull-dereference # warn if a null dereference is detected
                         -Wuseless-cast # warn if you perform a cast to the same type
                         -Wdouble-promotion # warn if float is implicit promoted to double
                         -Wformat=2 # warn on security issues around functions that format output
                                    # (ie printf)
                         )
endif()

# data dependencies
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data/container.jpg DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin/ressources)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data/awesomeface.png DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/bin/ressources)

# Find dependencies

# Find GLFW3
find_package(glfw3 REQUIRED)

# add GLAD as a library
# source: http://glad.dav1d.de/
add_library(GLAD SHARED libs/GLAD/glad.c)
target_include_directories(GLAD
  PUBLIC
  $<INSTALL_INTERFACE:include>
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/libs/GLAD>
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/libs/GLAD/)

# add stb_image
# source: https://github.com/nothings/stb
add_library(stb_image SHARED libs/stb_image/stb_image.c)
target_include_directories(stb_image
   PUBLIC
   $<INSTALL_INTERFACE:include>
   $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/libs/stb_image>
   PRIVATE
     ${CMAKE_CURRENT_SOURCE_DIR}/libs/stb_image)

# Find GLM
# pris depuis conan
find_package(GLM REQUIRED)

# Build executable
add_executable(sandbox src/main.cpp)
target_compile_features(sandbox PRIVATE cxx_std_14)
target_link_libraries(sandbox PRIVATE project_warnings --coverage)

target_link_libraries(sandbox PRIVATE GLAD)
target_link_libraries(sandbox PRIVATE glfw)
target_link_libraries(sandbox PRIVATE stb_image)
target_link_libraries(sandbox ${CONAN_LIBS})


#enable_testing()

# add_executable(tester tester.cpp)
# target_link_libraries(tester PRIVATE project_warnings --coverage)
# add_test(Tester tester)
