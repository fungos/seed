#include "api/net/Address.h"

namespace Seed { namespace Net {

Address::Address()
{
	address = 0;
	port = 0;
}

Address::Address(unsigned char a, unsigned char b, unsigned char c, unsigned char d, unsigned short port)
{
	this->address = ( a << 24 ) | ( b << 16 ) | ( c << 8 ) | d;
	this->port = port;
}

Address::Address(unsigned int address, unsigned short port)
{
	this->address = address;
	this->port = port;
}

unsigned int Address::GetAddress() const
{
	return address;
}

unsigned char Address::GetA() const
{
	return (unsigned char) (address >> 24);
}

unsigned char Address::GetB() const
{
	return (unsigned char) (address >> 16);
}

unsigned char Address::GetC() const
{
	return (unsigned char) (address >> 8);
}

unsigned char Address::GetD() const
{
	return (unsigned char) (address);
}

unsigned short Address::GetPort() const
{
	return port;
}

bool Address::operator == ( const Address & other ) const
{
	return address == other.address && port == other.port;
}

bool Address::operator != ( const Address & other ) const
{
	return ! ( *this == other );
}


}} // namespace


