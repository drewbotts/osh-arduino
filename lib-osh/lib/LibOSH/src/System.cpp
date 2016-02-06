#include "LibOSH.h"


void System::addSensor(Sensor* sensor)
{
    this->sensors[numSensors++] = sensor;
}


#ifndef LibOSH_NOXML

void System::writeXML(XMLWriter& w, bool nested)
{
    w.tagStart("PhysicalSystem");
    w.tagField("gml:id", "SYS01");
    if (!nested)
        writeNamespaces(w);
    w.tagEnd(true, false);

    w.writeNode("gml:description", this->Description);
    w.writeNode("gml:identifier", this->UniqueID);

    // output description
    w.tagOpen("outputs");
    w.tagOpen("OutputList");

    for (int i = 0; i < numSensors; i++)
    {
        Sensor* s = sensors[i];
        s->writeOutput(w, true);
    }

    w.tagClose();
    w.tagClose();

    w.tagClose(); // PhysicalSystem
}

#endif
