import Options
from os import unlink, symlink, popen
from os.path import exists

srcdir = "."
blddir = "build"
VERSION = "0.0.1"

def set_options(opt):
	opt.tool_options("compiler_cxx")
	opt.add_option('--enable-widget', action='store', default=1, help='Enable widget support [Default: True]')

def configure(conf):
	conf.check_tool("compiler_cxx")
	conf.check_tool("node_addon")
	conf.check_cfg(package='poppler-glib', uselib_store='POPPLER', args='--cflags --libs')
	conf.check_cfg(package='clutter-1.0', uselib_store='CLUTTER', args='--cflags --libs')

	if Options.options.enable_widget:
		print "Enabled Widget Support"
		conf.env["ENABLE_MX"] = True
		conf.env["ENABLE_WIDGET"] = True
		conf.check_cfg(package='mx-1.0', uselib_store='MX', args='--cflags --libs')

def build(bld):
	obj = bld.new_task_gen("cxx", "shlib", "node_addon")
	obj.cxxflags = ["-Wall", "-ansi", "-pedantic"]
	obj.target = "jsdx_widget_pdfviewer"
	obj.source = """
		src/jsdx_widget_pdfviewer.cpp
		src/viewer.cpp
		"""
	obj.cxxflags = ["-D_FILE_OFFSET_BITS=64", "-D_LARGEFILE_SOURCE"]
	obj.uselib = "CLUTTER POPPLER"
	obj.includes = """
		node_modules/jsdx-toolkit/src
	"""

	if bld.env["ENABLE_MX"]:
		obj.cxxflags.append("-DENABLE_MX");

		obj.uselib += " MX"

def shutdown():
	if Options.commands['clean']:
		if exists('jsdx_widget_pdfviewer.node'): unlink('jsdx_widget_pdfviewer.node')
	else:
		if exists('build/default/jsdx_widget_pdfviewer.node') and not exists('jsdx_widget_pdfviewer.node'):
			symlink('build/default/jsdx_widget_pdfviewer.node', 'jsdx_widget_pdfviewer.node')
