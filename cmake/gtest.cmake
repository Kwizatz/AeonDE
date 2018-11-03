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

#######################################
# Unit Testing
#######################################

find_path (GTEST_SOURCE_PATH googletest PATHS /usr/src/googletest)
if(GTEST_SOURCE_PATH)
    add_subdirectory(${GTEST_SOURCE_PATH} ${CMAKE_BINARY_DIR}/googletest)
    set(GTEST_LIBRARY gtest)
    set(GMOCK_LIBRARY gmock)
    add_subdirectory(tests)
else()
    find_package(GTest)
    if(GTest_FOUND)
        find_library(GMOCK_LIBRARY
                    NAMES gmock
                        libgmock
                        libgmock.a
                    PATHS "${GTEST_DIR}"
                    PATH_SUFFIXES lib)
        find_library(GMOCK_LIBRARY_DEBUG
                    NAMES gmockd
                        libgmockd
                        libgmockd.a
                    PATHS "${GTEST_DIR}"
                    PATH_SUFFIXES lib)
    endif()
endif()
