namespace layer3
{
  class ServiceBase : public layer2::ServiceBase
  {
  protected:
    void setState(State state)
    {
      ServiceState::setState(state);
      stateUpdated(this);
    }

  public:
    events::Event<ServiceBase*> stateUpdated;
    events::Event<ServiceBase*, const char*> statusUpdated;

    void setStatusMessage(const __FlashStringHelper* msg, const char* extendedMessage = nullptr)
    {
      layer2::ServiceBase::setStatusMessage(msg);
      statusUpdated(this, extendedMessage);
    }

    ServiceBase(const char* name) :
      layer2::ServiceBase(name) {}
  };

  class Service : public ServiceBase
  {
    typedef bool (*startService)(Service& svc);

  protected:
    const startService startFunc;

  public:
    Service(PGM_P name, startService startFunc) :
      ServiceBase(name), startFunc(startFunc)
    {
    }

    void start()
    {
      setState(Starting);
      if(startFunc(*this))
        setState(Started);
      else
        setState(Error);
    }
  };
}
