/*
 * JSDX Framework
 *
 * Copyright(c) 2012 Fred Chien <fred@mandice.com>
 *
 */

#include <v8.h>
#include <node.h>
#include <clutter/clutter.h>

#include "jsdx_widget_pdfviewer.hpp"
#include "viewer.hpp"

namespace JSDXWidgetPDFViewer {
 
	using namespace node;
	using namespace v8;

	extern "C" {
		static void init(Handle<Object> target)
		{
			HandleScope scope;

			Viewer::Initialize(target);
		}

		NODE_MODULE(jsdx_widget_pdfviewer, init);
	}

}
