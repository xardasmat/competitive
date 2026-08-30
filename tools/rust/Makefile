
RUSTC=rustc
RUSTFLAGS_DEBUG=-C debuginfo=2 --edition=2021
RUSTFLAGS_RELEASE=-C opt-level=3 --edition=2021

build/%: %.rs build
	${RUSTC} ${RUSTFLAGS_RELEASE} $< -o $@ 

debug/%: %.rs debug
	${RUSTC} ${RUSTFLAGS_DEBUG} $< -o $@ 

test_%: build/%
	oj test -c ./build/$* -d test/$*/

build:
	mkdir -p build

debug:
	mkdir -p debug
