CC=gcc

sgp30: sgp30.c
	$(CC) -o $@ $^ $(CFLAGS)  $(LDFLAGS)
