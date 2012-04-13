#ifndef JSDX_WIDGET_PDFVIEWER_H_
#define JSDX_WIDGET_PDFVIEWER_H_

#include <v8.h>
#include <clutter/clutter.h>

namespace JSDXWidgetPDFViewer {

#define JSDX_WIDGET_DEFINE_CONSTANT(target, name, constant)				\
	(target)->Set(v8::String::NewSymbol(name),							\
	v8::Integer::New(constant),											\
	static_cast<v8::PropertyAttribute>(v8::ReadOnly|v8::DontDelete))

}

#endif
