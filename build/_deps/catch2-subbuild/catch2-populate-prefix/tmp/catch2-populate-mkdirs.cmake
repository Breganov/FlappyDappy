# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "D:/Code/C++/MultiFlappyBird/build/_deps/catch2-src"
  "D:/Code/C++/MultiFlappyBird/build/_deps/catch2-build"
  "D:/Code/C++/MultiFlappyBird/build/_deps/catch2-subbuild/catch2-populate-prefix"
  "D:/Code/C++/MultiFlappyBird/build/_deps/catch2-subbuild/catch2-populate-prefix/tmp"
  "D:/Code/C++/MultiFlappyBird/build/_deps/catch2-subbuild/catch2-populate-prefix/src/catch2-populate-stamp"
  "D:/Code/C++/MultiFlappyBird/build/_deps/catch2-subbuild/catch2-populate-prefix/src"
  "D:/Code/C++/MultiFlappyBird/build/_deps/catch2-subbuild/catch2-populate-prefix/src/catch2-populate-stamp"
)

set(configSubDirs Debug)
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/Code/C++/MultiFlappyBird/build/_deps/catch2-subbuild/catch2-populate-prefix/src/catch2-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/Code/C++/MultiFlappyBird/build/_deps/catch2-subbuild/catch2-populate-prefix/src/catch2-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
