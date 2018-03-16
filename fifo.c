
//   File Name          : fifo.c

#include "fifo.h"
#include "cnc.h"

struct fifo_t objFifoLoaded;

static struct simpleShift fifoBuffer[LOAD_FIFO_BUFF_LEN];


fifo_result_t fifo_init(struct fifo_t * fifo, struct simpleShift *buffer, int16_t size)
{
  if (NULL != fifo && NULL != buffer && 0 < size) {
    fifo->buffer     = buffer;
    fifo->size       = size;
    fifo->first      = 0; //first
    fifo->elements_n = 0;

    return FIFO_SUCCESS;
  } 
  else {
    return FIFO_ERROR;
  }
}


fifo_result_t fifo_push(struct fifo_t *fifo, struct simpleShift *data)
{
  if (NULL != fifo && fifo->elements_n < fifo->size) {
    memcpy( &fifo->buffer[(fifo->first + fifo->elements_n)% fifo->size ], data, sizeof(struct simpleShift) );
    fifo->elements_n++;
    
    return FIFO_SUCCESS;
  } else {
    return FIFO_ERROR;
  }
}



fifo_result_t fifo_show(struct fifo_t *fifo, struct simpleShift *data, int16_t idx)
{ 
  //
  //idx 0.....elements_n-1
  //0            : first in queue
  //elements_n-1 : last in queue
  
  if (NULL == fifo) {
    return FIFO_ERROR;
  }
  if(fifo->size <= idx) {
    return FIFO_LAGE_INDEX_ERROR;
  }
  if(fifo->elements_n <= idx) {
    return FIFO_LAGE_INDEX_ELEMENT_ERROR;
  }
  if(idx <= fifo->first) {
      //f-i
      memcpy(data, &fifo->buffer[fifo->first-idx], sizeof(*data));
  }
  else {
      // s-i+f
      memcpy(data, &fifo->buffer[fifo->elements_n - idx + fifo->first], sizeof(*data));
  }
  return FIFO_SUCCESS;
}

fifo_result_t fifo_pull(struct fifo_t *fifo, struct simpleShift *data)
{
  if (NULL != fifo && 0 < fifo->elements_n) {
    //*data = fifo->buffer[fifo->first];
    memcpy(data, &fifo->buffer[fifo->first], sizeof(struct simpleShift));
    fifo->first = (fifo->first + 1) % fifo->size;
    fifo->elements_n--;
    return FIFO_SUCCESS;
  }
  else {
    return FIFO_ERROR;
  }
}


void initLoadFifo()
{
  fifo_result_t fifo_result; 
  
  fifo_result = fifo_init(&objFifoLoaded, &fifoBuffer[0], LOAD_FIFO_WORK_SIZE );
  if ( FIFO_SUCCESS != fifo_result) {
    int a =4;
    errorAlarm(LOAD_FIFO_ERROR);
  }
}
  

int get_size_of_queue(struct  fifo_t *fifo)
{
  if (NULL != fifo) {
      return fifo->elements_n;
  }else {
    return 0;
  }
}
 




