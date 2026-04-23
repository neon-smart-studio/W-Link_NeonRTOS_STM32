
#ifndef SPI_MASTER_INDEX_H
#define SPI_MASTER_INDEX_H

typedef enum hwSPI_Index_t
{
  hwSPI_Index_0 = 0,
  hwSPI_Index_1,
  hwSPI_Index_2,
#if defined (SPI4_BASE)
  hwSPI_Index_3,
#endif
#if defined (SPI5_BASE)
  hwSPI_Index_4,
#endif
#if defined (SPI6_BASE)
  hwSPI_Index_5,
#endif
  hwSPI_Index_MAX,
}hwSPI_Index;

#endif //SPI_MASTER_INDEX_H