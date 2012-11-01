from distutils.core import setup

setup(
    name='TES-analysis',
    version='0.1',
    author='Hugo Benichi',
    author_email='hugo[dot]benichi[at]m4x[dot]com',
    packages=['towelstuff', 'towelstuff.test'],
    #scripts=['bin/stowe-towels.py','bin/wash-towels.py'],
    url='http://github.com/hugobenichi/tes/',
    license='LICENSE',
    description='TES time waveform analysis package',
    long_description=open('README').read(),
    install_requires=[
        "numpy",# >= 1.1.1",
        "scipy",# == 0.1.4",
        "matplotlib"
    ],
)
