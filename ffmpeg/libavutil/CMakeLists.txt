project(libavutil)

enable_language(ASM)

set(AVUTIL_SRC
	adler32.c
	aes.c
	atomic.c
	audio_fifo.c
	avstring.c
	base64.c
	blowfish.c
	bprint.c
	buffer.c
	channel_layout.c
	cpu.c
	crc.c
	des.c
	dict.c
	downmix_info.c
	error.c
	eval.c
	fifo.c
	file.c
	float_dsp.c
	frame.c
	hash.c
	hmac.c
	imgutils.c
	integer.c
	intfloat_readwrite.c
	intmath.c
	lfg.c
	lls1.c
	lls2.c
	log.c
	log2_tab.c
	lzo.c
	mathematics.c
	md5.c
	mem.c
	murmur3.c
	opt.c
	parseutils.c
	pca.c
	pixdesc.c
	random_seed.c
	rational.c
	rc4.c
	ripemd.c
	samplefmt.c
	sha.c
	sha512.c
	softfloat.c
	stereo3d.c
	time_2.c
	timecode.c
	tree.c
	utils.c
	x86/cpu.c
	x86/float_dsp_init.c
	x86/lls_init.c
	x86/cpuid.asm
	x86/emms.asm
	x86/float_dsp.asm
	x86/lls.asm
	xga_font_data.c
	xtea.c
	orbis/misc_kernel.c
)

set(CMAKE_ASM_COMPILE_OBJECT "<CMAKE_ASM_COMPILER> -f elf64 -m amd64 <INCLUDES> -o <OBJECT> -P config.asm <SOURCE>"  CACHE STRING "" FORCE)

add_library(avutil STATIC ${AVUTIL_SRC})
target_link_libraries(avutil PUBLIC PS4LIBS)
target_include_directories(avutil SYSTEM BEFORE PUBLIC ${__PS4_INCLUDES__} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_compile_options(avutil PRIVATE "-include ../libavutil/internal.h" "-include ../config.h" "-include ../libavutil/orbis/misc_kernel.h")
