// bstream_test.cpp
//
////////////////////////////////////////////////////////////////////////////////

#include "bstream_test.h"
#include <nucleon/bstream.h>

using namespace quark;
using namespace nucleon;

//////////////////////////////////////////////////////////////////////
// bstream_test
//////////////////////////////////////////////////////////////////////

const char* bstream_test::getName() const
{
	return "bstream_test";
}

void bstream_test::execute()
{
	// TODO: fix refactor bstream in order to support other types than u8
	// testWordStream(); 
	testByteStream();
}

void bstream_test::testByteStream()
{
	typedef fixed_streambuf<u8> bstreambuf_t;
	typedef bstream<bstreambuf_t> bstream_t;

	char strbuf[5] = "";
	bstreambuf_t buffer((u8*)strbuf, sizeof(strbuf));
	bstream_t stream(buffer);

	// write to stream
	stream << 'H' << 'e' << 'l' << 'l' << 'o';

	// reset position
	stream.rdbuf()->seekpos(0);

	// read from stream
	pstring str;
	char c;
	while (!stream.eof())
	{
		stream >> c;
		str += c;
	}

	_TEST_ASSERT(str == "Hello");
}

/*void bstream_test::testWordStream()
{
	typedef fixed_streambuf<u16> bstreambuf_t;
	typedef bstream<bstreambuf_t> bstream_t;

	wchar_t strbuf[5] = L"";
	bstreambuf_t buffer(strbuf, sizeof(strbuf));
	bstream_t stream(buffer);

	// write to stream
	stream << L'H' << L'e' << L'l' << L'l' << L'o';

	// reset position
	stream.rdbuf()->seekpos(0);

	// read from stream
	std::wstring str;
	wchar_t c;
	while (!stream.eof())
	{
		stream >> c;
		str += c;
	}

	_TEST_ASSERT(str == L"Hello");
}*/
