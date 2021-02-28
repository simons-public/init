.PHONY: install

init:
	$(CC) init.c -o init -Wall

install: init
	install -b --owner=0 --group=0 --mode=755 -t /etc rc
	install -b --owner=0 --group=0 --mode=755 -t /sbin init

clean:
	rm init
