#include "LibOSH.h"


#ifndef LibOSH_NOXML

void Device::toXML(Print& out)
{
    XMLWriter w(&out);
    w.header();
    writeXML(w, false);
}

void Device::writeNamespaces(XMLWriter& w)
{
    w.tagField("xmlns", "http://www.opengis.net/sensorml/2.0");
    w.tagField("xmlns:swe", "http://www.opengis.net/swe/2.0");
    w.tagField("xmlns:gml", "http://www.opengis.net/gml/3.2");
}

#endif



#ifndef LibOSH_NOJSON

void Device::toJSON(Print& out)
{
    // TODO
}

#endif

