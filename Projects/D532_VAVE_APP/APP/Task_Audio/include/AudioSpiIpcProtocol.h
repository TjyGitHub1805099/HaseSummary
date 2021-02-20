#ifndef __AUDIO_SPI_IPC_PROTOCOL_H__
#define __AUDIO_SPI_IPC_PROTOCOL_H__

// protocol
// byte 0: type
// byte 1: flag
// byte 2-3: packet id
// byte after: data

// index
#define AUDIOSPI_INDEX_TYPE 0
#define AUDIOSPI_INDEX_FLAG 1

// type
#define AUDIOSPI_TYPE_REQ 0
#define AUDIOSPI_TYPE_ACK 1

// flag
#define AUDIOSPI_FLAG_ACK (1 << 0)

// devipc max transfer size
#define AUDIOSPI_MAX_SIZE 100
#define AUDIOSPI_HEADER_SIZE 4
#define AUDIOSPI_ACK_MSG_SIZE 4

#endif /* __AUDIO_SPI_IPC_PROTOCOL_H__ */
