# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Espressif/esp-idf-v5.0.4/components/bootloader/subproject"
  "C:/esp-rainmaker/examples/switch/build/default/bootloader"
  "C:/esp-rainmaker/examples/switch/build/default/bootloader-prefix"
  "C:/esp-rainmaker/examples/switch/build/default/bootloader-prefix/tmp"
  "C:/esp-rainmaker/examples/switch/build/default/bootloader-prefix/src/bootloader-stamp"
  "C:/esp-rainmaker/examples/switch/build/default/bootloader-prefix/src"
  "C:/esp-rainmaker/examples/switch/build/default/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "C:/esp-rainmaker/examples/switch/build/default/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "C:/esp-rainmaker/examples/switch/build/default/bootloader-prefix/src/bootloader-stamp${cfgdir}") # cfgdir has leading slash
endif()
