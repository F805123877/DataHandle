//
//  ContainerWriteHandle.h
//  DataHandle
//
//  Created by Alkenso on 5/9/17.
//
//

#pragma once

#include <DataHandle/DataWriteHandle.h>

namespace datarw
{
    template <typename Container>
    class ContainerWriteHandle : public DataWriteHandle
    {
    public:
        ContainerWriteHandle();
        explicit ContainerWriteHandle(Container& container, const bool copyData = false);
        explicit ContainerWriteHandle(const Container& container);
        explicit ContainerWriteHandle(Container&& container);

        const Container& getContainer();

    private:
        virtual void insertDataImpl(const unsigned char* data, const datarw::Range& range);
        virtual uint64_t getDataSizeImpl();

    private:
        Container m_container;
        Container& m_containerRef;
    };
    
    template <typename Container>
    datarw::ContainerWriteHandle<Container>::ContainerWriteHandle()
    : m_container()
    , m_containerRef(m_container)
    {}
    
    template <typename Container>
    datarw::ContainerWriteHandle<Container>::ContainerWriteHandle(Container& container, const bool copyData /* = false */)
    : m_container(copyData ? container : Container())
    , m_containerRef(copyData ? m_container : container)
    {}
    
    template <typename Container>
    datarw::ContainerWriteHandle<Container>::ContainerWriteHandle(const Container& container)
    : m_container(container)
    , m_containerRef(m_container)
    {}
    
    template <typename Container>
    datarw::ContainerWriteHandle<Container>::ContainerWriteHandle(Container&& container)
    : m_container(container)
    , m_containerRef(m_container)
    {}
    
    template <typename Container>
    const Container& datarw::ContainerWriteHandle<Container>::getContainer()
    {
        return m_containerRef;
    }
    
    template <typename Container>
    void datarw::ContainerWriteHandle<Container>::insertDataImpl(const unsigned char* data, const datarw::Range& range)
    {
        m_containerRef.resize(std::max(static_cast<size_t>(range.position + range.length), m_containerRef.size()));
        std::copy(data, data + static_cast<size_t>(range.length), m_containerRef.begin() + static_cast<size_t>(range.position));
    }
    
    template <typename Container>
    uint64_t datarw::ContainerWriteHandle<Container>::getDataSizeImpl()
    {
        return m_containerRef.size();
    }
    
    using VectorWriteHandle = ContainerWriteHandle<datarw::ByteBuffer>;
    using StringWriteHandle = ContainerWriteHandle<std::string>;
}