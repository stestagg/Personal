#!/usr/bin/python

from distutils.core import setup
from distutils.extension import Extension
from Cython.Distutils import build_ext

ext_modules=[
    Extension("bitfield",
              ["cimpl/field.pyx"],
              extra_compile_args=["-g"],
              extra_link_args=["-g"],
             )
]

setup(
  name = "Bitfield",
  cmdclass = {"build_ext": build_ext},
  ext_modules = ext_modules,
)
