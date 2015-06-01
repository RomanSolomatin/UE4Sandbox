UNAME := $(shell uname)

ifeq ($(UNAME),Darwin)
all:
	xcodebuild -target "MyProject3Editor - Mac" build
else
all:
	$(error "Undefined platform $(UNAME)")
endif

.PHONY: print-%
print-%:
	$(info "Variable $*=$($*)")
