from setuptools import Extension, setup

module = Extension('mysymnmf', sources=['symnmf.c', 'symnmfmodule.c'])
setup(name='mysymnmf',
     version='1.0',
     description='Python wrapper for custom C extension with capi and symnmf',
     ext_modules=[module])