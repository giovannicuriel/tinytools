#!/bin/sh
project_name=$1
mkdir -p ${project_name}/src

cd ${project_name}

cat << EOF > ./src/main.cpp
#include <iostream>

int main(void) {
  std::cout << "Hi, there" << std::endl;
  return 0;
}
EOF

cat << EOF > Makefile.am
SUBDIRS = src
EOF

cat << EOF > ./src/Makefile.am
bin_PROGRAMS = ${project_name}

${project_name}_SOURCES = main.cpp
EOF

autoscan
mv configure.scan configure.ac

sed -E 's/# Checks for programs/AM_INIT_AUTOMAKE([-Wall -Werror foreign]) \
# Checks for programs/g' ./configure.ac > ./configure.ac-2
rm ./configure.ac
mv ./configure.ac-2 ./configure.ac

touch README AUTHORS ChangeLog NEWS


cat << EOF > autogen.sh
autoreconf -i
automake --add-missing
EOF
chmod +x autogen.sh


