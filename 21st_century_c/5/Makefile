build:
	sh build_python

test:
	cd autodemo/py && LD_LIBRARY_PATH=. python -c \
		'import pypvnrt; print(pypvnrt.pressure_from_temp(100))'

clean:
	rm -rf autodemo
