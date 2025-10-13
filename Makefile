all:
	./build.sh

clean:
	rm -rf build/*
.PHONY : clean

run:
	./build/build/pong
.PHONY : run
