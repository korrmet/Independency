run: build
	./independency --int 7 --float 3.14 --string foo --flag

build: clean
	g++ independency.cpp -o independency -g3 -ggdb

clean:
	rm -rf independency
