
AC_PREREQ([2.69])
AC_INIT([cinelerra], [5.1], [mail@lists.cinelerra-cv.org])
AM_INIT_AUTOMAKE([foreign])

AM_PROG_AS
AC_LANG_CPLUSPLUS
AC_LANG_C
AC_PROG_CXX

CFG_CFLAGS+=" -fno-omit-frame-pointer -fno-math-errno -fno-signed-zeros"
CFG_CFLAGS+=" -pthread -Wall"
# misguided pedantic warnings
CFG_CFLAGS+=" -Wno-unused-result"
CFG_CFLAGS+=" -Wno-stringop-truncation"
CFG_CFLAGS+=" -Wno-stringop-overflow"
CFG_CFLAGS+=" -Wno-format-truncation"
CFG_CFLAGS+=" -Wno-format-overflow"
CFG_CFLAGS+=" -D__STDC_CONSTANT_MACROS"
CFG_CFLAGS+=" -D__STDC_LIMIT_MACROS"
CFG_CFLAGS+=" -DPNG_SKIP_SETJMP_CHECK=1"
CFG_CFLAGS+=" -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64"
CFG_CFLAGS+=" -I/usr/include/freetype2"

AC_DEFUN([CHECK_WITH], [
AC_ARG_WITH([$1],
  AC_HELP_STRING([--with-[$1]], [$2 ($4)]),
  [WANT_$3=$withval], [WANT_$3='$4'])
AC_SUBST([WANT_$3])
])

CHECK_WITH([jobs],[parallel build jobs],[JOBS],[auto])
CHECK_WITH([exec-name],[binary executable name],[CIN],[cin])
CHECK_WITH([single-user],[to install cin in bin],[CINBIN_BUILD],[no])
CHECK_WITH([ladspa-build],[build ladspa library],[LADSPA_BUILD],[yes])
CHECK_WITH([cinlib],[cinelerra library path],[CINLIB_DIR],[auto])
CHECK_WITH([cindat],[cinelerra share path],[CINDAT_DIR],[auto])
CHECK_WITH([plugin-dir],[plugin install dir],[PLUGIN_DIR],[auto])
CHECK_WITH([ladspa-dir],[ladspa install dir],[LADSPA_DIR],[auto])
CHECK_WITH([config-dir],[.bcast config dir],[CONFIG_DIR],[$$HOME/.bcast5])
CHECK_WITH([browser],[cin_browser path],[CIN_BROWSER],[firefox])
CHECK_WITH([git-ffmpeg],[git ffmpeg using url],[GIT_FFMPEG],[no])

if test "x$WANT_CINBIN_BUILD" = "xyes"; then
  WANT_LOCALE_DIR='$$CIN_LIB/locale'
  test "x$WANT_CINDAT_DIR" = "xauto" && WANT_CINDAT_DIR='$$CIN_PATH'
  test "x$WANT_CINLIB_DIR" = "xauto" && WANT_CINLIB_DIR='$$CIN_PATH'
  test "x$WANT_PLUGIN_DIR" = "xauto" && WANT_PLUGIN_DIR='$$CIN_LIB/plugins'
  test "x$WANT_LADSPA_DIR" = "xauto" && WANT_LADSPA_DIR='$$CIN_LIB/ladspa'
  sinstall=dinstall
  suninstall=duninstall
else
  lcldir=`test "x$prefix" = "xNONE" && prefix=$ac_default_prefix; \
    test "x$exec_prefix" = "xNONE" && exec_prefix="${prefix}"; \
    eval "dir=\"$localedir\""; eval "dir=\"$dir\""; eval "dir=\"$dir\""; \
    echo "$dir"`
  usrdat=`test "x$prefix" = "xNONE" && prefix=$ac_default_prefix; \
    test "x$exec_prefix" = "xNONE" && exec_prefix="${prefix}"; \
    eval "dir=\"$datadir\""; eval "dir=\"$dir\""; eval "dir=\"$dir\""; \
    echo "$dir"`
  usrlib=`test "x$prefix" = "xNONE" && prefix="$ac_default_prefix"; \
    test "x$exec_prefix" = "xNONE" && exec_prefix="${prefix}"; \
    eval "dir=\"$libdir\""; eval "dir=\"$dir\""; eval "dir=\"$dir\""; \
    echo "$dir"`
  test "x$usrlib" = "x/lib" -o "x$usrlib" = "x/usr/lib" -o \
       "x$usrlib" = "x/usr/local/lib" -a -e "/usr/lib64" && \
  test ! -e "/usr/lib" -o `ls -1 /usr/lib64 | wc -l` -gt `ls -1 /usr/lib | wc -l` && \
    usrlib+="64"
  WANT_LOCALE_DIR="$lcldir"
  test "x$WANT_CINDAT_DIR" = "xauto" && WANT_CINDAT_DIR="$usrdat/$WANT_CIN"
  test "x$WANT_CINLIB_DIR" = "xauto" && WANT_CINLIB_DIR="$usrlib/$WANT_CIN"
  test "x$WANT_PLUGIN_DIR" = "xauto" && WANT_PLUGIN_DIR="$WANT_CINLIB_DIR/plugins"
  test "x$WANT_LADSPA_DIR" = "xauto" && WANT_LADSPA_DIR="$WANT_CINLIB_DIR/ladspa"
  sinstall=cinstall
  suninstall=cuninstall
fi
AC_SUBST(sinstall)
AC_SUBST(suninstall)

if test "x$WANT_JOBS" = "xauto"; then
  CPUS=`grep -c "^proc" /proc/cpuinfo`
  WANT_JOBS=`expr $CPUS + $CPUS / 2 + 2`
fi

AC_DEFUN([PKG_DEF],[
STATIC_PKGS+=" [$1]"
ver_$1="$2"
lib_$1="$3"
inc_$1="$4"
])

AC_DEFUN([PKG_3RD],[
AC_ARG_ENABLE([$1],
  AC_HELP_STRING([--enable-$1],[build $1 ($2)]),
  [PKG_$1=$enableval], [PKG_$1=$2])
PKG_DEF([$1],[$3],[$4],[$5])
])

# thirdparty builds

PKG_3RD([a52dec],[yes],
  [a52dec-0.7.4],
  [ liba52/.libs/liba52.a ],
  [ include ])

PKG_3RD([djbfft],[yes],
  [djbfft-0.76],
  [ libdjbfft.a ],
  [ . ])

PKG_3RD([audiofile],[no],
  [audiofile-0.3.6],
  [ libaudiofile/.libs/libaudiofile.a \
    libaudiofile/modules/.libs/libmodules.a \
    libaudiofile/alac/.libs/libalac.a ],
  [ libaudiofile ])

PKG_3RD([encore],[no],
  [encore-5.0],
  [ \$(OBJDIR)/libencore.a ],
  [ . ])

PKG_3RD([esound],[no],
  [esound-0.2.41],
  [ .libs/libesd.a \
    .libs/libesddsp.a ],
  [ . ])

PKG_3RD([fdk],[auto],
  [fdk-aac-0.1.5],
  [ .libs/libfdk-aac.a ],
  [ libAACdec/include libAACenc/include libSYS/include ])

PKG_3RD([ffmpeg],[yes],
  [ffmpeg-3.4.1],
  [ libavutil/libavutil.a \
    libavcodec/libavcodec.a \
    libpostproc/libpostproc.a \
    libavdevice/libavdevice.a \
    libavformat/libavformat.a \
    libswscale/libswscale.a \
    libavfilter/libavfilter.a \
    libswresample/libswresample.a ],
  [ . ])

if test "x$WANT_GIT_FFMPEG" != "xno" ; then
  ver_ffmpeg="ffmpeg.git"
fi

PKG_3RD([fftw],[auto],
  [fftw-3.3.7],
  [ .libs/libfftw3.a \
    libbench2/libbench2.a \
    rdft/scalar/r2cb/.libs/librdft_scalar_r2cb.a \
    rdft/scalar/r2r/.libs/librdft_scalar_r2r.a \
    rdft/scalar/.libs/librdft_scalar.a \
    rdft/scalar/r2cf/.libs/librdft_scalar_r2cf.a \
    rdft/.libs/librdft.a \
    api/.libs/libapi.a \
    reodft/.libs/libreodft.a \
    dft/scalar/codelets/.libs/libdft_scalar_codelets.a \
    dft/scalar/.libs/libdft_scalar.a \
    dft/.libs/libdft.a \
    kernel/.libs/libkernel.a \
    simd-support/.libs/libsimd_support.a ],
  [ api ])

PKG_3RD([flac],[auto],
  [flac-1.3.2],
  [ src/libFLAC/.libs/libFLAC.a \
    src/libFLAC++/.libs/libFLAC++.a \
    src/share/replaygain_analysis/.libs/libreplaygain_analysis.a \
    src/share/replaygain_synthesis/.libs/libreplaygain_synthesis.a \
    src/share/grabbag/.libs/libgrabbag.a \
    src/share/utf8/.libs/libutf8.a \
    src/test_libs_common/.libs/libtest_libs_common.a ],
  [ include ])

PKG_3RD([giflib],[yes],
  [giflib-5.1.4],
  [ lib/.libs/libgif.a \
    util/libgetarg.a ],
  [ lib ])

PKG_3RD([ilmbase],[auto],
  [ilmbase-2.2.1],
  [ Iex/.libs/libIex.a \
    IexMath/.libs/libIexMath.a \
    Half/.libs/libHalf.a \
    Imath/.libs/libImath.a \
    IlmThread/.libs/libIlmThread.a ],
  [ Iex Half Imath config IlmThread ])

PKG_DEF([ladspa], [ladspa-0.4.17], [], [])

PKG_3RD([lame],[auto],
  [lame-3.100],
  [ libmp3lame/.libs/libmp3lame.a \
    mpglib/.libs/libmpgdecoder.a ],
  [ include ])

PKG_3RD([libavc1394],[auto],
  [libavc1394-0.5.4],
  [ libavc1394/.libs/libavc1394.a \
    librom1394/.libs/librom1394.a \
    common/.libs/libraw1394util.a ],
  [ . ])

PKG_3RD([libraw1394],[auto],
  [libraw1394-2.1.2],
  [ src/.libs/libraw1394.a ],
  [ . ])

PKG_3RD([libiec61883],[auto],
  [libiec61883-1.2.0],
  [ src/.libs/libiec61883.a ],
  [ src ])

PKG_3RD([libdv],[auto],
  [libdv-1.0.0],
  [ libdv/.libs/libdv.a ],
  [ . ])

PKG_3RD([libjpeg],[auto],
  [libjpeg-turbo-1.5.1],
  [ .libs/libjpeg.a \
    .libs/libturbojpeg.a \
    simd/.libs/libsimd.a ],
  [ . ])

PKG_3RD([opus],[auto],
  [opus-1.2.1],
  [ .libs/libopus.a ],
  [ include ])

PKG_3RD([openjpeg],[auto],
  [openjpeg-2.1.0-20160221],
  [ bin/libopenjp2.a ],
  [ src/lib/openjp2 ])

PKG_3RD([libogg],[auto],
  [libogg-1.3.2],
  [ src/.libs/libogg.a ],
  [ include ])

PKG_3RD([libsndfile],[auto],
  [libsndfile-1.0.28],
  [ src/.libs/libsndfile.a \
    src/.libs/libcommon.a \
    src/G72x/.libs/libg72x.a \
    src/GSM610/.libs/libgsm.a ],
  [ . ])

PKG_3RD([libtheora],[auto],
  [libtheora-1.1.1],
  [ lib/.libs/libtheora.a \
    lib/.libs/libtheoradec.a \
    lib/.libs/libtheoraenc.a ],
  [ . ])

PKG_3RD([libuuid],[yes],
  [libuuid-1.0.3],
  [ .libs/libuuid.a ],
  [ . ])

PKG_3RD([libvorbis],[auto],
  [libvorbis-1.3.5],
  [ lib/.libs/libvorbis.a \
    lib/.libs/libvorbisenc.a \
    lib/.libs/libvorbisfile.a ],
  [ include . ])

PKG_3RD([mjpegtools],[yes],
  [mjpegtools-2.1.0],
  [ utils/mmxsse/.libs/libmmxsse.a \
    utils/.libs/libmjpegutils.a \
    lavtools/.libs/liblavfile.a \
    lavtools/.libs/liblavjpeg.a \
    mplex/.libs/libmplex2.a \
    yuvfilters/.libs/libyuvfilters.a \
    mpeg2enc/.libs/libmpeg2encpp.a ],
  [ .  lavtools utils ])

PKG_3RD([openexr],[auto],
  [openexr-2.2.1],
  [ IlmImf/.libs/libIlmImf.a \
    IlmImfUtil/.libs/libIlmImfUtil.a ],
  [ IlmImf config ])

#PKG_3RD([festival],[no],
#  [festival],
#  [],
#  [])
#PKG_3RD(speech_tools,[no],
#  [speech_tools],
#  [],
#  [])
#
PKG_3RD([tiff],[auto],
  [tiff-4.0.9],
  [ libtiff/.libs/libtiff.a \
    libtiff/.libs/libtiffxx.a \
    port/.libs/libport.a ],[
   . ])

PKG_3RD([twolame],[auto],
  [twolame-0.3.13],
  [ libtwolame/.libs/libtwolame.a ],
  [  libtwolame ])

PKG_3RD([x264],[auto],
  [x264-snapshot-20180118-2245],
  [ libx264.a ],
  [ . ])

PKG_3RD([x265],[auto],
  [x265_v2.6],
  [ libx265.a ],
  [ . source ])

PKG_3RD([libvpx],[auto],
  [libvpx-1.6.1],
  [ libvpx.a ],
  [ . ])

AC_SUBST(STATIC_PKGS)

AC_DEFUN([CHECK_ENABLE], [
AC_ARG_ENABLE([$1],
  AC_HELP_STRING([--enable-[$1]], [$3 ([$4])]),
  [WANT_$2=$enableval], [WANT_$2=[$4]])
])

CHECK_ENABLE([static-build], [STATIC_BUILD], [build static], [auto])
CHECK_ENABLE([x264_hidepth], [X264_HIDEPTH], [build x264 10bit], [no])

test "x$WANT_STATIC_BUILD" = "xauto" && WANT_STATIC_BUILD=$WANT_CINBIN_BUILD

AC_DEFUN([CHECK_PROG], [
 AC_CHECK_PROG([PROG_$1], [$2], [yes],[no])
])
AC_DEFUN([REQUIRE_PROG], [
 CHECK_PROG([$1],[$2])
 if test "x$PROG_$1" = "xno"; then
   AC_MSG_WARN([$2 is missing and required.])
   FATAL_ERROR=1
 fi
])

# Checks for programs.

REQUIRE_PROG(NASM, [nasm])
REQUIRE_PROG(YASM, [yasm])
REQUIRE_PROG(OBJCOPY, [objcopy])
if test "x$FATAL_ERROR" != "x"; then
 AC_MSG_ERROR("fatal eror.")
fi
CHECK_PROG(PACTL, [pactl])
WANT_PACTL=$PROG_PACTL

# libx264 nasm fix
AC_MSG_CHECKING([nasm x264 compatible])
echo "vmovdqa32 [[eax]]{k1}{z}, zmm0" > conftest.asm
nasm conftest.asm -o conftest.o > /dev/null 2>&1
if test $? != 0 ; then
 AC_MSG_RESULT([no])
 AC_MSG_WARN([libx264 built without assembly code])
 X264_CFG_PARAMS="$X264_CFG_PARAMS --disable-asm"
else
 AC_MSG_RESULT([yes])
fi
rm -f conftest.asm conftest.o

AC_CHECK_DECL([X_HAVE_UTF8_STRING],,[no_utf=yes],[#include <X11/Xlib.h>])
if test "$no_utf" = "yes"; then
  AC_MSG_ERROR([Cinelerra requires utf8 support in X Windows.])
fi

# Checks for headers/libraries.

AC_DEFUN([CHECK_HEADERS], [
if test "x$HAVE_$1" != "xno"; then
 AC_CHECK_HEADER(m4_apply([m4_car],m4_split([$3])), [HAVE_$1=yes], [HAVE_$1=no],
  [m4_foreach([i], m4_split([$3]), [[#include "]i["
]])])
 AC_SUBST([HAVE_$1])
 AC_MSG_CHECKING([$2])
 AC_MSG_RESULT([$HAVE_$1])
 if test "x$HAVE_$1" = "xno"; then
  if test "x$PKG_$1" = "x"; then RESULT=no; fi
 fi
fi
])

AC_DEFUN([CHECK_LIB], [
if test "x$HAVE_$1" != "xno"; then
 if test "x$PKG_$1" = "x" -o "x$WANT_STATIC_BUILD" = "xno"; then
  AC_CHECK_LIB([$2], [$3], [HAVE_$1=yes], [HAVE_$1=no], [$4])
  AC_SUBST([HAVE_$1])
  if test "x$HAVE_$1" = "xyes"; then
   if test "x$PKG_$1" = "x"; then
    SYSTEM_LIBS+=" -l[$2]"
    for lib in [$4]; do SYSTEM_LIBS+=" $lib"; done
   else
    SHARED_$1+=" -l[$2]"
    for lib in [$4]; do SHARED_LIBS+=" $lib"; done
   fi
  elif test "x$PKG_$1" = "x"; then RESULT=no; fi
 fi
fi
])

CHECK_LIB([X11], [X11], [XOpenDisplay])
CHECK_HEADERS([X11], [X11 headers], [X11/X.h X11/Xlib.h X11/Xutil.h X11/cursorfont.h])
CHECK_HEADERS([X11], [X11 headers], [X11/keysym.h X11/Sunkeysym.h X11/Xatom.h X11/XF86keysym.h])
CHECK_LIB([XEXT], [Xext], [XShmQueryExtension])
CHECK_HEADERS([XEXT], [Xlib XShm extention], [X11/Xlib.h X11/extensions/XShm.h X11/extensions/shape.h])
CHECK_LIB([XINERAMA], [Xinerama], [XineramaQueryExtension])
CHECK_HEADERS([XINERAMA], [Xinerama headers], [X11/extensions/Xinerama.h])
CHECK_LIB([XV], [Xv], [XvQueryExtension])
CHECK_HEADERS([XV], [Xlib Xv extention], [X11/Xlib.h X11/extensions/Xvlib.h])
CHECK_LIB([XFIXES], [Xfixes], [XFixesQueryVersion])
CHECK_LIB([BZ2], [bz2], [BZ2_bzDecompress])
CHECK_LIB([FONTCONFIG], [fontconfig], [FcInit])
CHECK_LIB([FREETYPE], [freetype], [FT_Init_FreeType])
CHECK_LIB([LZMA], [lzma], [lzma_version_number])
CHECK_LIB([PNG], [png], [png_read_png])
CHECK_LIB([PTHREAD], [pthread], [pthread_create])
CHECK_LIB([Z], [z], [gzopen])
CHECK_LIB([fftw], [fftw3], [fftw_execute])
CHECK_HEADERS([fftw], [fftw headers], [fftw3.h])
CHECK_LIB([libuuid], [uuid], [uuid_clear])
CHECK_HEADERS([libuuid], [uuid headers], [uuid/uuid.h])
CHECK_HEADERS([LINUX_CDROM], [linux cdrom headers], [linux/cdrom.h linux/futex.h])
CHECK_HEADERS([LINUX_SYS], [linux sys], [sys/ioctl.h sys/stat.h sys/types.h sys/time.h sys/syscall.h])
CHECK_LIB([flac], [FLAC], [FLAC__stream_decoder_new])
CHECK_HEADERS([flac], [FLAC headers], [FLAC/stream_decoder.h])
CHECK_LIB([lame], [mp3lame], [lame_init])
CHECK_HEADERS([lame], [lame headers], [lame/lame.h])
CHECK_LIB([libjpeg], [jpeg], [jpeg_start_decompress])
CHECK_HEADERS([libjpeg], [jpeg headers], [stdio.h jpeglib.h])
CHECK_LIB([libogg], [ogg], [ogg_stream_init])
CHECK_HEADERS([libogg], [ogg headers], [ogg/ogg.h])
CHECK_LIB([openjpeg], [openjp2], [opj_version])
CHECK_HEADERS([openjpeg], [openjpeg headers], [openjpeg.h])
CHECK_LIB([libsndfile], [sndfile], [sf_open])
CHECK_HEADERS([libsndfile], [sndfile headers], [sndfile.h])
CHECK_LIB([ilmbase], [IlmImf], [ImfOpenInputFile])
CHECK_HEADERS([ilmbase], [IlmImf headers], [OpenEXR/ImfCRgbaFile.h])
CHECK_LIB([libtheora], [theora], [theora_info_init], [-ltheoraenc -ltheoradec -logg])
CHECK_HEADERS([libtheora], [threora headers], [theora/theoraenc.h])
CHECK_LIB([libvorbis], [vorbisenc], [vorbis_encode_init], [-lvorbis -lvorbisfile -logg])
CHECK_HEADERS([libvorbis], [vorbis encoders headers], [vorbis/vorbisenc.h])
CHECK_LIB([libvorbis], [vorbisfile], [ov_open])
CHECK_HEADERS([libvorbis], [vorbis file headers], [vorbis/vorbisfile.h])
CHECK_LIB([libvpx], [vpx], [vpx_codec_decode])
CHECK_HEADERS([libvpx], [vpx headers], [vpx/vpx_decoder.h])
CHECK_LIB([mjpegtools], [mjpegutils], [mjpeg_info])
CHECK_HEADERS([mjpegtools], [mjpegtools headers], [mjpegtools/mjpeg_types.h])
CHECK_LIB([tiff], [tiff], [TIFFOpen])
CHECK_HEADERS([tiff], [tiff headers], [tiff.h tiffio.h])
CHECK_LIB([twolame], [twolame], [twolame_encode_buffer_float32_interleaved])
CHECK_HEADERS([twolame], [twolame headers], [twolame.h])
CHECK_LIB([x264], [x264], [x264_encoder_encode])
CHECK_HEADERS([x264], [x264 headers], [stdint.h x264.h])
CHECK_LIB([x265], [x265], [x265_encoder_encode])
CHECK_HEADERS([x265], [x265 headers], [x265.h])
CHECK_LIB([a52dec], [a52], [a52_init])
CHECK_HEADERS([a52dec], [a52 headers], [a52.h])
CHECK_LIB([encore], [encore], [encore])
CHECK_HEADERS([encore], [encore headers], [encore.h])
CHECK_LIB([giflib], [gif], [DGifOpen])
CHECK_HEADERS([giflib], [gif lib headers], [gif_lib.h])
CHECK_LIB([fdk], [fdk-aac], [faacDecInit])
CHECK_HEADERS([fdk], [fdk headers], [fdk-aac/genericStds.h])
CHECK_LIB([jbig], [jbig], [jbg_dec_init])
CHECK_LIB([vdpau], [vdpau], [vdp_device_create_x11])

#if test "x$HAVE_mjpegtools" = "xyes"; then
#CFG_CFLAGS+=" -I/usr/include/mjpegtools"
#fi

AC_DEFUN([CHECK_WANT], [
RESULT=yes
AC_ARG_WITH(m4_tolower([$1]),
 AC_HELP_STRING([m4_join([-],[--with],m4_tolower([$1]))], [$3] (auto)),
  [WANT_$1=$withval],[WANT_$1=$2])
# WANT_$1
if test "x$WANT_$1" != "xno" ; then
 m4_quote($4)
 if test "x$RESULT" = "xno" -a "x$PKG_$1" = "x"; then
  echo "=== want $1 Failed."
  WANT_$1=no
 elif test "x$WANT_$1" = "xauto" ; then
  WANT_$1=yes
 fi
fi
CFG_WANTS+=" $1"
AC_SUBST([WANT_$1])
])

CHECK_WANT([GL], [auto], [use opengl], [
 CHECK_LIB([GL], [GL], [glUseProgram])
 CHECK_LIB([GL], [GLU], [gluOrtho2D])
 CHECK_HEADERS([GL], [opengl headers], [GL/gl.h GL/glext.h GL/glu.h GL/glx.h])])

CHECK_WANT([OSS], [auto], [use OSS audio], [
 CHECK_HEADERS([OSS], [oss headers], [sys/soundcard.h])])

CHECK_WANT([XFT], [auto], [use libXft], [
 CHECK_LIB([XFT], [Xft], [XftInit])
 CHECK_HEADERS([XFT], [Xft/freetype headers], [X11/Xlib.h X11/Xft/Xft.h])])

CHECK_WANT([ALSA], [auto], [use libasound/alsa], [
 CHECK_LIB([ALSA], [asound], [snd_pcm_open])
 CHECK_HEADERS([ALSA], [asound headers], [alsa/asoundlib.h])])

CHECK_WANT([FIREWIRE], [auto], [use firewire], [
 CHECK_LIB([libavc1394], [avc1394], [avc1394_init_target])
 CHECK_HEADERS([libavc1394], [libavc1394 headers], [libavc1394/avc1394.h])
 CHECK_LIB([libavc1394], [rom1394], [rom1394_get_bus_id])
 CHECK_HEADERS([libavc1394], [librom1394 headers], [libavc1394/rom1394.h])
 CHECK_LIB([libiec61883], [iec61883], [iec61883_mpeg2_recv_init])
 CHECK_HEADERS([libiec61883], [libiec61883 headers], [libiec61883/iec61883.h])
 CHECK_LIB([libraw1394], [raw1394], [raw1394_iso_recv_init])
 CHECK_HEADERS([libraw1394], [libraw1394 headers], [libraw1394/raw1394.h])])

CHECK_WANT([DV], [auto], [use dv], [
 CHECK_LIB([libdv], [dv], [dv_init])
 CHECK_HEADERS([libdv], [libdv headers], [libdv/dv.h])])

CHECK_WANT([DVB], [auto], [use dvb], [
 CHECK_HEADERS([DVB], [dvb device headers], [linux/dvb/dmx.h linux/dvb/frontend.h])])

CHECK_WANT([VIDEO4LINUX2], [auto], [use v4l2], [
 CHECK_HEADERS([VIDEO4LINUX2], [v4l2 system headers], [linux/kernel.h linux/types.h linux/videodev2.h])])

CHECK_WANT([XXF86VM], [auto], [use xf86vmode], [
 CHECK_HEADERS([XXF86VM], [XF86VM headers], [X11/Xlib.h X11/extensions/xf86vmode.h])])

CHECK_WANT([ESOUND], [no], [use esd], [
 CHECK_LIB([esound], [esd], [esd_open_sound])
 CHECK_HEADERS([esound], [esound headers], [esd.h])
 CHECK_LIB([audiofile], [audiofile], [afOpenFile])
 CHECK_HEADERS([audiofile], [audiofile headers], [audiofile.h])])

CHECK_WANT([LIBOPUS], [auto], [use libopus], [
 CHECK_LIB([opus], [opus], [opus_multistream_decoder_create])
 CHECK_HEADERS([opus], [libopus headers], [opus/opus_multistream.h])])

CHECK_WANT([DL], [auto], [system has libdl], [
 CHECK_LIB([DL], [dl], [dlopen])])

CHECK_WANT([EXR], [auto], [use exr], [])
CHECK_WANT([OPENCV], [no], [opencv=sys/sta/dyn,git/tar=url], [
 WANT_OPENCV="$withval"
])

CHECK_WANT([NUMA], [auto], [system has libnuma], [
 CHECK_LIB([NUMA], [numa], [numa_alloc])])

CHECK_WANT([OPENEXR], [auto], [use openexr], [
 AC_MSG_CHECKING([for openexr available])
 saved_LIBS="$LIBS"
 saved_CXXFLAGS="$CXXFLAGS"
 # ilmbase libs
 LIBS=" -lIlmImf -lIlmThread -lIex -lpthread"
 CXXFLAGS="-I/usr/include/OpenEXR"
 AC_LANG_PUSH(C++)
 AC_LINK_IFELSE([AC_LANG_PROGRAM([[
#include "ImfChannelList.h"
#include "ImfChromaticities.h"
#include "ImfCompression.h"
#include "ImfIO.h"
#include "ImfInputFile.h"
#include "ImfOutputFile.h"
#include "ImfPixelType.h"
#include "ImfRgbaFile.h"
#include "ImfRgbaYca.h"
#include "ImfVersion.h"
class EXRIStream : public Imf::IStream { public:
EXRIStream() : Imf::IStream("mypath") {} };
class EXROStream : public Imf::OStream { public: 
EXROStream() : Imf::OStream("mypath") {} };
]])], [HAVE_OPENEXR=yes], [HAVE_OPENEXR=no])
 RESULT="$HAVE_OPENEXR"
 if test "x$RESULT" = "xyes"; then
   SHARED_openexr="$LIBS"
   CFG_CFLAGS+=" -I/usr/include/OpenEXR"
 fi
 LIBS="$saved_LIBS"
 CXXFLAGS="$saved_CXXFLAGS"
 AC_LANG_POP(C++)
 AC_MSG_RESULT([$HAVE_OPENEXR])
])
AC_SUBST([HAVE_OPENEXR])

# build global_config
OBJDIR=`uname -m`
AC_SUBST(OBJDIR)

echo "AC_HELP_STRING([Reason],[Package])"
echo "AC_HELP_STRING([------],[-------])"

AC_DEFUN([PKG_DISABLED],[
 PKG_$1="no"
 BUILD_$1=0
 AC_SUBST(BUILD_$1)
 echo "AC_HELP_STRING([disabled],[$1])"
])
AC_DEFUN([PKG_SHARED],[
 BUILD_$1=0
 AC_SUBST(BUILD_$1)
 SHARED_LIBS+="$SHARED_$1"
 echo "AC_HELP_STRING([shared],[$1])"
])
AC_DEFUN([PKG_STATIC],[
 PKG_$1="yes"
 BUILD_$1=1
 AC_SUBST(BUILD_$1)
 STATIC_BLDS+=" [$1]"
 echo "AC_HELP_STRING([$2],[$1])"
])
AC_DEFUN([PKG_FORCED],[PKG_STATIC([$1],[forced])])

# order matters
for dep in \
        ffmpeg/twolame ffmpeg/lame ffmpeg/openjpeg \
        ffmpeg/libvorbis ffmpeg/libtheora ffmpeg/x264 ffmpeg/x265 ffmpeg/fdk \
        libiec61883/libraw1394 libavc1394/librom1394 \
        openexr/ilmbase ilmbase/libogg \
        libtheora/libogg libtheora/libvorbis ; do
  lib=`dirname $dep`;  needs=`basename $dep`
  eval pkg_lib="\$PKG_$lib"; eval needs_lib="\$HAVE_$needs";
  if test "x$pkg_lib" != "xno"; then
    eval "PKG_$needs=yes"
  fi
done

AC_DEFUN([PKG_PROVIDE],[
if test "x$BUILD_$1" = "x" ; then
 if test "x$2" = "xno" -o "x$PKG_$1" = "xno"; then
  PKG_DISABLED([$1])
 elif test "x$PKG_$1" = "xyes"; then
  PKG_FORCED([$1])
 elif test "x$WANT_STATIC_BUILD" = "xyes"; then
  PKG_STATIC([$1],[static])
 elif test "x$HAVE_$1" = "xyes"; then
  PKG_SHARED([$1])
 else
  PKG_STATIC([$1],[needed])
 fi
fi
])

PKG_PROVIDE([a52dec])
PKG_PROVIDE([djbfft])
PKG_PROVIDE([encore])
PKG_PROVIDE([audiofile],[$WANT_ESOUND])
PKG_PROVIDE([esound],[$WANT_ESOUND])
PKG_PROVIDE([fdk])
PKG_PROVIDE([ffmpeg])
PKG_PROVIDE([fftw])
PKG_PROVIDE([flac])
PKG_PROVIDE([giflib])
PKG_PROVIDE([ilmbase], [$WANT_OPENEXR])
PKG_PROVIDE([ladspa],[$WANT_LADSPA_BUILD])
PKG_PROVIDE([lame])
PKG_PROVIDE([libavc1394],[$WANT_FIREWIRE])
PKG_PROVIDE([libraw1394],[$WANT_FIREWIRE])
PKG_PROVIDE([libiec61883],[$WANT_FIREWIRE])
PKG_PROVIDE([libdv],[$WANT_DV])
PKG_PROVIDE([libjpeg])
PKG_PROVIDE([libogg])
PKG_PROVIDE([libsndfile])
PKG_PROVIDE([libtheora])
PKG_PROVIDE([libuuid])
PKG_PROVIDE([libvorbis])
PKG_PROVIDE([mjpegtools])
PKG_PROVIDE([openexr], [$WANT_OPENEXR])
PKG_PROVIDE([openjpeg])
PKG_PROVIDE([tiff])
PKG_PROVIDE([twolame])
PKG_PROVIDE([x264])
PKG_PROVIDE([x265])
PKG_PROVIDE([libvpx])
PKG_PROVIDE([opus], [$WANT_OPUS])

AC_SUBST(STATIC_BLDS)
AC_SUBST(SHARED_LIBS)

for f in $SYSTEM_LIBS; do
  echo "AC_HELP_STRING([system],[$f])"
done
AC_SUBST(SYSTEM_LIBS)

echo ""
for v in GL XFT XXF86VM OSS ALSA FIREWIRE DV DVB \
	 VIDEO4LINUX2 ESOUND PACTL OPENEXR; do
  eval vv="\$WANT_$v"
  if test "x$vv" != "xno"; then
    CFG_CFLAGS+=" -DHAVE_$v"
    vv=with
  else
    vv=without
  fi
  v=`echo "$v" | tr "A-Z" "a-z"`
  echo "  using: $vv-$v"
done

echo "  using: with-jobs = $WANT_JOBS"
echo "  using: exec-name = $WANT_CIN"
echo "  using: with-cinlib = $WANT_CINLIB_DIR"
echo "  using: with-cindat = $WANT_CINDAT_DIR"
echo "  using: with-config-dir = $WANT_CONFIG_DIR"
echo "  using: with-browser = $WANT_CIN_BROWSER"
echo "  using: with-plugin-dir = $WANT_PLUGIN_DIR"
echo "  using: with-ladspa-dir = $WANT_LADSPA_DIR"
echo "  using: with-opencv = $WANT_OPENCV"
echo "  using: with-git-ffmpeg = $WANT_GIT_FFMPEG"
echo ""
echo "  using: single-user  = $WANT_CINBIN_BUILD"
echo "  using: static-build = $WANT_STATIC_BUILD"
echo "  using: ladspa-build = $WANT_LADSPA_BUILD"
echo ""

# build extras
if test "x$HAVE_tiff" = "xyes"; then
  if test "x$HAVE_jbig" = "xyes"; then
    EXTRA_LIBS+=' -ljbig'
  fi
fi
if test "x$HAVE_NUMA" = "xyes"; then
  EXTRA_LIBS+=' -lnuma'
fi
if test "x$HAVE_DL" = "xyes"; then
  EXTRA_LIBS+=' -ldl'
  FFMPEG_EXTRA_CFG+=' --extra-ldflags="-ldl"'
fi
if test "x$HAVE_opus" = "xyes"; then
  EXTRA_LIBS+=' -lopus'
  CFG_CFLAGS+=' -I/usr/include/opus'
  FFMPEG_EXTRA_CFG+=' --enable-libopus'
fi

AC_SUBST(EXTRA_LIBS)
AC_SUBST(FFMPEG_EXTRA_CFG)
AC_SUBST(WANT_X264_HIDEPTH)

AC_SUBST(CFG_CFLAGS)
AC_SUBST(CFG_CXXFLAGS)

AC_CONFIG_FILES([Makefile])
AC_OUTPUT

exec > global_config

echo "SHELL := /bin/bash"
echo "export SHELL"
echo "WANT_JOBS := $WANT_JOBS"
echo ""
echo "OBJDIR := $OBJDIR"
echo "BINDIR := \$(TOPDIR)/bin"
echo "CINELERRA := \$(TOPDIR)/cinelerra"
echo "GUICAST := \$(TOPDIR)/guicast"
echo "LIBZMPEG3 := \$(TOPDIR)/libzmpeg3"
echo "MPEG2ENC := \$(TOPDIR)/mpeg2enc"
echo "MPLEXLO := \$(TOPDIR)/mplexlo"
echo "PLUGINS := \$(TOPDIR)/plugins"
echo "TDB := \$(TOPDIR)/db"
echo "THIRDPARTY := \$(TOPDIR)/thirdparty"
echo "EXTRA_LIBS := $EXTRA_LIBS"
echo "FFMPEG_EXTRA_CFG := $FFMPEG_EXTRA_CFG"
echo "export OBJDIR BINDIR CINELERRA GUICAST LIBZMPEG3"
echo "export MPEG2ENC MPLEXLO PLUGINS TDB"
echo "export THIRDPARTY EXTRA_LIBS FFMPEG_EXTRA_CFG"
echo ""

echo "WANT_CIN := $WANT_CIN"
for w in $CFG_WANTS; do ww=WANT_$w; echo "WANT_$w := ${!ww}"; done
echo ""

echo "ifndef CFLAGS_"
echo "ifdef CFLAGS"
echo "CFLAGS_ := \$(CFLAGS)"
echo "else"
echo "CFLAGS_ := ${CFLAGS:--g -O2}"
echo "endif"
echo "endif"

echo "CFLAGS := \$(CFLAGS_)"
echo "CFLAGS += '-DCIN=\"\$(WANT_CIN)\"'"
echo "CFLAGS += '-DCINLIB_DIR=\"$WANT_CINLIB_DIR\"'"
echo "CFLAGS += '-DCINDAT_DIR=\"$WANT_CINDAT_DIR\"'"
echo "CFLAGS += '-DCONFIG_DIR=\"$WANT_CONFIG_DIR\"'"
echo "CFLAGS += '-DPLUGIN_DIR=\"$WANT_PLUGIN_DIR\"'"
echo "CFLAGS += '-DLOCALE_DIR=\"$WANT_LOCALE_DIR\"'"
echo "CFLAGS += '-DLADSPA_DIR=\"$WANT_LADSPA_DIR\"'"
echo "CFLAGS += '-DCIN_BROWSER=\"$WANT_CIN_BROWSER\"'"
for flg in $CFG_CFLAGS; do echo "CFLAGS += $flg"; done
echo ""
for flg in $CFG_CXXFLAGS; do echo "CXXFLAGS += $flg"; done
echo ""

if test  $WANT_X264_HIDEPTH = "yes" ; then
  X264_CFG_PARAMS="$X264_CFG_PARAMS --bit-depth=10"
fi

for pkg in $STATIC_PKGS; do
  eval pkg_lib="\$PKG_$pkg"
  if test "x$pkg_lib" = "xno"; then continue; fi;
  echo "static_pkgs += $pkg"
done
echo ""

for pkg in $STATIC_BLDS; do
  echo "static_blds += $pkg"
  ver_pkg=ver_$pkg
  echo "$ver_pkg := \$(THIRDPARTY)/${!ver_pkg}"
  inc_pkg=inc_$pkg
  echo "$inc_pkg := \\"
  for inc in ${!inc_pkg}; do
    echo "  -I\$($ver_pkg)/$inc \\"
  done
  echo ""
  echo "static_incs += \$(inc_$pkg)"
  lib_pkg=lib_$pkg
  echo "$lib_pkg := \\"
  for lib in ${!lib_pkg}; do
    echo "  \$($ver_pkg)/$lib \\"
  done
  echo ""
  echo "static_libs += \$($lib_pkg)"
  echo ""
done
echo ""

for lib in $SHARED_LIBS; do echo "shared_libs += $lib"; done
echo ""
for lib in $SYSTEM_LIBS; do echo "system_libs += $lib"; done
echo "export static_pkgs static_blds shared_libs system_libs"
echo ""

echo "thirdparty_libraries := \$(static_libs) \$(shared_libs)"
echo "libraries += -Wl,--start-group"
echo "libraries += \$(thirdparty_libraries)"
echo "libraries += \$(system_libs)"
echo "libraries += \$(EXTRA_LIBS)"
echo "libraries += -Wl,--end-group"
# -Wl,--start-group ... -Wl,--end-group does not work on ubuntu
echo "libraries += \$(thirdparty_libraries)"
echo "libraries += \$(system_libs)"
echo "libraries += \$(EXTRA_LIBS)"
echo ""
echo "export thirdparty_libraries libraries"
echo "export CFLAGS_ CXXFLAGS_ LDFLAGS_"
echo "unexport CFLAGS CXXFLAGS LDFLAGS"

if test "x$HAVE_tiff" = "xyes"; then
  if test "x$HAVE_jbig" != "xyes"; then
    echo "tiff.cfg_params+= --disable-jbig"
  fi
fi

if test "x$WANT_GIT_FFMPEG" != "xno"; then
echo "ffmpeg.git := $WANT_GIT_FFMPEG"
fi
if test "x$HAVE_opus" = "xyes"; then
echo 'ffmpeg.cflags+=" -I/usr/include/opus"'
fi
if test "x$X264_CFG_PARAMS" != "x" ; then
  echo "x264.cfg_params :=$X264_CFG_PARAMS --enable-static --enable-pic"
fi
