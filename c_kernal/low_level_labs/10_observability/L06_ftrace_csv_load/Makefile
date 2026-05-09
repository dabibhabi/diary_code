# L06 — ftrace function_graph on read(2)

CFLAGS  := -O2 -g -Wall -Wextra
LDFLAGS :=

.PHONY: build run gen-csv drop-caches trace-cold trace-warm trace-stop clean help

help:
	@echo "Targets:"
	@echo "  build          — compile csv_loader (TODO: write it)"
	@echo "  gen-csv        — generate a 1MB sample.csv"
	@echo "  drop-caches    — sudo flush page cache (cold run prep)"
	@echo "  trace-cold     — capture cold-run ftrace into cold.trace"
	@echo "  trace-warm     — capture warm-run ftrace into warm.trace"
	@echo "  trace-stop     — turn ftrace off and reset"
	@echo "  clean          — remove artifacts"

csv_loader: csv_loader.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

build: csv_loader

gen-csv:
	python3 -c "import random; \
	  open('sample.csv','w').writelines(','.join(f'{random.random():.6f}' for _ in range(8))+'\n' for _ in range(50000))"

drop-caches:
	sudo sh -c 'sync && echo 3 > /proc/sys/vm/drop_caches'

trace-cold: build gen-csv drop-caches
	sudo sh -c 'cd /sys/kernel/tracing && \
	  echo function_graph > current_tracer && \
	  echo vfs_read > set_graph_function && \
	  echo > trace && \
	  echo 1 > tracing_on'
	./csv_loader sample.csv
	sudo sh -c 'echo 0 > /sys/kernel/tracing/tracing_on && cat /sys/kernel/tracing/trace' > cold.trace

trace-warm: build
	sudo sh -c 'cd /sys/kernel/tracing && \
	  echo function_graph > current_tracer && \
	  echo vfs_read > set_graph_function && \
	  echo > trace && \
	  echo 1 > tracing_on'
	./csv_loader sample.csv
	sudo sh -c 'echo 0 > /sys/kernel/tracing/tracing_on && cat /sys/kernel/tracing/trace' > warm.trace

trace-stop:
	sudo sh -c 'cd /sys/kernel/tracing && echo nop > current_tracer && echo > set_graph_function'

clean:
	rm -f csv_loader sample.csv cold.trace warm.trace
