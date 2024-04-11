independency: clean
	g++ independency.cpp -o independency -g3 -ggdb
	./independency

clean:
	rm -rf independency
