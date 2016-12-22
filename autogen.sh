#!/bin/sh
# Run this program to build the configuration
# files for GNU Typist:
# [*/]Makefile.in, aclocal.m4, configure,
# config.h.in, config.sub, config.guess
# INSTALL, configur.bat, doc/gtypist.info, lessons/gtypist.typ
# (maybe more)

echo "Checking for required tools..."

(autoconf --version) < /dev/null > /dev/null 2>&1 || {
    echo
    echo "**Error**: You must have \`autoconf' installed"
    echo "Get ftp://ftp.gnu.org/gnu/autoconf/autoconf-2.57.tar.bz2"
    echo "(or a newer version if it is available)"
    exit 1
}

(automake --version) < /dev/null > /dev/null 2>&1 || {
    echo
    echo "**Error**: You must have \`automake' installed."
    echo "Get ftp://ftp.gnu.org/gnu/automake/automake-1.7.7.tar.bz2"
    echo "(or a newer version if it is available)"
    exit 1
}

(gettext --version) < /dev/null > /dev/null 2>&1 || {
    echo
    echo "**Error**: You must have \`gettext' installed."
    echo "Get ftp://ftp.gnu.org/gnu/gettext/gettext-0.12.1.tar.gz"
    echo "(or a newer version if it is available)"
    exit 1
}

(autopoint --version) < /dev/null > /dev/null 2>&1 || {
    echo
    echo "**Error**: You must have \`autopoint' (part of gettext) installed."
    echo "Get ftp://ftp.gnu.org/gnu/gettext/gettext-0.12.1.tar.gz"
    echo "(or a newer version if it is available)"
    exit 1
}

(help2man --version) < /dev/null > /dev/null 2>&1 || {
    echo
    echo "**Error**: You must have \`help2man' installed."
    echo "Get ftp://ftp.gnu.org/gnu/help2man/help2man_1.33.1.tar.gz"
    echo "(or a newer version if it is available)"
    exit 1
}

(makeinfo --version) < /dev/null > /dev/null 2>&1 || {
    echo
    echo "**Error**: You must have \`texinfo' installed."
    echo "Get ftp://ftp.gnu.org/gnu/texinfo/texinfo-4.6.tar.bz2"
    echo "(or a newer version if it is available)"
    exit 1
}

VERSION=`./version.sh`

for file in INSTALL
  do
  echo "creating $file..."
  if test $file -nt ${file}.in; then
      echo "*** \"$file\" has been modified,"
      echo "but it is generated from ${file}.in."
      echo "Please apply the changes to ${file}.in instead."
      exit -1;
  fi
  sed "s/@VERSION/$VERSION/g" ${file}.in > $file
  # TODO: this causes cvs to think that ${file}.in was modified...
  # solution: use a "stamp" file for each $file ?
  touch ${file}.in
done


# Build configuration files

echo "creating build configuration files..."
aclocal -I m4
autoheader
automake --add-missing

if [ $? != 0 ]; then
    echo ""
    echo "Automake wasn't able to generate the necessary files like"
    echo "config.sub, config.guess, etc. needed for compilation"
    echo "You can copy them from some other place, but it's better"
    echo "to investigate why automake --add-missing failed, so that"
    echo "it doesn't cause some obscure problems later."
    echo "After pressing ENTER, the process will continue, but"
    echo "you can use CONTROL-C to interrupt it."
    read
fi

autoconf
if [ $? != 0 ]; then
    echo -e \
    	"--------------------------------------------------------------------" \
    	"\nSomething was wrong, autoconf failed." \
	"\nConsult notes on how to build if from git in the README.git file," \
	"\nif you don't know how to resolve this."
    exit 1
fi
