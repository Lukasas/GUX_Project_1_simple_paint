#pragma once

#include <Xm/DrawingA.h>
#include <stack>

template<typename T>
class Controller
{
	using CallbackFn = void (T::*)(Widget, XtPointer, XtPointer);

	struct CallbackData
	{
		T * obj;
		Widget widget;
		XtPointer user_data;
		CallbackFn fn;
	};

	std::stack<CallbackData*> m_Callbacks;


	private:
	static void CallbackCaller(Widget, XtPointer, XtPointer);

	public:
	Controller();
	virtual ~Controller();
	void RegisterCallback(T * obj, Widget widget, String CallbackName, CallbackFn Function, XtPointer client_data);
};

template<typename T>
void Controller<T>::CallbackCaller(Widget widget, XtPointer user_data, XtPointer call_data)
{
	CallbackData * data = (CallbackData*)user_data;
	data->call_data = call_data;
	T * obj = data->obj;
	CallbackFn fn = data->fn;


	(obj->*fn)(widget, data->user_data, call_data);
}

template<typename T>
Controller<T>::Controller()
{

}

template<typename T>
void Controller<T>::RegisterCallback(T * obj, Widget widget, String CallbackName, CallbackFn Function, XtPointer user_data)
{
	CallbackData * data = new CallbackData();
	data->obj = obj;
	data->widget = widget;
	data->user_data = user_data;
	data->fn = Function;

	m_Callbacks.push(data);
	XtAddCallback(widget, CallbackName, CallbackCaller, (XtPointer)data);
}
template<typename T>
Controller<T>::~Controller()
{
	// Unhook all callbacks
	CallbackData *p = NULL;
	while (m_Callbacks.size() > 0)
	{
		p = m_Callbacks.top();
		delete p;
		m_Callbacks.pop();
	}
}