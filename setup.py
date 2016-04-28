from distutils.core import setup, Extension

module = Extension("_cubetree",
                   sources=["_cubetree/_cubetree.c",
                            "_cubetree/cube.c",
                            "_cubetree/lookup.c",
                            "_cubetree/ida_star.c"],
                   include_dirs=["_cubetree/"])

setup(name="cubetree", py_modules=["cubetree"], ext_modules=[module])

