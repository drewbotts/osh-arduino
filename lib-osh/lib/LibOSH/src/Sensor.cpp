#include "LibOSH.h"


void Sensor::addMeasurement(const char* name, const char* def, const char* uom, const char* label, const char* type)
{
    Measurement* output = new Measurement();
    output->setName(name);
    output->setType((type != NULL) ? type : Measurement::QUANTITY);
    output->setDefinition(def);
    output->setLabel(label);
    output->setUom(uom);
    this->outputs[numOutputs++] = output;
}


void Sensor::sendData(float* data, Print& out)
{
    int lastIndex = numOutputs-1;
    for (int i = 0; i < numOutputs; i++)
    {
        out.print(data[i]);
        if (i < lastIndex)
            out.print(',');
    }

    out.print('\r');
    out.print('\n');
}


#ifndef LibOSH_NOXML

void Sensor::writeXML(XMLWriter& w, bool nested)
{
    w.tagStart("PhysicalComponent");
    w.tagField("gml:id", "S01");
    if (!nested)
        writeNamespaces(w);
    w.tagEnd(true, false);

    w.writeNode("gml:description", this->Description);
    w.writeNode("gml:identifier", this->UniqueID);

    // output description
    w.tagOpen("outputs");
    w.tagOpen("OutputList");
    writeOutput(w, nested);
    w.tagClose();
    w.tagClose();

    w.tagClose(); // PhysicalComponent
}


void Sensor::writeOutput(XMLWriter& w, bool nested)
{
    w.tagStart("output");

    if (numOutputs == 1)
    {
        Measurement* m = outputs[0];
        w.tagField("name", m->getName());
        w.tagEnd(true, false);
        m->writeXML(w, nested);
    }
    else
    {
        w.tagField("name", "rec");
        w.tagEnd(true, false);

        w.tagOpen("swe:DataRecord");
        for (int i = 0; i < numOutputs; i++)
        {
            Measurement* m = outputs[i];

            w.tagStart("swe:field");
            w.tagField("name", m->getName());
            w.tagEnd(true, false);

            m->writeXML(w, nested);
            w.tagClose();
        }
        w.tagClose();
    }

    w.tagClose();
}


void Measurement::writeXML(XMLWriter& w, bool nested)
{
    w.tagStart(this->Type);
    w.tagField("definition", this->Definition);
    w.tagEnd(true, false);
    w.writeNode("swe:label", (this->Label != 0) ? this->Label : this->Name);
    w.tagStart("swe:uom");
    w.tagField("code", this->Uom);
    w.tagEnd(true, true);
    w.tagClose();
}

#endif
