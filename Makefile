# Alternative GNU Make workspace makefile autogenerated by Premake

ifndef config
  config=debug
endif

ifndef verbose
  SILENT = @
endif

ifeq ($(config),debug)
  Core_config = debug
  Application_config = debug

else ifeq ($(config),release)
  Core_config = release
  Application_config = release

else
  $(error "invalid configuration $(config)")
endif

PROJECTS := Core Application

.PHONY: all clean help $(PROJECTS) 

all: $(PROJECTS)

Core:
ifneq (,$(Core_config))
	@echo "==== Building Core ($(Core_config)) ===="
	@${MAKE} --no-print-directory -C Core -f Makefile config=$(Core_config)
endif

Application: Core
ifneq (,$(Application_config))
	@echo "==== Building Application ($(Application_config)) ===="
	@${MAKE} --no-print-directory -C Application -f Makefile config=$(Application_config)
endif

clean:
	@${MAKE} --no-print-directory -C Core -f Makefile clean
	@${MAKE} --no-print-directory -C Application -f Makefile clean

help:
	@echo "Usage: make [config=name] [target]"
	@echo ""
	@echo "CONFIGURATIONS:"
	@echo "  debug"
	@echo "  release"
	@echo ""
	@echo "TARGETS:"
	@echo "   all (default)"
	@echo "   clean"
	@echo "   Core"
	@echo "   Application"
	@echo ""
	@echo "For more information, see https://github.com/premake/premake-core/wiki"