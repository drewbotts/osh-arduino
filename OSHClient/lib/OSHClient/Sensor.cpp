
#ifndef OSH_NOMETADATA

#include "OSHClient.h"

namespace osh
{

void Sensor::addTimeStampUTC()
{
    addTimeStamp(DEF_ISO8601, DEF_UTC);
}


void Sensor::addTimeStampOBC(const char* uom)
{
    addTimeStamp(uom, 0);
}


void Sensor::addTimeStamp(const char* uom, const char* refFrame)
{
    Measurement* m = new Measurement();
    m->setName("time");
    m->setLabel("Sampling Time");
    m->setDefinition(DEF_SAMPLING_TIME);
    m->setUom(uom);
    m->setRefFrame(refFrame);
    m->setType(TIME);
    addMeasurement(m);
}


void Sensor::addMeasurement(Measurement* meas)
{
    this->outputs[numOutputs++] = meas;
}


void Sensor::addMeasurement(const char* name, const char* def, const char* uom, const char* label, const char* type)
{
    Measurement* output = new Measurement();
    output->setName(name);
    output->setType((type != NULL) ? type : QUANTITY);
    output->setDefinition(def);
    output->setLabel(label);
    output->setUom(uom);
    this->outputs[numOutputs++] = output;
}


void Sensor::addLocationLLA(const char* def, const char* label)
{
    VectorMeas* v = new VectorMeas();
    v->setName("loc");
    v->setDefinition(def);
    v->setLabel(label);
    v->setRefFrame(DEF_EPSG4979);
    v->addCoordinate("Lat", "deg", "Geodetic Latitude", NULL, DEF_LAT);
    v->addCoordinate("Long", "deg", "Longitude", NULL, DEF_LON);
    v->addCoordinate("h", "m", "Altitude", NULL, DEF_ALT);
    this->outputs[numOutputs++] = v;
}

void Sensor::addOrientationEuler(const char* def, const char* label)
{
    VectorMeas* v = new VectorMeas();
    v->setName("orientation");
    v->setDefinition(def);
    v->setLabel(label);
    v->setRefFrame(DEF_NED);
    v->addCoordinate("Z", "deg", "Heading Angle", NULL, DEF_HEADING);
    v->addCoordinate("Y", "deg", "Pitch Angle", NULL, DEF_PITCH);
    v->addCoordinate("X", "deg", "Roll Angle", NULL, DEF_ROLL);
    this->outputs[numOutputs++] = v;
}

void Sensor::addAccel(const char* def, const char* label)
{
    VectorMeas* v = new VectorMeas();
    v->setDefinition(def);
    v->setLabel(label);
    v->setRefFrame("#PLATFORM_FRAME");
    v->addCoordinate("X", "m/s2", "X Accel", NULL, DEF_ACCEL);
    v->addCoordinate("Y", "m/s2", "Y Accel", NULL, DEF_ACCEL);
    v->addCoordinate("Z", "m/s2", "Z Accel", NULL, DEF_ACCEL);
    this->outputs[numOutputs++] = v;
}

void Sensor::addMag(const char* def, const char* label)
{
    VectorMeas* v = new VectorMeas();
    v->setDefinition(def);
    v->setLabel(label);
    v->setRefFrame("#PLATFORM_FRAME");
    v->addCoordinate("X", "uT", "X Mag", NULL, DEF_MAG);
    v->addCoordinate("Y", "uT", "Y Mag", NULL, DEF_MAG);
    v->addCoordinate("Z", "uT", "Z Mag", NULL, DEF_MAG);
    this->outputs[numOutputs++] = v;
}


#ifndef OSH_NOXML

void Sensor::writeXML(XMLWriter& w, bool nested)
{
    // PhysicalComponent
    w.tagStart(w.buildTagName(SML_PREFIX, ELT_COMPONENT));

    // id
    w.tagField(w.buildTagName(GML_PREFIX, ATT_ID), "S01");

    // namespace decl
    if (!nested)
        writeNamespaces(w);
    w.tagEnd(true, false);

    // description
    if (this->Description != NULL)
        w.writeNode(w.buildTagName(GML_PREFIX, ELT_DESCRIPTION), this->Description);

    // identifier
    if (this->UniqueID != NULL)
        w.writeNode(w.buildTagName(GML_PREFIX, ELT_IDENTIFIER), this->UniqueID);

    // name
    if (this->Name != NULL)
        w.writeNode(w.buildTagName(GML_PREFIX, ATT_NAME), this->Name);

    // outputs
    if (this->numOutputs > 0)
    {
        w.tagOpen(w.buildTagName(SML_PREFIX, ELT_OUTPUTS));
        w.tagOpen(w.buildTagName(SML_PREFIX, ELT_OUTPUTLIST));
        w.tagStart(w.buildTagName(SML_PREFIX, ELT_OUTPUT));
        w.tagField(w.buildTagName(NULL, ATT_NAME), "out");
        w.tagEnd(true, false);
        writeOutput(w);
        w.tagClose();
        w.tagClose();
        w.tagClose();
    }

    w.tagClose(); // PhysicalComponent
}


void Sensor::writeOutput(XMLWriter& w)
{
    if (numOutputs == 1)
    {
        outputs[0]->writeXML(w);
    }
    else
    {
        w.tagStart(w.buildTagName(SWE_PREFIX, ELT_DATARECORD));
        w.tagField(w.buildTagName(NULL, ATT_DEFINITION), this->DataRecordDef);
        w.tagEnd(true, false);
        if (this->Label != NULL)
        {
            w.writeNode(w.buildTagName(SWE_PREFIX, ELT_LABEL), this->Label);
        }

        for (int i = 0; i < numOutputs; i++)
        {
            Measurement* m = outputs[i];
            w.tagStart(w.buildTagName(SWE_PREFIX, ELT_FIELD));
            w.tagField(w.buildTagName(NULL, ATT_NAME), m->getName());
            w.tagEnd(true, false);
            m->writeXML(w);
            w.tagClose();
        }

        w.tagClose();
    }
}

#endif // OSH_NOXML


Sensor::~Sensor()
{
    for (int i = 0; i < numOutputs; i++)
        delete outputs[i];
}


} // namespace osh

#endif // OSH_NOMETADATA
