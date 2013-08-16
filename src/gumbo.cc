/*
 *  hello.cc
 *
 *
 *  Created by Pierre Lepers on 15/08/13.
 *  Copyright 2013 Digitas. All rights reserved.
 *
 */




#include <node.h>
#include <gumbo.h>






using namespace v8;

///////////////////////////
//
// Read file and find title


static const char* find_title(const GumboNode* root) {
	assert(root->type == GUMBO_NODE_ELEMENT);
	assert(root->v.element.children.length >= 2);
	
	const GumboVector* root_children = &root->v.element.children;
	GumboNode* head = NULL;
	for (int i = 0; i < root_children->length; ++i) {
		GumboNode* child = (GumboNode*) root_children->data[i];
		if (child->type == GUMBO_NODE_ELEMENT &&
			child->v.element.tag == GUMBO_TAG_HEAD) {
			head = child;
			break;
		}
	}
	assert(head != NULL);
	
	GumboVector* head_children = &head->v.element.children;
	for (int i = 0; i < head_children->length; ++i) {
		GumboNode* child = (GumboNode*) head_children->data[i];
		if (child->type == GUMBO_NODE_ELEMENT &&
			child->v.element.tag == GUMBO_TAG_TITLE) {
			if (child->v.element.children.length != 1) {
				return "<empty title>";
			}
			GumboNode* title_text = (GumboNode*) child->v.element.children.data[0];
			assert(title_text->type == GUMBO_NODE_TEXT);
			return title_text->v.text.text;
		}
	}
	return "<no title found>";
}



/////////////////////////////
//
// Glues and JS functions

Handle<Value> Add(const Arguments& args) {
	HandleScope scope;
	
	if (args.Length() < 1) {
		ThrowException(Exception::TypeError(String::New("Wrong number of arguments")));
		return scope.Close(Undefined());
	}
	
	if (!args[0]->IsString() ) {
		ThrowException(Exception::TypeError(String::New("Wrong arguments")));
		return scope.Close(Undefined());
	}
	
	Local<String> htmlStr = args[0]->ToString();
	int input_length = htmlStr->Length();
	char* input = (char*) malloc( input_length * sizeof(char) );
	htmlStr->WriteAscii( input );
	
	
	GumboOutput* output = gumbo_parse_with_options(
												   &kGumboDefaultOptions, input, input_length);
	const char* title = find_title(output->root);
	gumbo_destroy_output(&kGumboDefaultOptions, output);
	free(input);
	
	
	
	
	Local<String> strTitle = String::New(title);
	return scope.Close(strTitle);
}

void Init(Handle<Object> exports) {
	exports->Set(String::NewSymbol("add"),
				 FunctionTemplate::New(Add)->GetFunction());
}






NODE_MODULE(gumbo, Init)