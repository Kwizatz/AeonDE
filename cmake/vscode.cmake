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

###########################################
# Generate VS Code Environment
###########################################

message(STATUS "Instrumenting for Visual Studio Code")

if(CMAKE_GENERATOR MATCHES "(MSYS|Unix) Makefiles")
    find_program (GDB_EXECUTABLE gdb HINTS ENV MINGW_PREFIX MSYS2_PATH)
    find_program(BASH_EXECUTABLE bash HINTS ENV MINGW_PREFIX MSYS2_PATH)
    find_program(GLOBAL_EXECUTABLE NAMES global global.exe)
    find_program(CLANG_EXECUTABLE NAMES clang clang.exe)
    find_program(CLANG_TIDY_EXECUTABLE NAMES clang-tidy clang-tidy.exe)

    set(DEBUG_PATH)
    if(CMAKE_GENERATOR MATCHES "MSYS Makefiles")
        set(DEBUG_PATH "${CMAKE_BINARY_DIR}/source/library;$ENV{MINGW_PREFIX}/bin")
    else()
        set(DEBUG_PATH "${CMAKE_BINARY_DIR}/source/library:$ENV{PATH}")
    endif()

    set(DEBUG_CONFIGURATIONS "")

    file(GLOB_RECURSE directories LIST_DIRECTORIES true ${CMAKE_SOURCE_DIR}/source *)
    
    foreach(DIRECTORY ${directories})
        string(REPLACE "${CMAKE_SOURCE_DIR}/source/" "" TARGET_LOCATION ${DIRECTORY})
        if(IS_DIRECTORY ${DIRECTORY} AND EXISTS ${DIRECTORY}/CMakeLists.txt)
            get_directory_property(TARGETS DIRECTORY "${DIRECTORY}" BUILDSYSTEM_TARGETS)
            foreach(TARGET ${TARGETS})
                get_target_property(target_type ${TARGET} TYPE)
                if (${target_type} STREQUAL "EXECUTABLE")
                    message(STATUS "Generating debug launch configuration for ${TARGET}")
                    set(DEBUG_CONFIGURATIONS "${DEBUG_CONFIGURATIONS}
                    {
                    \"name\": \"${TARGET}\",
                    \"type\": \"cppdbg\",
                    \"request\": \"launch\",
                    \"args\": [],
                    \"stopAtEntry\": false,
                    \"cwd\": \"${CMAKE_BINARY_DIR}\",
                    \"environment\": [
                        {
                            \"name\":\"PATH\",
                            \"value\":\"${DEBUG_PATH}\"
                        }                    
                    ],
                    \"externalConsole\": true,
                            \"program\": \"${CMAKE_BINARY_DIR}/source/${TARGET_LOCATION}/${TARGET}${CMAKE_EXECUTABLE_SUFFIX}\",
                            \"miDebuggerPath\": \"${GDB_EXECUTABLE}\",
                            \"MIMode\": \"gdb\",
                            \"setupCommands\": [
                                {
                                    \"description\": \"Enable pretty-printing for gdb\",
                                    \"text\": \"-enable-pretty-printing\",
                                    \"ignoreFailures\": true
                                }
                                ]
                    },\n")
                endif()
            endforeach(TARGET)
        endif()
    endforeach()

    configure_file("${CMAKE_SOURCE_DIR}/cmake/tasks.json.in" "${CMAKE_SOURCE_DIR}/.vscode/tasks.json")
    configure_file("${CMAKE_SOURCE_DIR}/cmake/settings.json.in" "${CMAKE_SOURCE_DIR}/.vscode/settings.json")
    configure_file("${CMAKE_SOURCE_DIR}/cmake/launch.json.in" "${CMAKE_SOURCE_DIR}/.vscode/launch.json")
    configure_file("${CMAKE_SOURCE_DIR}/cmake/c_cpp_properties.json.in" "${CMAKE_SOURCE_DIR}/.vscode/c_cpp_properties.json" @ONLY)
endif()
