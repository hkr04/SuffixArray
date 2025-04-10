from distutils.core import setup, Extension
from Cython.Build import cythonize

ext = Extension(name="SuffixArray", 
                sources=["SuffixArray.pyx", "src/suffix_array.cpp"],
                extra_compile_args=["-std=c++11"])

setup(ext_modules = cythonize(ext))