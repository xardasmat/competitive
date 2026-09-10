


build/%: %.go build
	go build -o $@ $<

test_%: build/%
	oj test -c ./build/$* -d tests/$*/

build:
	mkdir -p build

