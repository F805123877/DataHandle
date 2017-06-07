/*
 * Copyright (c) 2017 Alkenso (Vashurkin Vladimir)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO
 * THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#pragma once

template <TYPE_RAW_BYTES_IMPL(Data)>
void datarw::DataReadHandle::peekData(const Range& range, Data* data)
{
    peekDataInternal(range, reinterpret_cast<unsigned char*>(data), false);
}

template <TYPE_BYTE_BUFFER_IMPL(Buffer)>
void datarw::DataReadHandle::peekData(const Range& range, Buffer& buffer)
{
    buffer.resize(static_cast<size_t>(range.length));
    peekData(range, &buffer[0]);
}

template <TYPE_BYTE_BUFFER_IMPL(Buffer)>
void datarw::DataReadHandle::peekAppendData(const Range& range, Buffer& buffer)
{
    const size_t currentBufferSize = buffer.size();
    buffer.resize(static_cast<size_t>(currentBufferSize + range.length));
    peekData(range, &buffer[currentBufferSize]);
}

template <TYPE_BYTE_BUFFER_IMPL(Result)>
Result datarw::DataReadHandle::peekData(const Range& range)
{
    Result buffer;
    peekData(range, buffer);
    
    return buffer;
}

template <TYPE_RAW_BYTES_IMPL(Data)>
void datarw::DataReadHandle::readData(const uint64_t dataSize, Data* data)
{
    readDataInternal(dataSize, reinterpret_cast<unsigned char*>(data));
}

template <TYPE_BYTE_BUFFER_IMPL(Buffer)>
void datarw::DataReadHandle::readData(const uint64_t dataSize, Buffer& buffer)
{
    buffer.resize(static_cast<size_t>(dataSize));
    readData(dataSize, &buffer[0]);
}

template <TYPE_BYTE_BUFFER_IMPL(Buffer)>
void datarw::DataReadHandle::appendData(const uint64_t dataSize, Buffer& buffer)
{
    const size_t currentBufferSize = buffer.size();
    buffer.resize(static_cast<size_t>(currentBufferSize + dataSize));
    readData(dataSize, &buffer[currentBufferSize]);
}

template <TYPE_BYTE_BUFFER_IMPL(Result)>
Result datarw::DataReadHandle::readData(const uint64_t dataSize)
{
    Result buffer;
    readData(dataSize, buffer);
    
    return buffer;
}

template <TYPE_RAW_BYTES_IMPL(Data)>
void datarw::DataReadHandle::readAllData(Data* data)
{
    peekData(datarw::Range(0, getDataSize()), data);
}

template <TYPE_BYTE_BUFFER_IMPL(Buffer)>
void datarw::DataReadHandle::readAllData(Buffer& buffer)
{
    buffer.resize(getDataSize());
    readAllData(&buffer[0]);
}

template <TYPE_BYTE_BUFFER_IMPL(Buffer)>
void datarw::DataReadHandle::appendAllData(Buffer& buffer)
{
    const size_t currentBufferSize = buffer.size();
    buffer.resize(static_cast<size_t>(currentBufferSize + getDataSize()));
    readAllData(&buffer[currentBufferSize]);
}

template <TYPE_BYTE_BUFFER_IMPL(Result)>
Result datarw::DataReadHandle::readAllData()
{
    Result buffer;
    readAllData(buffer);
    
    return buffer;
}

template<typename T>
T datarw::DataReadHandle::peekValueLE(int64_t offset)
{
    return peekValue<T>(offset, IS_BIG_ENDIAN);
}

template<typename T>
T datarw::DataReadHandle::peekValueFromCurrentLE(int64_t offset)
{
    return peekValueFromCurrent<T>(offset, IS_BIG_ENDIAN);
}

template<typename T>
T datarw::DataReadHandle::readValueLE()
{
    return readValue<T>(IS_BIG_ENDIAN);
}

template<typename T>
T datarw::DataReadHandle::peekValueBE(int64_t offset)
{
    return peekValue<T>(offset, !IS_BIG_ENDIAN);
}

template<typename T>
T datarw::DataReadHandle::peekValueFromCurrentBE(int64_t offset)
{
    return peekValueFromCurrent<T>(offset, !IS_BIG_ENDIAN);
}

template<typename T>
T datarw::DataReadHandle::readValueBE()
{
    return readValue<T>(!IS_BIG_ENDIAN);
}

template<typename T>
T datarw::DataReadHandle::peekValue(int64_t offset, const bool reverseByteOrder)
{
    T value;
    memset(&value, 0, sizeof(T));
    peekData(Range(offset, sizeof(T)), reinterpret_cast<unsigned char*>(&value));
    
    return reverseByteOrder ? utils::ReverseValueByteOrder<T>(value) : value;
}

template<typename T>
T datarw::DataReadHandle::peekValueFromCurrent(int64_t offset, const bool reverseByteOrder)
{
    return peekValue<T>(tellPosition() + offset, reverseByteOrder);
}

template<typename T>
T datarw::DataReadHandle::readValue(const bool reverseByteOrder)
{
    T value;
    memset(&value, 0, sizeof(T));
    readData(sizeof(T), reinterpret_cast<unsigned char*>(&value));
    
    return reverseByteOrder ? utils::ReverseValueByteOrder<T>(value) : value;
}
