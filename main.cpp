// MIT License
//
// Copyright (c) 2023 Thomas Strunz
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.


#define WIN32_LEAN_AND_MEAN
//#define NO_STRICT
#ifdef TEXT
#undef TEXT
#endif
#include "windows.h"

#undef  WIN32_LEAN_AND_MEAN

#include <iostream>
#include <string>
#include <array>

namespace
{
	void FormatLastError(wchar_t* errorBuffer, size_t size)
	{
		::FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, ::GetLastError(), 0, errorBuffer, size, nullptr);
	}

	bool SetTimeOff(int ms)
	{
		BOOL rc;
		SYSTEMTIME s;
		FILETIME f;
		ULARGE_INTEGER u;
		::GetSystemTime(&s);
		::SystemTimeToFileTime(&s, &f);
		memcpy(&u, &f, sizeof(u));
		u.QuadPart += ms * 10000;
		memcpy(&f, &u, sizeof(f));
		::FileTimeToSystemTime(&f, &s);
		rc = ::SetSystemTime(&s);
		return(rc != 0);
	}
}

int main(int argc, char* argv[])
{
	long offsetMs = 200;
	if (argc > 1)
	{
		char* p_end;
		const char* p = argv[1];
		std::cout << "Argument: " << p << std::endl;
		offsetMs = std::strtol(p, &p_end, 10);
		if (p == p_end)
		{
			std::cout << "Parsing failed: " << p_end << std::endl;
			offsetMs = 200;
		}
		std::cout << "Time offset: " << offsetMs << std::endl;
	}

	if (SetTimeOff(offsetMs))
	{
		std::cout << "Changed system time by " << offsetMs << " ms." << std::endl;
	}
	else
	{
		std::array<wchar_t, 4096> buffer;
		FormatLastError(buffer.data(), buffer.size());
		std::wcout << L"Failed to change system time: " << buffer.data() << std::endl;
	}
}

