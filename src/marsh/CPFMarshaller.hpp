/***************************************************************************
  tag: Peter Soetens  Mon Jan 19 14:11:20 CET 2004  CPFMarshaller.hpp 

                        CPFMarshaller.hpp -  description
                           -------------------
    begin                : Mon January 19 2004
    copyright            : (C) 2004 Peter Soetens
    email                : peter.soetens@mech.kuleuven.ac.be
 
 ***************************************************************************
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the GNU Lesser General Public            *
 *   License as published by the Free Software Foundation; either          *
 *   version 2.1 of the License, or (at your option) any later version.    *
 *                                                                         *
 *   This library is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
 *   Lesser General Public License for more details.                       *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this library; if not, write to the Free Software   *
 *   Foundation, Inc., 59 Temple Place,                                    *
 *   Suite 330, Boston, MA  02111-1307  USA                                *
 *                                                                         *
 ***************************************************************************/ 
 
#ifndef PI_PROPERTIES_CPFSERIALIZER
#define PI_PROPERTIES_CPFSERIALIZER

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <Property.hpp>
#include <PropertyIntrospection.hpp>
#include "StreamProcessor.hpp"

namespace RTT
{
    /**
     * A class for marshalling a property or propertybag into a
     * component property description, following the CORBA 3 standard.
     * @see CPFDemarshaller for reading the result back in.
     */
    template<typename output_stream>
    class CPFMarshaller 
        : public Marshaller, protected PropertyIntrospection, public StreamProcessor<output_stream>
    {
        /**
         * Write-out formatting of a property.
         */
        template<class T>
        void doWrite( const Property<T> &v, const std::string& type )
        { 
            *(this->s)<<indent << "<simple name=\"" << this->escape( v.getName() ) << "\" type=\""<< type <<"\">";
            if ( !v.getDescription().empty() )
                *(this->s) << "<description>"<< this->escape( v.getDescription() ) << "</description>";
            *(this->s) << "<value>" << v.get() << "</value></simple>\n";
        }

        /**
         * Specialisation in case of a string.
         */
        void doWrite( const Property<std::string> &v, const std::string& type )
        { 
            *(this->s)<<indent << "<simple name=\"" << this->escape( v.getName() ) << "\" type=\""<< type <<"\">";
            if ( !v.getDescription().empty() )
                *(this->s) << "<description>"<< this->escape( v.getDescription() ) << "</description>";
            *(this->s) << "<value>" << this->escape( v.get() ) << "</value></simple>\n";
        }

        std::string indent;

        std::string escape(std::string s)
        {
            std::string::size_type n=0;
            // replace amps first.
            while ((n = s.find("&",n)) != s.npos) {
                s.replace(n, 1, std::string("&amp;"));
                n += 5;
            }

            n=0;
            while ((n = s.find("<",n)) != s.npos) {
                s.replace(n, 1, std::string("&lt;"));
                n += 4;
            }

            n=0;
            while ((n = s.find(">",n)) != s.npos) {
                s.replace(n, 1, std::string("&gt;"));
                n += 4;
            }

            // TODO: Added escapes for other XML entities
            return s;
        }

        virtual void introspect(PropertyBase* pb)
        {
            PropertyIntrospection::introspect( pb );
        }

        virtual void introspect(Property<bool> &v) 
        { 
            doWrite( v, "boolean");
        }

        virtual void introspect(Property<char> &v) 
        { 
            doWrite( v, "char");
        }

        virtual void introspect(Property<int> &v) 
        { 
            doWrite( v, "long");
        }
			
        virtual void introspect(Property<unsigned int> &v) 
        { 
            doWrite( v, "ulong");
        }
			
        virtual void introspect(Property<double> &v) 
        {
            doWrite( v, "double");
        }

        virtual void introspect(Property<std::string> &v) 
        {
            doWrite( v, "string");
        }
			
        virtual void introspect(Property<PropertyBag> &b) 
        {
            PropertyBag v = b.get();
            *(this->s) <<indent<<"<struct name=\""<<escape(b.getName())<<"\" type=\""<< escape(v.getType())<< "\">\n";
            indent +="   ";
            if ( !b.getDescription().empty() )
                *(this->s) <<indent<<"<description>"  <<escape(b.getDescription()) << "</description>\n";

            b.value().identify(this);

            indent = indent.substr(0, indent.length()-3); 
            *(this->s) <<indent<<"</struct>\n";
        }

    public:
        /**
         * Construct a CPFMarshaller.
         */
        CPFMarshaller(output_stream &os) :
            StreamProcessor<output_stream>(os), indent("  ")
        {
        }

        virtual void serialize(PropertyBase* v)
        {
            v->identify( this );
        }

        virtual void serialize(const PropertyBag &v) 
        {
            *(this->s) <<"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                       <<"<!DOCTYPE properties SYSTEM \"cpf.dtd\">\n";
            *(this->s) <<"<properties>\n";

            v.identify(this);

            *(this->s) << "</properties>\n";
        }

        virtual void flush()
        {
            this->s->flush();
        }
            
	};
}
#endif
