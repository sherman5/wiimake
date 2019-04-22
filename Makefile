SOURCES=$(wildcard wiimake/*py)

all: install

install: 
	pip3 install .

lint : 
	pylint wiimake