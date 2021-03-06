project(libavformat)

set(AVFORMAT_SRC
	4xm.c
	a64.c
	aacdec.c
	ac3dec.c
	act.c
	adp.c
	adtsenc.c
	adxdec.c
	aea.c
	afc.c
	aiffdec.c
	aiffenc.c
	allformats.c
	amr.c
	anm.c
	apc.c
	ape.c
	apetag.c
	aqtitledec.c
	asf.c
	asfcrypt.c
	asfdec.c
	asfenc.c
	assdec.c
	assenc.c
	ast.c
	astdec.c
	astenc.c
	au.c
	audiointerleave.c
	avc.c
	avidec.c
	avienc.c
	avio.c
	aviobuf.c
	avlanguage.c
	avr.c
	avs.c
	bethsoftvid.c
	bfi.c
	bink.c
	bintext.c
	bit.c
	bmv.c
	boadec.c
	brstm.c
	c93.c
	cache.c
	caf.c
	cafdec.c
	cafenc.c
	cavsvideodec.c
	cdg.c
	cdxl.c
	cinedec.c
	concat.c
	concatdec.c
	crcenc.c
	crypto.c
	cutils.c
	data_uri.c
	daud.c
	dfa.c
	diracdec.c
	dnxhddec.c
	dsicin.c
	dtsdec.c
	dtshddec.c
	dv.c
	dvenc.c
	dxa.c
	eacdata.c
	electronicarts.c
	epafdec.c
	ffmdec.c
	ffmenc.c
	ffmetadec.c
	ffmetaenc.c
	file.c
	file_open.c
	filmstripdec.c
	filmstripenc.c
	flacdec.c
	flacenc.c
	flacenc_header.c
	flac_picture.c
	flic.c
	flvdec.c
	flvenc.c
	format.c
	framecrcenc.c
	framehash.c
	frmdec.c
	ftp.c
	g722.c
	g723_1.c
	g729dec.c
	gif.c
	gifdec.c
	golomb_tab.c
	gsmdec.c
	gxf.c
	gxfenc.c
	h261dec.c
	h263dec.c
	h264dec.c
	hdsenc.c
	hevc.c
	hevcdec.c
	hls.c
	hlsenc.c
	hlsproto.c
	hnm.c
	icodec.c
	icoenc.c
	id3v1.c
	id3v2.c
	id3v2enc.c
	idcin.c
	idroqdec.c
	idroqenc.c
	iff.c
	ilbc.c
	img2.c
	img2dec.c
	img2enc.c
	ingenientdec.c
	ipmovie.c
	ircam.c
	ircamdec.c
	ircamenc.c
	isom.c
	iss.c
	iv8.c
	ivfdec.c
	ivfenc.c
	jacosubdec.c
	jacosubenc.c
	jvdec.c
	latmenc.c
	lmlm4.c
	loasdec.c
	lvfdec.c
	lxfdec.c
	m4vdec.c
	matroska.c
	matroskadec.c
	matroskaenc.c
	md5enc.c
	md5proto.c
	metadata.c
	mgsts.c
	microdvddec.c
	microdvdenc.c
	mkvtimestamp_v2.c
	mm.c
	mmf.c
	mov.c
	movenc.c
	movenchint.c
	mov_chan.c
	mp3dec.c
	mp3enc.c
	mpc.c
	mpc8.c
	mpeg.c
	mpegenc.c
	mpegts.c
	mpegtsenc.c
	mpegvideodec.c
	mpjpeg.c
	mpl2dec.c
	mpsubdec.c
	msnwc_tcp.c
	mtv.c
	mux.c
	mvdec.c
	mvi.c
	mxf.c
	mxfdec.c
	mxfenc.c
	mxg.c
	ncdec.c
	nistspheredec.c
	nsvdec.c
	nullenc.c
	nut.c
	nutdec.c
	nutenc.c
	nuv.c
	oggdec.c
	oggenc.c
	oggparsecelt.c
	oggparsedirac.c
	oggparseflac.c
	oggparseogm.c
	oggparseopus.c
	oggparseskeleton.c
	oggparsespeex.c
	oggparsetheora.c
	oggparsevorbis.c
	oggparsevp8.c
	oma.c
	omadec.c
	omaenc.c
	options.c
	os_support.c
	paf.c
	pcm.c
	pcmdec.c
	pcmenc.c
	pjsdec.c
	pmpdec.c
	psxstr.c
	pva.c
	pvfdec.c
	qcp.c
	r3d.c
	rawdec.c
	rawenc.c
	rawvideodec.c
	realtextdec.c
	redspark.c
	replaygain.c
	riff.c
	riffdec.c
	riffenc.c
	rl2.c
	rm.c
	rmdec.c
	rmenc.c
	rmsipr.c
	rpl.c
	rsd.c
	rso.c
	rsodec.c
	rsoenc.c
	rtmppkt.c
	rtp.c
	rtpenc.c
	rtpenc_aac.c
	rtpenc_amr.c
	rtpenc_chain.c
	rtpenc_h263.c
	rtpenc_h263_rfc2190.c
	rtpenc_h264.c
	rtpenc_jpeg.c
	rtpenc_latm.c
	rtpenc_mpv.c
	rtpenc_vp8.c
	rtpenc_xiph.c
	samidec.c
	sauce.c
	sbgdec.c
	sdp.c
	sdr2.c
	seek.c
	segafilm.c
	segment.c
	sierravmd.c
	siff.c
	smacker.c
	smjpeg.c
	smjpegdec.c
	smjpegenc.c
	smoothstreamingenc.c
	smush.c
	sol.c
	soxdec.c
	soxenc.c
	spdif.c
	spdifdec.c
	spdifenc.c
	srtdec.c
	srtenc.c
	subfile.c
	subtitles.c
	subviewer1dec.c
	subviewerdec.c
	swf.c
	swfdec.c
	swfenc.c
	takdec.c
	tedcaptionsdec.c
	tee.c
	thp.c
	tiertexseq.c
	tmv.c
	tta.c
	tty.c
	txd.c
	uncodedframecrcenc.c
	url.c
	utils.c
	vc1test.c
	vc1testenc.c
	vivo.c
	voc.c
	vocdec.c
	vocenc.c
	vorbiscomment.c
	vplayerdec.c
	vqf.c
	w64.c
	wavdec.c
	wavenc.c
	wc3movie.c
	webvttdec.c
	westwood_aud.c
	westwood_vqa.c
	wtvdec.c
	wtvenc.c
	wtv_common.c
	wv.c
	wvdec.c
	wvenc.c
	xa.c
	xmv.c
	xwma.c
	yop.c
	yuv4mpeg.c
)

add_library(avformat STATIC ${AVFORMAT_SRC})
target_link_libraries(avformat PS4LIBS ScePosix_stub_weak SceFios2_stub_weak)
target_include_directories(avformat SYSTEM BEFORE PUBLIC ${__PS4_INCLUDES__} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/..)
target_compile_options(avformat PRIVATE "-include ../libavutil/internal.h" "-include ../config.h" "-include ../libavutil/orbis/misc_kernel.h")

