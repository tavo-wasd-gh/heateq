GO = go
HUGO = hugo

all: docs

docs: .public-lock
.public-lock: .deps-lock config.toml $(shell find content -type f)
	${HUGO} --gc --minify --baseURL "${{ steps.pages.outputs.base_url }}/"
	@touch .public-lock

.deps-lock:
	@${GO} version > /dev/null
	@${HUGO} version > /dev/null
	${GO} mod init heateq || true
	${HUGO} mod get -u || true
	@touch .deps-lock

clean:
	rm -rf public/ \
		resources/ \
		go.mod go.sum \
		.hugo_build.lock \
		.deps-lock \
		.public-lock

.PHONY: all docs clean
