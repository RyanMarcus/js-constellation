all: constellation.js driver.min.js

constellation.js: main.c
	emcc -Wall -O3 --closure 1 -s MODULARIZE=1 main.c -s RESERVED_FUNCTION_POINTERS=1 -s EXPORTED_FUNCTIONS='["_createSky", "_progress", "_drawLines", "_getStarX", "_getStarY", "_getStarAt", "_readStarX", "_readStarY"]' -o constellation.js

driver.min.js: node_modules driver.js
	node ./node_modules/babel-cli/bin/babel.js driver.js | ./node_modules/uglifyjs/bin/uglifyjs --lint --mangle > driver.min.js


node_modules: package.json
	npm i

.phony: clean


clean:
	rm -f constellation.js
	rm -f constellation.js.mem
	rm -f driver.min.js
