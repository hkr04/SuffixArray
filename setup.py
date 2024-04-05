from distutils.core import setup, Extension
from Cython.Build import cythonize

ext = Extension(name="SuffixArray", sources=["SuffixArray.pyx", "cpp_code/suffix_array.cpp"])

setup(ext_modules = cythonize(ext))