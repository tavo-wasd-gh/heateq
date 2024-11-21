GO = go
HUGO = hugo

all: docs

serve: .deps-lock
	${HUGO} server \
		--disableFastRender \
		--noHTTPCache \
		--ignoreCache

docs: .public-lock
.public-lock: .deps-lock config.toml $(shell find content -type f)
	${HUGO} --gc --minify
	@touch .public-lock

.deps-lock:
	@${GO} version > /dev/null
	@${HUGO} version > /dev/null
	${GO} mod init github.com/tavo-wasd-gh/heateq > /dev/null
	${HUGO} mod get -u > /dev/null
	@touch .deps-lock

clean:
	rm -rf public/ \
		resources/ \
		go.mod go.sum \
		.hugo_build.lock \
		.deps-lock \
		.public-lock

.PHONY: all docs serve clean
