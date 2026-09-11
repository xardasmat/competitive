
CXX=c++
FLAGS=-std=c++23 -O3

build/%: %.cpp build
	${CXX} ${FLAGS} -DNDEBUG -o $@ $<

build/ut_%: %.cpp build
	${CXX} ${FLAGS} -DUT -static -I$(GTEST_INCLUDE_DIR) -L$(GTEST_LIBRARY) -o $@ $< -lgmock -lgtest

build/gen_%: %.cpp build
	${CXX} ${FLAGS} -DGEN -o $@ $<

debug/%: %.cpp debug
	${CXX} ${FLAGS} -g -o $@ $<

test_%: build/%
	oj test -c build/$* -d tests/$*/

ut_%: build/ut_%
	$< --gtest_color=yes

gen_test_%: build/gen_%
	./build/gen_$*

build:
	mkdir -p build

debug:
	mkdir -p debug
