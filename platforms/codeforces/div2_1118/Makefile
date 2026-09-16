
CXX=c++
FLAGS=-std=c++23 -O3
DFLAGS=-std=c++23 -O0 -DDEBUG -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC

build/%: %.cpp build
	${CXX} ${FLAGS} -DNDEBUG -o $@ $<

build/ut_%: %.cpp build
	${CXX} ${FLAGS} -DUT -static -I$(GTEST_INCLUDE_DIR) -L$(GTEST_LIBRARY) -o $@ $< -lgmock -lgtest

build/gen_%: %.cpp build
	${CXX} ${FLAGS} -DGEN -o $@ $<

build/interactor_%: %.cpp build
	${CXX} ${FLAGS} -DINTERACTOR -o $@ $<

debug/%: %.cpp debug
	${CXX} ${DFLAGS} -g -o $@ $<

test_%: build/%
	oj test -c build/$* -d tests/$*/

testi_%: build/% build/interactor_%
	oj test-interactive -c build/$* "build/interactor_$* tests/$*/1.in"

ut_%: build/ut_%
	$< --gtest_color=yes

gen_test_%: build/gen_%
	./build/gen_$*

build:
	mkdir -p build

debug:
	mkdir -p debug
