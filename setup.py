from setuptools import setup, Extension

long_desc = """
"""

module = Extension("_cubetree",
                   sources = ["_cubetree/_cubetree.c",
                              "_cubetree/_cubetree_Cube.c",
                              "_cubetree/cube.c",
                              "_cubetree/lookup.c",
                              "_cubetree/search.c",
                              "_cubetree/profile.c"],
                   include_dirs = ["_cubetree/"],
                   extra_compile_args = ["-O3"])

setup(
    name = "cubetree",
    packages = ["cubetree"],
    ext_modules = [module],
    version = "0.1",
    description = "Optimal Rubik's cube solver built for collaborative, "
                  "distributed solving.",
    long_description = long_desc,
    author = "Andrew Bauer",
    author_email = "abau171@gmail.com",
    url = "https://github.com/abau171/cubetree",
    download_url = "https://github.com/abau171/cubetree/archive/v0.1.tar.gz",
    license = "MIT",
    keywords = ["rubik", "cube", "puzzle", "distributed", "solver"],
    classifiers = [
        "Development Status :: 4 - Beta",
        "Intended Audience :: Other Audience",
        "License :: OSI Approved :: MIT License",
        "Programming Language :: Python :: 3 :: Only",
        "Topic :: Games/Entertainment :: Puzzle Games",
        "Topic :: System :: Distributed Computing"
    ],
)

