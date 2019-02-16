#ifndef REMOTECONTROL_EXAMPLEPAYLOAD_H
#define REMOTECONTROL_EXAMPLEPAYLOAD_H

#include <AbstractPayload.h>

class ExamplePayload : AbstractPayload {
   private:
    /* data */
   public:
    ExamplePayload(/* args */);
    ~ExamplePayload();
    bool execute();
};

ExamplePayload::ExamplePayload(/* args */) {
}

ExamplePayload::~ExamplePayload() {
}

bool ExamplePayload::execute() {
}

#endif