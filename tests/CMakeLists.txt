include(FetchContent)

FetchContent_Declare(
  googletest
  URL      https://github.com/google/googletest/archive/refs/tags/v1.14.0.tar.gz
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)

# Prevent overriding the parent project's compiler/linker settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(test_polyfit test_polyfit.cpp)
target_link_libraries(test_polyfit PRIVATE polyfit GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(test_polyfit)
