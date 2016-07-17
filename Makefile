constellation.js: main.c
	emcc -Wall -O3 --closure 1 -s MODULARIZE=1 main.c -s RESERVED_FUNCTION_POINTERS=1 -s EXPORTED_FUNCTIONS='["_createSky", "_progress", "_drawLines", "_getStarX", "_getStarY", "_getStarAt", "_readStarX", "_readStarY"]' -o constellation.js

.phony: clean


clean:
	rm -f constellation.js
	rm -f constellation.js.mem
