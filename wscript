#!/usr/bin/env python
# encoding: utf-8
import sys
import os
import fnmatch
import glob
sys.path.insert(0, './waf_tools')

VERSION = '0.0.1'
APPNAME = 'SimpleRayTracer'

srcdir = '.'
blddir = 'build'

from waflib.Build import BuildContext
from waflib import Logs
import tbb

def options(opt):
    opt.load('compiler_cxx')
    opt.load('compiler_c')
    opt.load('tbb')
    opt.load('freeimage')


def configure(conf):
    conf.load('compiler_cxx waf_unit_test')
    conf.load('compiler_c')
    conf.load('tbb')
    conf.load('freeimage')

    conf.check_tbb()
    conf.check_freeimage()

    # we want C++11
    if conf.env.CXX_NAME in ["icc", "icpc"]:
        common_flags = "-Wall -std=c++11"
        opt_flags = " -O3 -xHost -mtune=native -unroll -g"
    elif conf.env.CXX_NAME in ["clang"]:
        common_flags = "-Wall -std=c++11"
        opt_flags = " -O3 -march=native -g"
    else:
        if int(conf.env['CC_VERSION'][0]+conf.env['CC_VERSION'][1]) < 47:
            common_flags = "-Wall -std=c++0x"
        else:
            common_flags = "-Wall -std=c++11"
        opt_flags = " -O3 -march=native -g"

    all_flags = common_flags + opt_flags
    conf.env['CXXFLAGS'] = conf.env['CXXFLAGS'] + all_flags.split(' ')
    print(conf.env['CXXFLAGS'])


def build(bld):
    bld.program(features = 'cxx',
                source='main.cpp',
                includes='./includes',
                uselib = 'TBB FREEIMAGE',
                target='Example')

