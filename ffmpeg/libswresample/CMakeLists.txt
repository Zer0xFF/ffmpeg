project(libswresample)

enable_language(ASM)

set(SWRESAMPLE_SRC

	audioconvert.c
	dither.c
	rematrix.c
	resample.c
	swresample.c
	x86/swresample_x86.c
	x86/audio_convert.asm
	x86/rematrix.asm
)

set(CMAKE_ASM_COMPILE_OBJECT "<CMAKE_ASM_COMPILER> -f elf64 -m amd64 <INCLUDES> -o <OBJECT> -P config.asm <SOURCE>"  CACHE STRING "" FORCE)

add_library(swresample STATIC ${SWRESAMPLE_SRC})
target_link_libraries(swresample PUBLIC PS4LIBS)
target_include_directories(swresample SYSTEM BEFORE PUBLIC ${__PS4_INCLUDES__} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_compile_options(swsresample PRIVATE "-include ../../../../../../../../${CMAKE_CURRENT_SOURCE_DIR}/../libavutil/internal.h" "-include ../../../../../../../../${CMAKE_CURRENT_SOURCE_DIR}/../config.h" "-include ../../../../../../../../${CMAKE_CURRENT_SOURCE_DIR}/../libavutil/orbis/misc_kernel.h")
