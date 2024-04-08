
lint:
	@cppcheck --language=c --enable=all --suppress=missingIncludeSystem ./playground

fmt:
	@find . -name '*.c' | xargs -L 1 clang-format -i
