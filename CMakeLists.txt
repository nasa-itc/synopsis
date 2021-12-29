cmake_minimum_required(VERSION 3.14)
project(synopsis VERSION 0.8.0 DESCRIPTION "Science Yield improvemeNt via Onboard Prioritization and Summary of Information System")

# GoogleTest requires at least C++11
set(CMAKE_CXX_STANDARD 11)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_library(synopsis SHARED
    src/synopsis.cpp
    src/ASDS.cpp
    src/DpMsg.cpp
    src/PassthroughASDS.cpp
)
set_target_properties(synopsis PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(synopsis PROPERTIES PUBLIC_HEADER include/synopsis.hpp)
target_include_directories(synopsis PRIVATE include)
target_include_directories(synopsis PRIVATE src)

include(GNUInstallDirs)
install(TARGETS synopsis
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Unit Testing

enable_testing()

add_executable(
  synopsis_test
  test/synopsis_test.cpp
)
target_include_directories(synopsis_test PRIVATE include)
target_link_libraries(
  synopsis_test
  gtest_main
  synopsis
)

include(GoogleTest)
gtest_discover_tests(synopsis_test)

