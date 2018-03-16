
#ifndef __fifo_H
#define __fifo_H

#ifdef __cplusplus
 extern "C" {
#endif
   
   
#include "stm32f4xx_hal.h"
#include "cnc.h"

#define  LOAD_FIFO_BUFF_LEN (17)  
#define  LOAD_FIFO_WORK_SIZE (15)

typedef enum {
    FIFO_SUCCESS = 0,
    FIFO_FALSE   = 2,
    FIFO_TRUE    = 1,
    FIFO_ERROR   = 3,
    FIFO_LAGE_INDEX_ERROR = 4,
    FIFO_LAGE_INDEX_ELEMENT_ERROR = 5
} fifo_result_t;

struct fifo_t { 
    struct simpleShift *buffer;
    uint16_t size;
    uint16_t first;
    uint16_t elements_n;
};

extern struct fifo_t objFifoLoaded;

fifo_result_t fifo_init(struct fifo_t *fifo, struct simpleShift *buffer, int16_t size);
fifo_result_t fifo_push(struct fifo_t *fifo, struct simpleShift *data);
fifo_result_t fifo_show(struct fifo_t *fifo, struct simpleShift *data, int16_t idx);
fifo_result_t fifo_pull(struct fifo_t *fifo, struct simpleShift *data);
int get_size_of_queue(struct fifo_t *fifo);

#ifdef __cplusplus
}
#endif

#endif /* __fifo_H */
