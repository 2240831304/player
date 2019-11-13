
#ifndef PACKETLIST_H
#define PACKETLIST_H

extern "C"{
    #include <libavformat/avformat.h>
    #include <SDL2/SDL.h>
}

typedef struct PacketList
{
    AVPacketList *first_pkt; // 队头
    AVPacketList *last_pkt; // 队尾
    int nb_packets; //包的个数
    int size; // 占用空间的字节数
    SDL_mutex* mutex; // 互斥信号量
    SDL_cond* cond; // 条件变量
}PacketList;


static void packet_queue_init(PacketList *q)
{
    memset(q, 0, sizeof(PacketList));
    q->nb_packets = 0;
    q->size = 0;
    q->mutex = SDL_CreateMutex();
    q->cond = SDL_CreateCond();
}


static int packet_queue_put(PacketList *q, AVPacket *pkt)
{
    AVPacketList *pkt1;
    if(av_dup_packet(pkt) < 0) {
        return -1;
    }

    pkt1 = (AVPacketList *)av_malloc(sizeof(AVPacketList));
    if (!pkt1)
        return -1;

    pkt1->pkt = *pkt;
    pkt1->next = NULL;

    //SDL_LockMutex(q->mutex);

    //改变队头，队尾的值
    if (!q->last_pkt)
        q->first_pkt = pkt1;
    else
        q->last_pkt->next = pkt1;

    q->last_pkt = pkt1;
    q->nb_packets++;
    //q->size += pkt1->pkt.size;

    //SDL_UnlockMutex(q->mutex);
    return 0;
}


static int packet_queue_get(PacketList *q, AVPacket *pkt)
{
    if(q->nb_packets <= 0)
        return -1;
    AVPacketList *pkt1;

    //SDL_LockMutex(q->mutex);

    pkt1 = q->first_pkt;
    if (pkt1) {
        q->first_pkt = pkt1->next;
        if (!q->first_pkt)
            q->last_pkt = NULL;
        q->nb_packets--;
        //q->size -= pkt1->pkt.size;
        *pkt = pkt1->pkt;
        av_free(pkt1);
    }
    //SDL_UnlockMutex(q->mutex);

    return 1;
}

static void packet_queue_clear(PacketList *q)
{
    SDL_LockMutex(q->mutex);
    while(q->nb_packets > 0){
        AVPacketList *pkt1 = q->first_pkt;
        q->first_pkt = pkt1->next;

        if (!q->first_pkt)
            q->last_pkt = NULL;

        if (pkt1) {
            q->nb_packets--;
            //q->size -= pkt1->pkt.size;
            av_free(pkt1);
        }
    }
    SDL_UnlockMutex(q->mutex);
}

static bool packet_queue_empty(PacketList *q)
{
    bool flag;
    SDL_LockMutex(q->mutex);
    if(q->nb_packets > 0)
        flag = false;
    else
        flag = true;
    SDL_UnlockMutex(q->mutex);

    return flag;
}

static int packet_queue_Size(PacketList *q)
{
    return q->nb_packets;
}


#endif