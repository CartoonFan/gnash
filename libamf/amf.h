// 
//   Copyright (C) 2005, 2006, 2007, 2008 Free Software Foundation, Inc.
// 
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

// This file is for the low level support for encoding and decoding AMF objects.
// As this class has no data associated with it, all the methods are static as
// they are for convenience only.
// All the encoding methods return a Buffer class, which is simply an array on
// of unsigned bytes, and a byte count.
// The only extraction classes parse either a raw AMF object or the larger
// "variable"

#ifndef _AMF_H_
#define _AMF_H_

#ifdef HAVE_CONFIG_H
#include "gnashconfig.h"
#endif

#include <vector>
#include <string>
#include <cstring>
#include <map>
#include <boost/cstdint.hpp>
#include <boost/shared_ptr.hpp>

#include "network.h"
#include "element.h"
#include "dsodefs.h"

/// \namespace amf
///
/// This namespace is for all the AMF specific classes in libamf.
namespace amf 
{

// forward declaration
class Buffer;

/// All numbers in AMF format are 8 byte doubles.
const size_t AMF0_NUMBER_SIZE = 0x08;

/// \brief The header size in bytes of an common AMF object.
///	The size of an AMF header is a type field (1 byte), followed by a
///	length field. (short)
const gnash::Network::byte_t AMF_HEADER_SIZE = 3;

/// \brief  The header size of a property.
///	A property is a little different. It always assumes the the
///	first field is a string that's the property name, then the
///	type byte like a regular AMF object and length is used for the
///	data. So a property object header is then only 5 bytes instead
///	of the 6 that one assumes would be used.
const gnash::Network::byte_t AMF_PROP_HEADER_SIZE = 5;

/// AMF version 0 is supported by default
const gnash::Network::byte_t AMF_VERSION = 0;

/// For terminating sequences, a byte with value 0x09 is used.
const gnash::Network::byte_t TERMINATOR = 0x09;

/// \brief The maximum size for a string.
/// As if there is a parsing error, we'll often see the symptom of the length
/// for the following value is bogus. Although the length field is a short, it
/// seems silly to assume we'll ever see a string 65,000 characters long. Still,
/// it makes sense to make this an adjustable thing.
const int SANE_STR_SIZE = 1024;

/// \class AMF
///
//// An AMF object is the binary representation of an ActionScript object. AMF
/// is used to send objects, wheather to a SharedObject .sol file, a memory based
/// LocalConnection segment, or over an RTMP connection for streaming.
class DSOEXPORT AMF {
public:
    /// \enum AMF::shared_obj_types_e
    ///		This is the types of SharedObjects that can be
    ///		serialized or deserialized.
    typedef enum {
        CONNECT = 0x01,
        DISCONNECT = 0x02,
        SET_ATTRIBUTE = 0x03,
        UPDATE_DATA = 0x04,
        UPDATE_ATTRIBUTE = 0x05,
        SEND_MESSAGE = 0x06,
        STATUS = 0x07,
        CLEAR_DATA = 0x08,
        DELETE_DATA = 0x09,
        DELETE_ATTRIBYTE = 0x0a,
        INITIAL_DATA = 0x0b
    } shared_obj_types_e;
    /// \enum AMF::filetype_e
    ///		This is the type of file being streamed.
    typedef enum {
	FILETYPE_ERROR = -1,
	FILETYPE_NONE = 0,
	FILETYPE_HTML,
	FILETYPE_SWF,
	FILETYPE_VIDEO,
	FILETYPE_AUDIO,
	FILETYPE_MP3,
	FILETYPE_FCS,
	FILETYPE_OSCP
    } filetype_e;

    /// \brief Create a new AMF class.
    ///		As most of the methods in the AMF class a static, this
    ///		is primarily only used when encoding complex objects
    ///		where the byte count is accumulated.
    AMF();
    /// Delete the alloczted AMF class
    ~AMF();

    /// @remarks 
    ///		Methods for encoding data into big endian formatted
    ///		raw AMF data. Note that while we could have had a
    ///		single overloaded encode method, this is more
    ///		explicit, which when it comes to manipulating binary
    ///		protocols make the code much more readable.
    
    /// \brief Encode a string object to it's serialized representation.
    /// 
    /// @param str a string value
    ///
    /// @return a binary AMF packet in big endian format
    static boost::shared_ptr<Buffer> encodeString(const std::string &str);
    
    /// \brief Encode an array of ASCII bytes to it's serialized representation.
    /// 
    /// @param data The data to serialize into big endian format
    /// 
    /// @param size The size of the data in bytes
    ///
    /// @return a binary AMF packet in big endian format
    static boost::shared_ptr<Buffer> encodeString(gnash::Network::byte_t *data, size_t size);

    /// \brief Encode a String object to it's serialized representation.
    ///		A NULL String is a string with no associated data.
    ///
    /// @return a binary AMF packet in big endian format
    static boost::shared_ptr<Buffer> encodeNullString();

    /// \brief Encode a Boolean object to it's serialized representation.
    ///
    /// @param flag The boolean value to serialize.
    ///
    /// @return a binary AMF packet in big endian format
    static boost::shared_ptr<Buffer> encodeBoolean(bool flag);

    /// \brief Encode an "Undefined" object to it's serialized representation.
    ///
    /// @return a binary AMF packet in big endian format
    static boost::shared_ptr<Buffer> encodeUndefined();

    /// \brief Encode a NULL object to it's serialized representation.
    ///		A NULL object is often used as a placeholder in RTMP.
    ///
    /// @return a binary AMF packet in big endian format
    static boost::shared_ptr<Buffer> encodeNull();

    /// \brief Encode a "Unsupported" object to it's serialized representation.
    ///
    /// @return a binary AMF packet in big endian format
    static  boost::shared_ptr<Buffer> encodeUnsupported();

    /// \brief Encode an XML object to it's serialized representation.
    ///
    /// @param data A pointer to the raw bytes that becomes the XML data.
    /// 
    /// @param nbytes The number of bytes to serialize.
    ///
    /// @return a binary AMF packet in big endian format
    static boost::shared_ptr<Buffer> encodeXMLObject(gnash::Network::byte_t *data, size_t size);

    /// \brief Encode a Typed Object to it's serialized representation.
    ///
    /// @param data A pointer to the raw bytes that becomes the data.
    /// 
    /// @param size The number of bytes to serialize.
    ///
    /// @return a binary AMF packet in big endian format
    static boost::shared_ptr<Buffer> encodeTypedObject(gnash::Network::byte_t *data, size_t size);

    /// \brief Encode a Reference to an object to it's serialized representation.
    ///
    /// @param data A pointer to the raw bytes that becomes the data.
    /// 
    /// @param size The number of bytes to serialize.
    ///
    /// @return a binary AMF packet in big endian format (header,data)
    static boost::shared_ptr<Buffer> encodeReference(gnash::Network::byte_t *data, size_t size);

    /// \brief Encode a Movie Clip (swf data) to it's serialized representation.
    ///
    /// @param data A pointer to the raw bytes that becomes the data.
    /// 
    /// @param size The number of bytes to serialize.
    ///
    /// @return a binary AMF packet in big endian format (header,data)
    static boost::shared_ptr<Buffer> encodeMovieClip(gnash::Network::byte_t *data, size_t size);

    /// \brief Encode an ECMA Array to it's serialized representation.
    ///		An ECMA Array, also called a Mixed Array, contains any
    ///		AMF data type as an item in the array.
    ///
    /// @param data A pointer to the raw bytes that becomes the data.
    /// 
    /// @param size The number of bytes to serialize.
    ///
    /// @return a binary AMF packet in big endian format
    static boost::shared_ptr<Buffer> encodeECMAArray(gnash::Network::byte_t *data, size_t size);

    /// \brief Encode a Long String to it's serialized representation.
    ///
    /// @param data A pointer to the raw bytes that becomes the data.
    /// 
    /// @param size The number of bytes to serialize.
    ///
    /// @return a binary AMF packet in big endian format
    static boost::shared_ptr<Buffer> encodeLongString(gnash::Network::byte_t *data, size_t size);

    /// \brief Encode a Record Set to it's serialized representation.
    ///
    /// @param data A pointer to the raw bytes that becomes the data.
    /// 
    /// @param size The number of bytes to serialize.
    ///
    /// @return a binary AMF packet in big endian format
    static boost::shared_ptr<Buffer> encodeRecordSet(gnash::Network::byte_t *data, size_t size);

    /// \brief Encode a Date to it's serialized representation.
    ///
    /// @param data A pointer to the raw bytes that becomes the data.
    /// 
    /// @return a binary AMF packet in big endian format
    static boost::shared_ptr<Buffer> encodeDate(gnash::Network::byte_t *data);

    /// \brief Encode a Strict Array to it's serialized representation.
    ///		A Strict Array is one where all the items are the same
    ///		data type, commonly either a number or a string.
    ///
    /// @param data A pointer to the raw bytes that becomes the data.
    /// 
    /// @param size The number of bytes to serialize.
    ///
    /// @return a binary AMF packet in big endian format (header,data)
    static boost::shared_ptr<Buffer> encodeStrictArray(gnash::Network::byte_t *data, size_t size);
    
    /// \brief Encode an object to it's serialized representation.
    ///
    /// @param el A smart pointer to an Element class.
    /// 
    /// @return a binary AMF packet in big endian format
    static boost::shared_ptr<Buffer> encodeObject(boost::shared_ptr<amf::Element> el);

    /// \brief Encode the end of an object to it's serialized representation.
    ///
    /// @return a binary AMF packet in big endian format
    static boost::shared_ptr<Buffer> encodeObjectEnd();

    /// \brief Encode a 64 bit number to it's serialized representation.
    ///
    /// @param num A double value to serialize.
    ///
    /// @return a binary AMF packet in big endian format
    static boost::shared_ptr<Buffer> encodeNumber(double num);

    /// \brief Encode an Element to it's serialized representation.
    ///
    /// @param el A smart pointer to the Element to encode.
    ///
    /// @return a binary AMF packet in big endian format
    static boost::shared_ptr<Buffer> encodeElement(boost::shared_ptr<amf::Element> el);

    /// Encode a variable to it's serialized representation.
    ///
    /// @param el A smart pointer to the Element to encode.
    ///
    /// @return a binary AMF packet in big endian format
    boost::shared_ptr<Buffer> encodeProperty(boost::shared_ptr<amf::Element> el);

    //
    // Methods for extracting data from big endian formatted raw AMF data.
    //

    /// \brief Extract the AMF0 object type from the header.
    ///
    /// @param in The raw data to extract values from.
    ///
    /// @return The data type from the header
    static Element::amf0_type_e extractElementHeader(gnash::Network::byte_t *in)
                         { return *(reinterpret_cast<Element::amf0_type_e *>(in)); };

    /// \brief Extract an AMF object from an array of raw bytes.
    ///		An AMF object is one of the support data types.
    ///
    /// @param in A real pointer to the raw data to start parsing from.
    ///
    /// @param tooFar A pointer to one-byte-past the last valid memory
    ///		address within the buffer.
    ///
    /// @return A smart ptr to an Element.
    ///
    /// @remarks May throw a ParserException
    boost::shared_ptr<amf::Element> extractAMF(gnash::Network::byte_t *in, gnash::Network::byte_t* tooFar);

    /// \brief Extract an AMF object from an array of raw bytes.
    ///
    /// @param buf A smart pointer to a Buffer to parse the data from.
    ///
    /// @return A smart ptr to an Element.
    ///
    /// @remarks May throw a ParserException
    boost::shared_ptr<amf::Element> extractAMF(boost::shared_ptr<Buffer> buf);
    
    /// \brief Extract a Property.
    ///		A Property is a standard AMF object preceeded by a
    ///		length and an ASCII name field. These are only used
    ///		with higher level ActionScript objects.
    ///
    /// @param in A real pointer to the raw data to start parsing from.
    ///
    /// @param tooFar A pointer to one-byte-past the last valid memory
    ///		address within the buffer.
    ///
    /// @return A smart ptr to an Element.
    ///
    /// @remarks May throw a ParserException
    boost::shared_ptr<amf::Element> extractProperty(gnash::Network::byte_t *in, gnash::Network::byte_t* tooFar);

    /// \brief Extract a Property.
    ///		A Property is a standard AMF object preceeded by a
    ///		length and an ASCII name field. These are only used
    ///		with higher level ActionScript objects.
    ///
    /// @param buf A smart pointer to an Buffer to parse the data from.
    ///
    /// @return A smart ptr to an Element.
    ///
    /// @remarks May throw a ParserException
    boost::shared_ptr<amf::Element> extractProperty(boost::shared_ptr<Buffer> buf);

    /// \brief Get the total number of allocated bytes used when serializing.
    ///
    /// @return The total allocated bytes.
    size_t totalsize() { return _totalsize; }
    
private:
    /// \var _totalsize
    ///
    /// The total number of bytes in serialized ActionScript object.
    size_t _totalsize;
};

/// \brief Swap bytes in raw data.
///	This only swaps bytes if the host byte order is little endian.
///
/// @param word The address of the data to byte swap.
///
/// @param size The number of bytes in the data.
///
/// @return A pointer to the raw data.
DSOEXPORT void *swapBytes(void *word, size_t size);


} // end of amf namespace

// end of _AMF_H_
#endif

// local Variables:
// mode: C++
// indent-tabs-mode: t
// End:
