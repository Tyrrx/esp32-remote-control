#ifndef REMOTECONTROL_ABSTRACTPAYLOAD_H
#define REMOTECONTROL_ABSTRACTPAYLOAD_H
class AbstractPayload {
   private:
    /* data */
   public:
    AbstractPayload();
    ~AbstractPayload();

    virtual bool execute() = 0;
};

AbstractPayload::AbstractPayload() {
}

AbstractPayload::~AbstractPayload() {
}

#endif