from setuptools import setup, Extension

module = Extension("_cubetree",
                   sources=["_cubetree/_cubetree.c",
                            "_cubetree/_cubetree_Cube.c",
                            "_cubetree/cube.c",
                            "_cubetree/lookup.c",
                            "_cubetree/search.c",
                            "_cubetree/profile.c"],
                   include_dirs=["_cubetree/"],
                   extra_compile_args=["-O3"])


setup(name="cubetree", packages=["cubetree"], ext_modules=[module])

