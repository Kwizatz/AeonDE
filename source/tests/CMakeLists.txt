# Copyright (C) 2018 Rodrigo Jose Hernandez Cordoba
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

include_directories(${GTEST_INCLUDE_DIR})
set(TESTED_SRCS)
set(TEST_SRCS Main.cpp)

add_executable(unit-tests ${TEST_SRCS} ${TESTED_SRCS})

if(GTEST_LIBRARY AND GTEST_LIBRARY_DEBUG)
 set(GTEST_LIBRARIES optimized ${GTEST_LIBRARY} debug ${GTEST_LIBRARY_DEBUG})
else()
 if(GTEST_LIBRARY_DEBUG)
  set(GTEST_LIBRARIES ${GTEST_LIBRARY_DEBUG})
 endif()
 if(GTEST_LIBRARY)
  set(GTEST_LIBRARIES ${GTEST_LIBRARY})
 endif()
endif()

if(GMOCK_LIBRARY AND GMOCK_LIBRARY_DEBUG)
 set(GMOCK_LIBRARIES optimized ${GMOCK_LIBRARY} debug ${GMOCK_LIBRARY_DEBUG})
else()
 if(GMOCK_LIBRARY_DEBUG)
  set(GMOCK_LIBRARIES ${GMOCK_LIBRARY_DEBUG})
 endif()
 if(GMOCK_LIBRARY)
  set(GMOCK_LIBRARIES ${GMOCK_LIBRARY})
 endif()
endif()

target_link_libraries(unit-tests AeonDE ${GTEST_LIBRARIES} ${GMOCK_LIBRARIES})
if(MSVC)
 # VCPKG compiles GTest/GMock DLLs.
 set_target_properties(unit-tests PROPERTIES
  COMPILE_FLAGS "-DSOURCE_PATH=\"\\\"${CMAKE_SOURCE_DIR}\\\"\" -DGTEST_LINKED_AS_SHARED_LIBRARY -D_CRT_SECURE_NO_WARNINGS -wd4251 -wd4275")
else()
    set_target_properties(unit-tests PROPERTIES
    COMPILE_FLAGS "-DSOURCE_PATH=\"\\\"${CMAKE_SOURCE_DIR}\\\"\"")
endif()
add_test(NAME unit-tests COMMAND unit-tests)
