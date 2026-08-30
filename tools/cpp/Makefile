
CXX=c++
FLAGS=-std=c++20 -O3

# for manual runs
%: %.cpp
	${CXX} ${FLAGS} -o build/$@ $<

# for auto testing
build/%: %.cpp build
	${CXX} ${FLAGS} -o $@ $<

build/gen_%: %.cpp build
	${CXX} ${FLAGS} -DGEN -o $@ $<

debug/%: %.cpp debug
	${CXX} ${FLAGS} -g -o $@ $<

test_%: build/%
	oj test -c build/$* -d tests/$*/
# 	@OJ_DIRS=""; \
# 	for dir in $$(find tests/$*/ -name "*.in" -exec dirname {} \; | sort -u); do \
# 		OJ_DIRS="$$OJ_DIRS -d $$dir"; \
# 	done; \
# 	oj test -c build/$* $$OJ_DIRS

gen_test_%: build/gen_%
	./build/gen_$*

test: build/$(bin)
	./test.sh build/$(bin) $*$(bin)/$(test) $(time)

build:
	mkdir -p build

debug:
	mkdir -p debug
