all : install

.PHONY : install
install :
	pip3 install .

.PHONY : test
test :
	pytest

.PHONY : lint
lint :
	pylint wiimake

.PHONY : docs
docs :
	sed -i "1s/.*/WiiMake $(shell sed "s/__version__ = '\(.*\)'/\1/" wiimake/_version.py)/" README.rst
	rst2html.py README.rst README.html

.PHONY : msi
msi : 
	python cx_setup.py bdist_msi

.PHONY : win_exe
win_exe : 
	rm -rf build && \
	python cx_setup.py build

.PHONY : win_test
win_test : win_exe
	build/exe.win32-3.5/wiimake.exe --version

#version=v3.3.33
#if [ $(git tag -l "${version}") ]; then
#    echo yes
#else
#    echo no
#fi