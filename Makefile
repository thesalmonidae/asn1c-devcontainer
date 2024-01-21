include make.properties

.DEFAULT_GOAL := help

.PHONY: help

help:  ## Display this help
	@awk 'BEGIN {FS = ":.*##"; printf "\nUsage:\n  make \033[36m\033[0m\n"} /^[a-zA-Z_-]+:.*?##/ { printf "  \033[36m%-15s\033[0m %s\n", $$1, $$2 } /^##@/ { printf "\n\033[1m%s\033[0m\n", substr($$0, 5) } ' $(MAKEFILE_LIST)

##@ ASN.1 Example - Observation module

generate: ## Generate Observation module
	rm -rf src/generated &&\
	mkdir src/generated &&\
	cd src/generated &&\
	asn1c ../../modules/observation.asn &&\
	rm converter-sample.c &&\
	sed -i 's/_BSD_SOURCE/_DEFAULT_SOURCE/g' asn_system.h &&\
	sed -i 's/_BSD_SOURCE/_DEFAULT_SOURCE/g' REAL.c

build: ## Build Observation Generator
	rm -rf .build &&\
	mkdir .build &&\
	LD_LIBRARY_PATH=/usr/local/lib64 gcc -lm -lcjson src/observation-generator.c src/generated/*.c -I src/generated -o .build/observation-generator

run: ## Run Observation Generator
	rm -f observation.xml
	rm -f observation.der
	.build/observation-generator --input observation.json
