AC_INIT(tmaClient,1.0.0)
AM_INIT_AUTOMAKE([-Wall -Werror foreign])

m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_MAKE_SET

dnl is it a debug build?
CFLAGS="-O3"
AC_ARG_ENABLE(debug,
AS_HELP_STRING([--enable-debug],
               [enable debugging, default: no]),
[case "${enableval}" in
             yes)
		debug=true
		CFLAGS="-g -O0"
		AC_DEFINE_UNQUOTED([DEBUG],  $withval  ,[debug enabled])
		;;
             no) 
		debug=false
	       CFLAGS="-O3"	
		;;
             *)   AC_MSG_ERROR([bad value ${enableval} for --enable-debug]) ;;
esac],
[debug=false])

AU_ALIAS([CHECK_SSL], [AX_CHECK_OPENSSL])
AC_DEFUN([AX_CHECK_OPENSSL], [
    found=false
    AC_ARG_WITH([openssl],
        [AS_HELP_STRING([--with-openssl=DIR],
            [root of the OpenSSL directory])],
        [
            case "$withval" in
            "" | y | ye | yes | n | no)
            AC_MSG_ERROR([Invalid --with-openssl value])
              ;;
            *) ssldirs="$withval"
              ;;
            esac
        ], [
            # if pkg-config is installed and openssl has installed a .pc file,
            # then use that information and don't search ssldirs
            AC_CHECK_TOOL([PKG_CONFIG], [pkg-config])
            if test x"$PKG_CONFIG" != x""; then
                OPENSSL_LDFLAGS=`$PKG_CONFIG openssl --libs-only-L 2>/dev/null`
                if test $? = 0; then
                    OPENSSL_LIBS=`$PKG_CONFIG openssl --libs-only-l 2>/dev/null`
                    OPENSSL_INCLUDES=`$PKG_CONFIG openssl --cflags-only-I 2>/dev/null`
                    found=true
                fi
            fi

            # no such luck; use some default ssldirs
            if ! $found; then
                ssldirs="/usr/local/ssl /usr/lib/ssl /usr/ssl /usr/pkg /usr/local /usr"
            fi
        ]
        )


    # note that we #include <openssl/foo.h>, so the OpenSSL headers have to be in
    # an 'openssl' subdirectory

    if ! $found; then
        OPENSSL_INCLUDES=
        for ssldir in $ssldirs; do
            AC_MSG_CHECKING([for include/openssl/ssl.h in $ssldir])
            if test -f "$ssldir/include/openssl/ssl.h"; then
                OPENSSL_INCLUDES="-I$ssldir/include"
                OPENSSL_LDFLAGS="-L$ssldir/lib"
                OPENSSL_LIBS="-lssl -lcrypto"
                found=true
                AC_MSG_RESULT([yes])
                break
            else
                AC_MSG_RESULT([no])
            fi
        done

        # if the file wasn't found, well, go ahead and try the link anyway -- maybe
        # it will just work!
    fi

    # try the preprocessor and linker with our new flags,
    # being careful not to pollute the global LIBS, LDFLAGS, and CPPFLAGS

    AC_MSG_CHECKING([whether compiling and linking against OpenSSL works])
    echo "Trying link with OPENSSL_LDFLAGS=$OPENSSL_LDFLAGS;" \
        "OPENSSL_LIBS=$OPENSSL_LIBS; OPENSSL_INCLUDES=$OPENSSL_INCLUDES" >&AS_MESSAGE_LOG_FD

    save_LIBS="$LIBS"
    save_LDFLAGS="$LDFLAGS"
    save_CPPFLAGS="$CPPFLAGS"
    LDFLAGS="$LDFLAGS $OPENSSL_LDFLAGS"
    LIBS="$OPENSSL_LIBS $LIBS"
    CPPFLAGS="$OPENSSL_INCLUDES $CPPFLAGS"
    AC_LINK_IFELSE(
        [AC_LANG_PROGRAM([#include <openssl/ssl.h>], [SSL_new(NULL)])],
        [
            AC_MSG_RESULT([yes])
            $1
        ], [
            AC_MSG_RESULT([no])
            $2
        ])
    CPPFLAGS="$save_CPPFLAGS"
    LDFLAGS="$save_LDFLAGS"
    LIBS="$save_LIBS"

    AC_SUBST([OPENSSL_INCLUDES])
    AC_SUBST([OPENSSL_LIBS])
    AC_SUBST([OPENSSL_LDFLAGS])
])

dnl wiringPi pin connected to PTT
    AC_ARG_WITH([ptt-pin],
        [AS_HELP_STRING([--with-ptt-pin=29],
            [wPi pin number where radio PTT is connected, use gpio readall for checkings pins])],
        [
            AC_DEFINE_UNQUOTED([PTT_PIN],  $withval  ,[wPi pin connected to PTT])
        ], [
            AC_DEFINE_UNQUOTED([PTT_PIN],[ 29 ],[wPi pin connected to PTT (29=physical pin 40 on RPI3b])
        ]
        )

dnl wiringPi pin connected to radio relay
    AC_ARG_WITH([power-pin],
        [AS_HELP_STRING([--with-power-pin=28],
            [wPi pin number where radio power relay is connected, use gpio readall for checkings pins])],
        [
            AC_DEFINE_UNQUOTED([POWER_PIN],  $withval  ,[wPi pin connected to power relay])
        ], [
            AC_DEFINE_UNQUOTED([POWER_PIN],[ 28 ],[wPi pin connected to PTT (28=physical pin 38 on RPI3b])
        ]
        )

dnl Sound files location
    AC_ARG_WITH([sound-files],
        [AS_HELP_STRING([--with-sound-files=DIR],
            [root of the sound files directory])],
        [
            AC_DEFINE_UNQUOTED([SOUND_FILES_LOCATION],  "$withval"  ,[Sound files location])
        ], [
            AC_DEFINE_UNQUOTED([SOUND_FILES_LOCATION],[ "./" ],[Sound files location])
        ]
        )

dnl Check curl:
AC_CHECK_LIB([curl], [curl_easy_setopt], [], [AC_MSG_ERROR([curl_easy_setopt was not found in libcurl])])
dnl Check ao:
AC_CHECK_LIB([ao], [ao_open_live], [], [AC_MSG_ERROR([ao_open_live was not found in libao])])
dnl Check mpg123:
AC_CHECK_LIB([mpg123], [mpg123_init], [], [AC_MSG_ERROR([mpg123_init was not found in libmpg123])])
dnl Check libwiringPi2:
AC_CHECK_LIB([wiringPi], [digitalWrite], [], [AC_MSG_NOTICE([digitalWrite was not found in libwiringPi])])

AX_CHECK_OPENSSL()

AC_CONFIG_HEADERS([config.h])
AC_OUTPUT(Makefile)
