/*
 * JSDX Framework
 *
 * Copyright(c) 2012 Fred Chien <fred@mandice.com>
 *
 */

#include <v8.h>
#include <node.h>
#include <poppler.h>
#include <clutter/clutter.h>

#include "jsdx_widget_pdfviewer.hpp"
#include "viewer.hpp"

#define IMAGE_DPI 150

namespace JSDXWidgetPDFViewer {
 
	using namespace node;
	using namespace v8;
	using namespace JSDXToolkit;

	Viewer::Viewer() : Actor() {
		HandleScope scope;

		_actor = NULL;
		document = NULL;
	}

	void Viewer::Initialize(Handle<Object> target)
	{
		HandleScope scope;

		Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
		tpl->InstanceTemplate()->SetInternalFieldCount(1);
		Local<String> name = String::NewSymbol("Viewer");

		/* Methods */
		PrototypeMethodsInit(tpl);

		target->Set(name, tpl->GetFunction());
	}

	void Viewer::PrototypeMethodsInit(Handle<FunctionTemplate> constructor_template)
	{
		HandleScope scope;

		Actor::PrototypeMethodsInit(constructor_template);
//		constructor_template->InstanceTemplate()->SetAccessor(String::NewSymbol("filterQuality"), Viewer::FilterQualityGetter, Viewer::FilterQualitySetter);

		NODE_SET_PROTOTYPE_METHOD(constructor_template, "loadFile", Viewer::LoadFile);
	}

	/* ECMAScript constructor */
	Handle<Value> Viewer::New(const Arguments& args)
	{
		HandleScope scope;

		if (!args.IsConstructCall()) {
			return ThrowException(Exception::TypeError(
				String::New("Use the new operator to create instances of this object."))
			);
		}

		// Creates a new instance object of this type and wraps it.
		Viewer* obj = new Viewer();
		obj->Wrap(args.This());

		/* Create Viewer */
		obj->_actor = clutter_texture_new();
		obj->cr = clutter_cairo_texture_create(CLUTTER_CAIRO_TEXTURE(obj->_actor));
		cairo_scale(obj->cr, IMAGE_DPI/72.0, IMAGE_DPI/72.0);

		return scope.Close(args.This());
	}

	Handle<Value> Viewer::LoadFile(const Arguments &args)
	{
		HandleScope scope;

		if (args[0]->IsString()) {
			Viewer *viewer = ObjectWrap::Unwrap<Viewer>(args.This());

			viewer->document = poppler_document_new_from_file(*String::Utf8Value(args[0]->ToString()), NULL, NULL);
		}

		return args.This();
	}
}
