
all: 
	make -C src

test:
	make -C ust

clean:
	make -C src clean
	make -C ust clean
