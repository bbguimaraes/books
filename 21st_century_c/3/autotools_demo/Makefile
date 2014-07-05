all: autodemo

autodemo:
	sh auto.conf

test:
	cd autodemo && \
		tar -xf hello-1.tar.gz
	cd autodemo/hello-1 && ./configure && make
	autodemo/hello-1/hello

clean:
	rm -rf autodemo/
