/*
 * tdf853x_common.c
 *
 *  Created on: 2018Äê9ÔÂ13ÈÕ
 *      Author: gaojing
 */
#include "Std_Types.h"
#include "tdf853x.h"

/* tdf853x write process */
int tdf853x_write_command(tdf853x_instance* instance, uint8_t* buffer, uint8_t length)
{
    int ret;

    if(instance->i2c_write == NULL_PTR){
        return -1;
    }

    ret = instance->i2c_write(instance, buffer, length);

    return ret;
}

/* tdf853x read process */
int tdf853x_read_command(tdf853x_instance* instance, uint8_t* buffer, uint8_t* length)
{
    int ret;
    boolean ack_received = FALSE;
    int repl_length = 0;
    int i = 0;

    /* send req */
    ret = tdf853x_write_command(instance, buffer, *length);
    if(ret < 0){
        return -1;
    }

    /* TODO: sleep */
    System_Task_Delay(1);

    /* wait for ack */
   while(!ack_received){
        /* ack nak message should be 3 length */
        ret = instance->i2c_read(instance, buffer, 3);
        if(ret < 0){
            return -1;
        }

        if(buffer[MSGTYPE] == ERR){
            return -1;
        }else if(buffer[MSGTYPE] == NAK){
            continue;
        }else if(buffer[MSGTYPE] == ACK){
            ack_received = TRUE;
            repl_length = buffer[LENGTH];
        }else{
            return -1;
        }
    }

    /* read repl */
    if(repl_length <= 0){
        return -1;
    }

    /* add <MsgType><PacketId><Length> length */
    repl_length += 3;

    ret = instance->i2c_read(instance, buffer, repl_length);
    if(ret < 0){
        return -1;
    }

    *length = repl_length;

    return 0;
}

// <moduleId><CmdId><Parameters>
void tdf853x_make_dll_payload(uint8_t moduleId, uint8_t cmdId, uint8_t* buf, int length, uint8_t* target_buf)
{
    uint8_t* data;
    int i;

    target_buf[MODULEID] = moduleId;
    target_buf[CMDID] = cmdId;

    data = &target_buf[PARAMETERS];
    for(i = 0; i < length; i++){
        *data = buf[i];
        data++;
    }
}
// <MsgType><PacketId><Length>[<moduleId><CmdId><Parameters>]
void tdf853x_make_i2c_payload(uint8_t msgType, uint8_t packetId, uint8_t length, uint8_t* target_buf)
{
    target_buf[MSGTYPE] = msgType;
    target_buf[PACKETID] = packetId;
    target_buf[LENGTH] = length;
}


