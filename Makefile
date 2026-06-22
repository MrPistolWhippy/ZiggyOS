CC=gcc
CFLAGS=-O3 -lpthread
TARGET=ziggy_kernel

all: sanitize compile run

sanitize:
	@sed -i 's/static inline //g' *.h 2>/dev/null || true

compile:
	@$(CC) main_v4.c -o $(TARGET) $(CFLAGS)

run:
	@./$(TARGET)

clean:
	@rm -f $(TARGET) *.o
