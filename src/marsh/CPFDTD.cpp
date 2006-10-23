
#include "marsh/CPFDTD.hpp"

namespace RTT
{
    namespace detail 
    {
        const char* cpf_dtd = 
        "<!ELEMENT properties (description? , (simple|sequence|struct)* )> \
<!ELEMENT choice (#PCDATA)> \
<!ELEMENT choices (choice+)> \
<!ELEMENT defaultvalue (#PCDATA)> \
<!ELEMENT description (#PCDATA)> \
<!ELEMENT simple (description?, value, choices?, defaultvalue?)> \
<!ATTLIST simple name CDATA #IMPLIED type (boolean|char|double|float|short|long|objref|octet|string|ulong|ushort) #REQUIRED> \
<!ELEMENT sequence (description?, (simple*|struct*|sequence*))> \
<!ATTLIST sequence name CDATA #IMPLIED type CDATA #REQUIRED> \
<!ELEMENT struct (description?, (simple|sequence|struct)*)> \
<!ATTLIST struct name CDATA #IMPLIED type CDATA #REQUIRED> \
<!ELEMENT value (#PCDATA)>";

    }
}
