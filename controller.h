#pragma once

#include <Xm/DrawingA.h>
#include <stack>

template<typename T>
class Controller
{
	using CallbackFn = void (T::*)(Widget, XtPointer, XtPointer);
	using EventHandlerFn = void (T::*)(Widget, XtPointer, XEvent*, Boolean*);

	struct CallbackData
	{
		T * obj;
		XtPointer user_data;
		union
		{
			CallbackFn fnCallback;
			EventHandlerFn fnEvent;
		};
	};

	using EventHandlerData = CallbackData;

	std::stack<CallbackData*> m_Callbacks;

	private:
	static void CallbackCaller(Widget, XtPointer, XtPointer);
	static void EventCaller(Widget, XtPointer, XEvent*, Boolean*);

	public:
	Controller();
	virtual ~Controller();
	void RegisterCallback(T * obj, Widget widget, String CallbackName, CallbackFn Function, XtPointer user_data);
	void RegisterEventHandler(T * obj, Widget widget, EventMask event_mask, Boolean nonmaskable, EventHandlerFn, XtPointer user_data);
};

template<typename T>
void Controller<T>::CallbackCaller(Widget widget, XtPointer user_data, XtPointer call_data)
{
	CallbackData * data = (CallbackData*)user_data;
	T * obj = data->obj;
	CallbackFn fnCallback = data->fnCallback;


	(obj->*fnCallback)(widget, data->user_data, call_data);
}

template<typename T>
void Controller<T>::EventCaller(Widget widget, XtPointer user_data, XEvent *event , Boolean * bContinue)
{
	EventHandlerData * data = (EventHandlerData*)user_data;
	T * obj = data->obj;
	EventHandlerFn fnEvent = data->fnEvent;

	(obj->*fnEvent)(widget,data->user_data, event, bContinue);
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
	data->user_data = user_data;
	data->fnCallback = Function;

	m_Callbacks.push(data);
	XtAddCallback(widget, CallbackName, CallbackCaller, (XtPointer)data);
}

template<typename T>
void Controller<T>::RegisterEventHandler(T * obj, Widget widget, EventMask event_mask, Boolean nonmaskable, EventHandlerFn Function, XtPointer user_data)
{
	EventHandlerData * data = new EventHandlerData();
	data->obj = obj;
	data->user_data = user_data;
	data->fnEvent = Function;

	m_Callbacks.push(data);
	XtAddEventHandler(widget, event_mask, nonmaskable, EventCaller, (XtPointer)data);
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