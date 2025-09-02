from setuptools import find_packages
from setuptools import setup

setup(
    name='xf_voice_msgs',
    version='0.0.1',
    packages=find_packages(
        include=('xf_voice_msgs', 'xf_voice_msgs.*')),
)
