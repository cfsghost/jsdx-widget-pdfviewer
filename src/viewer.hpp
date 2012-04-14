#ifndef JSDX_WIDGET_PDFVIEWER_VIEWER_H_
#define JSDX_WIDGET_PDFVIEWER_VIEWER_H_

#include <v8.h>
#include <node.h>

#include <poppler.h>
#include <actor.hpp>

namespace JSDXWidgetPDFViewer {

	using namespace JSDXToolkit;

	class Viewer : public Actor {
	public:
		static void Initialize(v8::Handle<v8::Object> target);
		static void PrototypeMethodsInit(v8::Handle<v8::FunctionTemplate> constructor_template);

		PopplerDocument *document;
		cairo_t *cr;

	protected:
		Viewer();

		static v8::Handle<v8::Value> New(const v8::Arguments& args);

		static v8::Handle<v8::Value> LoadFile(const v8::Arguments& args);

//		static v8::Handle<v8::Value> FilterQualityGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info);
//		static void FilterQualitySetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info);
	};

}

#endif
