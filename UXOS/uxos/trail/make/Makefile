CC	:=gcc
CFLAGES	:= -c
OBJS	:= min.o max.o main.o

run:
	./out

out: $(OBJS)
	@echo "CC -o $@ $(OBJS)"
	@$(CC) -o out $(OBJS)
	
%.o: %.c
	@echo "CC -o $@ $<"
	@$(CC) $(CFLAGES) -o $@ $<
	
clean:
	rm -f main.o max.o min.o out