from distutils.core import setup, Extension

extension = Extension(
    name='aclib_converter',
    sources=[
        'source/python/ACLIBConverter.cpp',
        'source/Converter.cpp'
    ],
    include_dirs=['source'],
    extra_compile_args=["/EHsc"], )

setup(name='aclib_converter',
      version='1.0',
      ext_modules=[extension])
