//
//  DataTypes.h
//  DataHandle
//
//  Created by Alkenso on 5/9/17.
//
//

#pragma once

#include <type_traits>
#include <vector>

#define IS_BIG_ENDIAN (*(uint16_t *)"\0\xff" < 0x100)

#define TYPE_INTEGRAL(ValueType) typename ValueType, typename = datarw::IsIntegral<ValueType>
#define TYPE_INTEGRAL_DEFAULT(ValueType, DefaultType) typename ValueType = DefaultType, typename = datarw::IsIntegral<ValueType>
#define TYPE_INTEGRAL_IMPL(ValueType) typename ValueType, typename

#define TYPE_RAW_BYTES(DataType) typename DataType, typename = datarw::IsByte<DataType>
#define TYPE_RAW_BYTES_IMPL(DataType) typename DataType, typename

#define TYPE_RAW_BYTE_BUFFER(DataType) TYPE_RAW_BYTES(DataType), TYPE_INTEGRAL(DataType##Size)
#define TYPE_RAW_BYTE_BUFFER_IMPL(DataType) TYPE_RAW_BYTES_IMPL(DataType), TYPE_INTEGRAL_IMPL(DataType##Size)

#define TYPE_BYTE_BUFFER(BufferType) typename BufferType, typename = datarw::IsByteBuffer<BufferType>
#define TYPE_BYTE_BUFFER_IMPL(BufferType) typename BufferType, typename

namespace datarw
{
    using ByteBuffer = std::vector<unsigned char>;
    using CharBuffer = std::vector<char>;
    
    template <typename T>
    struct IsByte
    {
        static const bool value =
        std::is_same<T, char>::value ||
        std::is_same<T, signed char>::value ||
#if __cplusplus > 201402L
        std::is_same<T, std::byte>::value ||
#endif
        std::is_same<T, unsigned char>::value;
    };

    template <typename T>
    using IsVector = std::is_same<T, std::vector<typename T::value_type, typename T::allocator_type>>;

    template <typename T>
    struct IsContiguousContainer
    {
        static const bool value =
        IsVector<T>::value ||
        std::is_same<T, std::string>::value;
    };
    
    template <typename T>
    using IsByteBuffer = typename std::enable_if<IsByte<typename T::value_type>::value && IsContiguousContainer<T>::value>::type;
    
    template <typename T>
    using IsIntegral = typename std::enable_if<std::is_integral<T>::value>::type;
    
    struct Range
    {
        uint64_t position;
        uint64_t length;
        
        Range(uint64_t _position, uint64_t _length) : position(_position), length(_length) {}
        Range() : position(0), length(0) {}
    };
}
