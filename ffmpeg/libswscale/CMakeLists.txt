enable_language(ASM)

enable_language(ASM)


set(SWSCALE_SRC
	colorspace-test.c
	input.c
	options.c
	output.c
	rgb2rgb.c
	swscale-test.c
	swscale.c
	swscale_unscaled.c
	utils.c
	yuv2rgb.c
	#	x86/swscale_mmx.asm
	x86/rgb2rgb.c
	x86/swscale.c
	x86/yuv2rgb.c
	x86/input.asm
	x86/output.asm
	x86/scale.asm
)
set(CMAKE_ASM_COMPILE_OBJECT "<CMAKE_ASM_COMPILER> -f elf64 -m amd64 <INCLUDES> -o <OBJECT> -P config.asm <SOURCE>"  CACHE STRING "" FORCE)

add_library(swscale STATIC ${SWSCALE_SRC})
target_link_libraries(swscale PUBLIC avutil PS4LIBS ScePosix_stub_weak)
target_include_directories(swscale SYSTEM BEFORE PUBLIC ${__PS4_INCLUDES__} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_compile_options(swscale PRIVATE "-include ../../../../../../../../${CMAKE_CURRENT_SOURCE_DIR}/../libavutil/internal.h" "-include ../../../../../../../../${CMAKE_CURRENT_SOURCE_DIR}/../config.h" "-include ../../../../../../../../${CMAKE_CURRENT_SOURCE_DIR}/../libavutil/orbis/misc_kernel.h")

