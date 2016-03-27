template <class TIn>
class ParameterClass_1
{
protected:
  TIn param1;

public:
  template <class TOut>
  TOut invoke(TOut (*func)(TIn))
  {
    return func(param1);
  }
};

template <class TIn1, class TIn2>
class ParameterClass_2 : public ParameterClass_1<TIn1>
{
protected:
  TIn2 param2;

public:
  template <class TOut>
  TOut invoke(TOut (*func)(TIn1, TIn2))
  {
    return func(ParameterClass_1<TIn1>::param1, param2);
  }
};

template <class TIn1, class TIn2, class TIn3>
class ParameterClass_3 : public ParameterClass_2<TIn1, TIn2>
{
protected:
  TIn3 param3;

public:
  template <class TOut>
  TOut invoke(TOut (*func)(TIn1, TIn2, TIn3))
  {
    return func(ParameterClass_1<TIn1>::param1, ParameterClass_2<TIn1, TIn2>::param2, param3);
  }
};


template <class TOut>
class ParameterReturnClass
{
  TOut returnValue;
};


template <class TParameters, class TFunc>
class IPCMessage
{
protected:
  TParameters parameters;
  TFunc func;

public:
  void invoke()
  {
    parameters.invoke(func);
  }
};