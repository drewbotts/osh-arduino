#ifndef LIBOSH_H
#define LIBOSH_H

#include "Stream.h"
#include "XMLWriter.h"


#ifndef NULL
#define NULL ((void *) 0)
#endif


// get/set macro
#define OSH_GETSETVAR(type, name) \
protected: \
    type name; \
public: \
    type& get##name() { return name; } \
    void set##name(type newval) { name = newval; }


/**
 * Base class for sensors and systems of multiple sensors
 */
class Device
{
    OSH_GETSETVAR(const char*, UniqueID);
    OSH_GETSETVAR(const char*, Name);
    OSH_GETSETVAR(const char*, Description);
    OSH_GETSETVAR(const double*, Location); // array of size 3 in EPSG 4979

public:
    virtual ~Device() {};

#ifndef LibOSH_NOXML
public:
    void toXML(Print& out);
protected:
    virtual void writeXML(XMLWriter& w, bool nested) = 0;
    void writeNamespaces(XMLWriter& w);
#endif

#ifndef LibOSH_NOJSON
public:
    void toJSON(Print& out);
#endif

};


/**
 * Measurement class represents a sensor measurement output
 */
class Measurement
{
    OSH_GETSETVAR(const char*, Name);
    OSH_GETSETVAR(const char*, Type);
    OSH_GETSETVAR(const char*, Definition);
    OSH_GETSETVAR(const char*, Label);
    OSH_GETSETVAR(const char*, Uom);

public:
    static constexpr char* QUANTITY = "Quantity";
    static constexpr char* QUANTITY_RANGE = "QuantityRange";
    static constexpr char* COUNT = "Count";
    static constexpr char* COUNT_RANGE = "CountRange";
    static constexpr char* CATEGORY = "Category";
    static constexpr char* CATEGORY_RANGE = "CategoryRange";
    static constexpr char* BOOLEAN = "Boolean";

#ifndef LibOSH_NOXML
    void writeXML(XMLWriter& w, bool nested);
#endif

#ifndef LibOSH_NOJSON
    // TODO JSON serialization
#endif
};


/**
 * Sensor class allowing to declare outputs and send data to a stream
 */
class Sensor: public Device
{
friend class System;

protected:
    int numOutputs = 0;
    Measurement* outputs[10];

public:
    void addMeasurement(const char* name, const char* def, const char* uom, const char* label = 0, const char* type = 0);
    void sendData(float* data, Print& out);

#ifndef LibOSH_NOXML
protected:
    void writeXML(XMLWriter& w, bool nested);
    void writeOutput(XMLWriter& w, bool nested);
#endif

#ifndef LibOSH_NOJSON
    // TODO JSON serialization
#endif
};


/**
 * System class is an aggregate of multiple sensors, with multiple
 * (usually asynchronous) outputs
 */
class System: public Device
{
private:
    int numSensors = 0;
    Sensor* sensors[10];

public:
    void addSensor(Sensor* sensor);

#ifndef LibOSH_NOXML
protected:
    void writeXML(XMLWriter& w, bool nested);
#endif

#ifndef LibOSH_NOJSON
    // TODO JSON serialization
#endif
};

#endif
