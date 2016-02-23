#ifndef LibOSH_NOMETADATA

#include "LibOSH.h"

namespace osh
{


#ifndef LibOSH_NOXML

void Device::toXML(Print& out)
{
    XMLWriter w(&out);
    w.header();
    writeXML(w, false);
}

void Device::writeNamespaces(XMLWriter& w)
{
#ifndef XMLWRITER_NONAMESPACE
    char nsUri[40];

    buildUrl(OGC_NS_PREFIX, SML_NS, nsUri);
    char tagName[6];
    strcpy_P(tagName, XMLNS_PREFIX);
    w.tagField(tagName, nsUri);

    buildUrl(OGC_NS_PREFIX, SWE_NS, nsUri);
    w.tagField(w.buildTagName(XMLNS_PREFIX, SWE_PREFIX), nsUri);

    buildUrl(OGC_NS_PREFIX, GML_NS, nsUri);
    w.tagField(w.buildTagName(XMLNS_PREFIX, GML_PREFIX), nsUri);

    strcpy_P(nsUri, XLINK_NS);
    w.tagField(w.buildTagName(XMLNS_PREFIX, XLINK_PREFIX), nsUri);
#endif
}

#endif // LibOSH_NOXML



#ifndef LibOSH_NOJSON

void Device::toJSON(Print& out)
{
    // TODO
}

#endif // LibOSH_NOJSON


} // namespace osh

#endif // LibOSH_NOMETADATA
