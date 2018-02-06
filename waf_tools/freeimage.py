#!/usr/bin/env python
# JB Mouret - 2014

"""
Quick n dirty freeimage detection
"""

from waflib.Configure import conf

def options(opt):
    opt.add_option('--freeimage', type='string', help='path to Intel TBB', dest='freeimage')

def check_lib(self, name, path):
    if self.env['DEST_OS']=='darwin':
        libname = name + '.dylib'
    else:
        libname = name + '.so'
    res = self.find_file(libname, path)
    lib = res[:-len(libname)-1]
    return res, lib

@conf
def check_freeimage(self, *k, **kw):
    if self.options.freeimage:
        includes_freeimage = [self.options.freeimage + '/include']
        libpath_freeimage = [self.options.freeimage + '/lib']
    else:
        includes_freeimage = ['/usr/local/include', '/usr/include', '/opt/intel/tbb/include']
        libpath_freeimage = ['/usr/local/lib/', '/usr/lib', '/opt/intel/tbb/lib', '/usr/lib/x86_64-linux-gnu/']

    self.start_msg('Checking FreeImage includes')
    incl = ''
    lib = ''
    try:
        res = self.find_file('FreeImage.h', includes_freeimage)
        incl = res[:-len('FreeImage.h')-1]
        self.end_msg(incl)
    except:
        self.end_msg('Not found in %s' % str(includes_freeimage), 'YELLOW')
        return

    self.start_msg('Checking FreeImage libs')
    try:
        res, lib = check_lib(self, 'libfreeimage', libpath_freeimage)
        self.end_msg(lib)
    except:
        self.end_msg('Not found in %s' % str(libpath_freeimage), 'YELLOW')
        return

    self.env.LIBPATH_FREEIMAGE = [lib]
    self.env.LIB_FREEIMAGE = ['freeimage']
    self.env.INCLUDES_FREEIMAGE = [incl]