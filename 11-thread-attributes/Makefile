ifeq ($(CC),cc)
    CC = gcc
endif

CFLAGS = -std=c17 -D_POSIX_C_SOURCE=200809L -D_XOPEN_SOURCE=700 -D_DARWIN_C_SOURCE -D_DEFAULT_SOURCE -D__BSD_VISIBLE \
         -Wall -Wextra -Werror -pedantic -Wconversion -Wshadow -Wformat=2 -Winit-self -Wswitch-default \
         -Wswitch-enum -Wfloat-equal -Wcast-align -Wpointer-arith -Wmissing-prototypes -Wstrict-prototypes \
         -Wundef -Wstrict-overflow=5 -Wwrite-strings -Wredundant-decls -Winline -Wvla -Wdouble-promotion \
         -Wdeclaration-after-statement
SANITIZERS = -fsanitize=address -fsanitize=undefined -fsanitize=leak
SUPPORTED_SANITIZERS =
CHECKER_REPORT_FILE = cppcheck-report.txt
REPORT_DIR = scan-results

DETACH_SOURCES = detach.c
SCHEDULE_SOURCES = schedule.c
SIZE_SOURCES = size.c
TEMPLATE_SOURCES = template.c

DETACH_BINARY = detach
SCHEDULE_BINARY = schedule
SIZE_BINARY = size
TEMPLATE_BINARY = template

# Attempt to compile with each sanitizer to check support
CHECK_SANITIZER = \
    $(shell echo "int main(){}" > sanitizer_check.c; \
    if $(CC) $(1) sanitizer_check.c -o /dev/null > /dev/null 2>&1; then echo $(1); fi; \
    rm -f sanitizer_check.c)

# Loop over the SANITIZERS and add supported ones to SUPPORTED_SANITIZERS
define LOOP_SANITIZERS
    $(foreach sanitizer,$(SANITIZERS),\
        $(eval SUPPORTED_SANITIZERS += $(call CHECK_SANITIZER,$(sanitizer))))
endef

$(eval $(LOOP_SANITIZERS))

ifeq ($(CC), gcc)
	ANALYSIS_FLAGS = -fanalyzer
	ANALYZER =
	ANALYZER_FLAGS =
else ifeq ($(CC), clang)
	CFLAGS += -Wdocumentation -Wshorten-64-to-32 -Wconstant-conversion -Wnullable-to-nonnull-conversion
	ANALYSIS_FLAGS = --analyze
	ANALYZER = scan-build
	ANALYZER_FLAGS = -disable-checker unix.Malloc -disable-checker deadcode.DeadStores -disable-checker core.StackAddressEscape -plist-html -o $(REPORT_DIR) --status-bugs
endif

all: clang-format $(DETACH_BINARY) $(SCHEDULE_BINARY) $(SIZE_BINARY) $(TEMPLATE_BINARY) analyze clang-tidy cppcheck

# Build rules for each binary
$(DETACH_BINARY): $(DETACH_SOURCES)
	$(CC) $(CFLAGS) $(SUPPORTED_SANITIZERS) -o $(DETACH_BINARY) $(DETACH_SOURCES)

$(SCHEDULE_BINARY): $(SCHEDULE_SOURCES)
	$(CC) $(CFLAGS) $(SUPPORTED_SANITIZERS) -o $(SCHEDULE_BINARY) $(SCHEDULE_SOURCES)

$(SIZE_BINARY): $(SIZE_SOURCES)
	$(CC) $(CFLAGS) $(SUPPORTED_SANITIZERS) -o $(SIZE_BINARY) $(SIZE_SOURCES)

$(TEMPLATE_BINARY): $(TEMPLATE_SOURCES)
	$(CC) $(CFLAGS) $(SUPPORTED_SANITIZERS) -o $(TEMPLATE_BINARY) $(TEMPLATE_SOURCES)

analyze:
	$(ANALYZER) $(ANALYZER_FLAGS) $(CC) $(CFLAGS) $(ANALYSIS_FLAGS) $(DETACH_SOURCES) -c
	$(ANALYZER) $(ANALYZER_FLAGS) $(CC) $(CFLAGS) $(ANALYSIS_FLAGS) $(SCHEDULE_SOURCES) -c
	$(ANALYZER) $(ANALYZER_FLAGS) $(CC) $(CFLAGS) $(ANALYSIS_FLAGS) $(SIZE_SOURCES) -c
	$(ANALYZER) $(ANALYZER_FLAGS) $(CC) $(CFLAGS) $(ANALYSIS_FLAGS) $(TEMPLATE_SOURCES) -c

clang-tidy:
	clang-tidy -quiet --warnings-as-errors=* -checks='*,-cert-err33-c,-clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling,-readability-identifier-length,-llvmlibc-restrict-system-libc-headers,-cppcoreguidelines-init-variables,-altera-unroll-loops,-llvm-header-guard,-cppcoreguidelines-avoid-magic-numbers,-readability-magic-numbers,-misc-unused-parameters,-android-cloexec-dup,-android-cloexec-open,-android-cloexec-creat,-android-cloexec-fopen,-bugprone-easily-swappable-parameters,-altera-id-dependent-backward-branch,-altera-struct-pack-align,-misc-include-cleaner' \
	$(DETACH_SOURCES) $(SCHEDULE_SOURCES) $(SIZE_SOURCES) $(TEMPLATE_SOURCES) \
	-- $(CFLAGS)

cppcheck:
	cppcheck --enable=all --error-exitcode=1 --inconclusive --check-level=exhaustive --inline-suppr --suppress=missingIncludeSystem --checkers-report=$(CHECKER_REPORT_FILE) --quiet \
	$(DETACH_SOURCES) $(SCHEDULE_SOURCES) $(SIZE_SOURCES) $(TEMPLATE_SOURCES)

clang-format:
	clang-format -i -- $(DETACH_SOURCES) $(SCHEDULE_SOURCES) $(SIZE_SOURCES) $(TEMPLATE_SOURCES)

clean:
	rm -f $(DETACH_BINARY) $(SCHEDULE_BINARY) $(SIZE_BINARY) $(TEMPLATE_BINARY)
	rm -f $(CHECKER_REPORT_FILE)
	rm -rf $(REPORT_DIR)
	find . -name '*.plist' -delete
	find . -name '*.o' -delete
