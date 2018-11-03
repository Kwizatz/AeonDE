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
# Code Formating
#######################################

find_program(CLANG_FORMAT_EXECUTABLE NAMES clang-format clang-format.exe)
find_program (GREP_EXECUTABLE grep HINTS ENV MINGW_PREFIX MSYS2_PATH ProgramFiles PATH_SUFFIXES usr/bin Git/usr/bin)
if(CLANG_FORMAT_EXECUTABLE)
	set(FORMAT_OPTIONS "--options=${CMAKE_SOURCE_DIR}/astylerc")
	configure_file("${CMAKE_SOURCE_DIR}/cmake/pre-commit.in" "${CMAKE_SOURCE_DIR}/.git/hooks/pre-commit")
endif()
