#ifndef ALLOCATORS_H
#define ALLOCATORS_H




#ifdef __cplusplus


#include <cstddef>
#include <cstdlib>







namespace ELang {
namespace {



template<class Type>
inline constexpr Type* Alloc(size_t uWidth)
{
    return (Type*) malloc(uWidth);
}



template<class Type>
inline constexpr Type* Realloc(Type* pT, size_t uWidth)
{
    return (Type*) realloc(pT, uWidth);
}



template<class Type>
inline constexpr void DestroyAt(Type* pT)
{
    pT->~Type();
}



template<class Type>
inline constexpr void DestroyAlloc(Type* pT, size_t uUsed)
{
    for (--uUsed; uUsed != 0; --uUsed)
    {
        DestroyAt(&pT[uUsed]);
    }
    
    DestroyAt(&pT[0]);
}



inline void CalcNewWidth(
        const size_t& uTypeBytes,
        const size_t& uAmount,
              size_t& uBufferWidthBytes,
              size_t& uBufferWidthType
)
{
    uBufferWidthType  += uAmount;
    uBufferWidthBytes += uAmount * uTypeBytes;
}





template<class T>
inline constexpr size_t CopyStringSlice(
              T*      pDest,
        const size_t& uDestIterStart,
        const T*      pSource,
        const size_t& uSourceSize
)
{
    size_t i = 0;

    for (; i < uSourceSize && pSource[i] != '\0'; ++i) {
        pDest[uDestIterStart + i] = pSource[i];
    }

    return i;
}




template<class T>
inline constexpr void CopySlice(
              T*      pDest,
        const size_t& uDestIterStart,
        const T*      pSource,
        const size_t& uSourceSize
)
{
    for (size_t i = 0; i < uSourceSize; ++i) {
        pDest[uDestIterStart + i] = pSource[i];
    }
}

}   // !





template<class Type, size_t BaseAlloc>
class DynamicAlloc
{
    public:
        DynamicAlloc()
            : m_uBufferWidthType(BaseAlloc),
            m_uBufferWidthBytes(m_uBufferWidthType * sizeof(Type)),
            m_pBuffer(Alloc<Type>(m_uBufferWidthBytes)),
            m_uUsed(0)
        {
            if (!m_pBuffer) {
                throw; // TODO
            }
        }

        ~DynamicAlloc() 
        {
            if (m_uUsed && m_pBuffer) {
                DestroyAlloc(m_pBuffer, m_uUsed);
            }

            if (m_pBuffer) {
                free(m_pBuffer);
            }

            m_pBuffer = nullptr;
        }

    
    public:
        Type* GetBegin() { return m_pBuffer; }

        const size_t& GetSize() { return m_uUsed; }



    public:
        void AddSlice(Type* pBeg, size_t uAmount) 
        {
            if (uAmount + m_uUsed >= m_uBufferWidthType) {
                CalcNewWidth(sizeof(Type), uAmount, m_uBufferWidthBytes, m_uBufferWidthType);

                if ((m_pBuffer = Realloc(m_pBuffer, m_uBufferWidthBytes)) == nullptr) {
                    throw; // TODO
                }
            }

            CopySlice(m_pBuffer, m_uUsed, pBeg, uAmount);
            
            m_uUsed += uAmount;
        }
        


        void AddStringSlice(Type* pBeg, size_t uMaxAmount) 
        {
            if (uMaxAmount + m_uUsed >= m_uBufferWidthType) {
                CalcNewWidth(sizeof(Type), uMaxAmount, m_uBufferWidthBytes, m_uBufferWidthType);

                if ((m_pBuffer = Realloc(m_pBuffer, m_uBufferWidthBytes)) == nullptr) {
                    throw; // TODO
                }
            }

            m_uUsed += CopyStringSlice(m_pBuffer, m_uUsed, pBeg, uMaxAmount);
        }


        void Append(Type a) 
        {
            if (1 + m_uUsed >= m_uBufferWidthType) {
                CalcNewWidth(sizeof(Type), 1, m_uBufferWidthBytes, m_uBufferWidthType);

                if ((m_pBuffer = Realloc(m_pBuffer, m_uBufferWidthBytes)) == nullptr) {
                    throw; // TODO
                }
            }

            m_pBuffer[m_uUsed] = a;

            m_uUsed += 1;
        }



        Type* Leak() 
        {
            Type* pLeak = m_pBuffer;
            m_pBuffer   = nullptr;
            m_uUsed     = 0;

            return pLeak;
        }


    private:
        size_t m_uBufferWidthType;
        size_t m_uBufferWidthBytes;

        Type*  m_pBuffer;
        
        size_t m_uUsed;

};





} // !ELang
#endif // !__cplusplus
#endif // !ALLOCATORS_H
