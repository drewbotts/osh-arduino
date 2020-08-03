
#ifndef OSH_NOMETADATA

#include "OSHClient.h"

namespace osh
{

/**
 * A bunch of strings stored in flash so we don't use up all the RAM!
 */
const char XMLNS_PREFIX[] PROGMEM = "xmlns";
const char SML_PREFIX[] PROGMEM = "";
const char GML_PREFIX[] PROGMEM = "g";
const char SWE_PREFIX[] PROGMEM = "s";
const char XLINK_PREFIX[] PROGMEM = "x";

#ifndef OSH_NONAMESPACE
const char OGC_NS_PREFIX[] PROGMEM = "http://www.opengis.net/def/";
const char SML_NS[] PROGMEM = "sensorml/2.0";
const char SWE_NS[] PROGMEM = "swe/2.0";
const char GML_NS[] PROGMEM = "gml/3.2";
const char XLINK_NS[] PROGMEM = "http://www.w3.org/1999/xlink";
#endif

const char TIME[] PROGMEM = "Time";
const char QUANTITY[] PROGMEM = "Quantity";
const char COUNT[] PROGMEM = "Count";
const char CATEGORY[] PROGMEM = "Category";
const char BOOLEAN[] PROGMEM = "Boolean";
const char TEXT[] PROGMEM = "Text";

const char ATT_ID[] PROGMEM = "id";
const char ATT_NAME[] PROGMEM = "name";
const char ATT_DEFINITION[] PROGMEM = "definition";
const char ATT_REFRAME[] PROGMEM = "referenceFrame";
const char ATT_AXISID[] PROGMEM = "axisID";

const char ELT_COMPONENT[] PROGMEM = "PhysicalComponent";
const char ELT_SYSTEM[] PROGMEM = "PhysicalSystem";
const char ELT_IDENTIFIER[] PROGMEM = "identifier";
const char ELT_DESCRIPTION[] PROGMEM = "description";
const char ELT_OUTPUTS[] PROGMEM = "outputs";
const char ELT_OUTPUTLIST[] PROGMEM = "OutputList";
const char ELT_OUTPUT[] PROGMEM = "output";
const char ELT_DATARECORD[] PROGMEM = "DataRecord";
const char ELT_VECTOR[] PROGMEM = "Vector";
const char ELT_FIELD[] PROGMEM = "field";
const char ELT_COORDINATE[] PROGMEM = "coordinate";
const char ELT_LABEL[] PROGMEM = "label";
const char ELT_UOM[] PROGMEM = "uom";
const char ATT_HREF[] PROGMEM = "href";
const char ATT_CODE[] PROGMEM = "code";

const char HTTP_PREFIX[] PROGMEM = "http://";
const char OGC_DEF_PREFIX[] PROGMEM = "";
const char DEF_SAMPLING_TIME[] PROGMEM = "http://www.opengis.net/def/property/OGC/0/SamplingTime";
const char DEF_ISO8601[] PROGMEM = "http://www.opengis.net/def/uom/ISO-8601/0/Gregorian";
const char DEF_UTC[] PROGMEM = "http://www.opengis.net/def/trs/BIPM/0/UTC";
const char DEF_EPSG4326[] PROGMEM = "http://www.opengis.net/def/crs/EPSG/0/4326";
const char DEF_EPSG4979[] PROGMEM = "http://www.opengis.net/def/crs/EPSG/0/4979";
const char DEF_NED[] PROGMEM = "http://www.opengis.net/def/cs/OGC/0/NED";
const char DEF_HEADING[] PROGMEM = "http://sensorml.com/ont/swe/property/TrueHeading";
const char DEF_PITCH[] PROGMEM = "http://sensorml.com/ont/swe/property/PitchAngle";
const char DEF_ROLL[] PROGMEM = "http://sensorml.com/ont/swe/property/RollAngle";
const char DEF_LAT[] PROGMEM = "http://www.opengis.net/def/axis-name/EPSG/0/9901";
const char DEF_LON[] PROGMEM = "http://www.opengis.net/def/axis-name/EPSG/0/9902";
const char DEF_ACCEL[] PROGMEM = "http://qudt.org/vocab/quantitykind/LinearAcceleration";
const char DEF_ALT[] PROGMEM = "http://mmisw.org/ont/cf/parameter/altitude";




} // namespace osh

#endif // OSH_NOMETADATA

