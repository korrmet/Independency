run: build
	./independency

build: clean
	g++ independency.cpp -o independency -g3 -ggdb

clean:
	rm -rf independency
