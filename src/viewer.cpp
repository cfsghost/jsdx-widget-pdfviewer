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
	using namespace JSDXToolkit;

	Viewer::Viewer() : Actor() {
		HandleScope scope;

		_actor = NULL;
		LoadFinishedFunc = new Persistent<Function>();
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
		constructor_template->InstanceTemplate()->SetAccessor(String::NewSymbol("filterQuality"), Viewer::FilterQualityGetter, Viewer::FilterQualitySetter);

		NODE_SET_PROTOTYPE_METHOD(constructor_template, "loadFile", Viewer::LoadFile);
		NODE_SET_PROTOTYPE_METHOD(constructor_template, "loadFileSync", Viewer::LoadFileSync);
		NODE_SET_PROTOTYPE_METHOD(constructor_template, "keepAspectRatio", Viewer::KeepAspectRatio);
		NODE_SET_PROTOTYPE_METHOD(constructor_template, "on", Viewer::On);
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

		return scope.Close(args.This());
	}

	void Viewer::_LoadFile(Viewer *texture, const char *filename, bool hasCallback)
	{
		CoglHandle cogltex;
		ClutterActor *instance = texture->_actor;

//		if (hasCallback)
//			g_signal_connect(G_OBJECT(instance), "load-finished", G_CALLBACK(Viewer::_LoadFinishedCallback), (gpointer)texture);

		clutter_texture_set_load_async(CLUTTER_TEXTURE(instance), TRUE);

		clutter_texture_set_from_file(CLUTTER_TEXTURE(instance), filename, NULL);
	}

	Handle<Value> Viewer::LoadFile(const Arguments &args)
	{
		HandleScope scope;

		if (args[0]->IsString()) {
			Viewer *texture = ObjectWrap::Unwrap<Viewer>(args.This());

			if (args[1]->IsFunction()) {
				*(texture->LoadFinishedFunc) = Persistent<Function>::New(Handle<Function>::Cast(args[1]));
				_LoadFile(texture, *String::Utf8Value(args[0]->ToString()), True);
			} else {
				_LoadFile(texture, *String::Utf8Value(args[0]->ToString()), False);
			}
		}

		return args.This();
	}

	Handle<Value> Viewer::LoadFileSync(const Arguments &args)
	{
		HandleScope scope;

		if (args[0]->IsString()) {
			Actor *actor = ObjectWrap::Unwrap<Actor>(args.This());
			ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;
			clutter_texture_set_load_async(CLUTTER_TEXTURE(instance), FALSE);
			
			return scope.Close(
				Boolean::New(clutter_texture_set_from_file(CLUTTER_TEXTURE(instance), *String::Utf8Value(args[0]->ToString()), NULL))
			);

			/* TODO: Need callback function for file loaded */
		}

		return scope.Close(
			Boolean::New(False)
		);
	}

	Handle<Value> Viewer::FilterQualityGetter(Local<String> name, const AccessorInfo& info)
	{
		HandleScope scope;

		ClutterActor *instance = ObjectWrap::Unwrap<Actor>(info.This())->_actor;

		return scope.Close(
			Integer::New(clutter_texture_get_filter_quality(CLUTTER_TEXTURE(instance)))
		);
	}

	void Viewer::FilterQualitySetter(Local<String> name, Local<Value> value, const AccessorInfo& info)
	{
		HandleScope scope;

		if (value->IsNumber()) {
			ClutterActor *instance = ObjectWrap::Unwrap<Actor>(info.This())->_actor;

//			clutter_texture_set_filter_quality(CLUTTER_TEXTURE(instance), (ClutterViewerQuality)value->ToInteger()->Value());
		}
	}

	Handle<Value> Viewer::KeepAspectRatio(const Arguments &args)
	{
		HandleScope scope;

		ClutterActor *instance = ObjectWrap::Unwrap<Actor>(args.This())->_actor;

		/* Return value if no parameter */
		if (args[0]->IsUndefined() || args[0]->IsNull())
			return scope.Close(
				Boolean::New(clutter_texture_get_keep_aspect_ratio(CLUTTER_TEXTURE(instance)))
			);

		if (args[0]->IsBoolean()) {
			clutter_texture_set_keep_aspect_ratio(CLUTTER_TEXTURE(instance), args[0]->ToBoolean()->Value());
		}

		return args.This();
	}
}
