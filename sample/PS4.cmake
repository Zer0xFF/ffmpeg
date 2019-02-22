set(CMAKE_SYSTEM_PROCESSOR  "x86_64")
set(triple "x86_64-scei-ps4")
set(CMAKE_SYSTEM_NAME "FreeBSD")
SET(CMAKE_CROSSCOMPILING 1)
set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)

set(CMAKE_C_COMPILER "/Users/zer0xff/bin/wine.c.sh")
set(CMAKE_CXX_COMPILER "/Users/zer0xff/bin/wine.c++.sh")
set(CMAKE_AR "/Users/zer0xff/bin/wine.ar.sh" CACHE FILEPATH "Archiver")
set(CMAKE_LINKER "/Users/zer0xff/bin/wine.ld.sh")
set(CMAKE_ASM_COMPILER "/Users/zer0xff/bin/wine.as.sh")
# set(CMAKE_FIND_ROOT_PATH "$ENV{SCE_ORBIS_SDK_DIR}/usr" CACHE STRING "")
set(CMAKE_SYSROOT "$ENV{SCE_ORBIS_SDK_DIR}" CACHE STRING "")
set(CMAKE_CXX_COMPILER_FORCED on)
set(CMAKE_C_COMPILER_TARGET "${triple}")
set(CMAKE_CXX_COMPILER_TARGET "${triple}")

# only search for libraries and includes in the PS4SDK directory
#set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
#set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_SKIP_RPATH true)

set(CMAKE_COMMON_FLAGS "-Wall -fdiagnostics-color=always -DNDEBUG -g")
set(CMAKE_COMMON_C_FLAGS "-Wno-unused-function -Wno-unused-label -Werror=implicit-function-declaration -fno-strict-aliasing -fPIC -O3 -D__PS4__ -D__ORBIS__ -DORBIS")

set(CMAKE_C_LINK_EXECUTABLE "<CMAKE_C_COMPILER> <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>" CACHE STRING "")
set(CMAKE_CXX_LINK_EXECUTABLE "<CMAKE_CXX_COMPILER> <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>" CACHE STRING "")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${CMAKE_COMMON_C_FLAGS} ${CMAKE_COMMON_FLAGS} -std=c11" CACHE STRING "")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_COMMON_C_FLAGS} ${CMAKE_COMMON_FLAGS} -std=c++11 " CACHE STRING "")
set(CMAKE_EXE_LINKER_FLAGS "-s -Wl,--addressing=non-aslr,--strip-unused-data -L $ENV{TAUON_SDK_DIR}/lib -L $ENV{SCE_ORBIS_SDK_DIR}/target/lib " CACHE STRING "")
set(CMAKE_ASM_COMPILE_OBJECT "<CMAKE_ASM_COMPILER> <INCLUDES> <FLAGS> -o <OBJECT> -c <SOURCE>"  CACHE STRING "")


#link_directories("$ENV{PS4SDK}/lib")

set(TOOLCHAIN_PS4SDK_LIBS)
LIST(APPEND TOOLCHAIN_PS4SDK_LIBS kernel_tau_stub_weak c_stub_weak SceSysmodule_stub_weak SceSysmodule_tau_stub_weak SceSystemService_stub_weak SceSystemService_tau_stub_weak SceShellCoreUtil_tau_stub_weak ScePigletv2VSH_tau_stub_weak kernel_util)

# this is a work around to avoid duplicate library warning,
# as the toolchain file can be proccessed multiple times
# people over cmake IRC suggest I move this to a find module
if(NOT TARGET PS4::LIBS)
	add_library(PS4LIBS INTERFACE)
	add_library(PS4::LIBS ALIAS PS4LIBS)
	target_link_libraries(PS4LIBS INTERFACE ${TOOLCHAIN_PS4SDK_LIBS})
endif()

if(NOT TARGET PS4::HEADERS)
	add_library(PS4HEADERS INTERFACE)
	add_library(PS4::HEADERS ALIAS PS4HEADERS)
	set(__PS4_INCLUDES__
	"$ENV{TAUON_SDK_DIR}/include"
	"$ENV{SCE_ORBIS_SDK_DIR}/target/include"
	"$ENV{SCE_ORBIS_SDK_DIR}/target/include/common")
	target_include_directories(PS4HEADERS SYSTEM INTERFACE "$ENV{TAUON_SDK_DIR}/include" "$ENV{SCE_ORBIS_SDK_DIR}/target/include" "$ENV{SCE_ORBIS_SDK_DIR}/target/include/common")
	#include_directories("$ENV{TAUON_SDK_DIR}/include" "$ENV{SCE_ORBIS_SDK_DIR}/target/include" "$ENV{SCE_ORBIS_SDK_DIR}/target/include/common")
endif()
