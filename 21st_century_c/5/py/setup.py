from distutils.core import Extension, setup

Emodule = Extension(
    'pypvnrt',
    libraries=[ 'pvnrt' ],
    library_dirs=[ '..' ],
    sources=[ 'ideal.py.c' ])

setup(
    name='pypvnrt',
    version='1.0',
    description='pressure * volume = n * R * Temperature',
    ext_modules=[ Emodule ])
