/*
 * Copyright (c) 2010  by Radek Pazdera <radek.pazdera@gmail.com>
 *
 * This file is part of nfgen.
 *
 * nfgen is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * nfgen is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with nfgen.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _NETFLOW__H_
#define _NETFLOW__H_

#include <stdint.h>
#include <time.h>

#define MAX_NETFLOW_PDU_SIZE 1464
#define MAX_NETFLOW_RECORDS 30

/** NetFlow version 5 PDU header */
struct netflowHeader
{
    uint16_t version;       /* 5 */
    uint16_t count;         /* The number of records in the PDU */
    uint32_t sysUpTime;     /* Current time in millisecs since router booted */
    uint32_t unixSecs;      /* Current seconds since 0000 UTC 1970 */
    uint32_t unixNsecs;     /* Residual nanoseconds since 0000 UTC 1970 */
    uint32_t flowSequence;  /* Seq counter of total flows seen */
    uint8_t  engineType;    /* Type of flow switching engine (RP,VIP,etc.) */
    uint8_t  engineId;      /* Slot number of the flow switching engine */
    uint16_t reserved;
};

/** NetFlow version 5 record */
struct netflowRecord
{
    uint32_t srcAddr;     /* Source IP Address */
    uint32_t dstAddr;     /* Destination IP Address */
    uint32_t nextHop;     /* Next hop router's IP Address */
    uint16_t input;       /* Input interface index */
    uint16_t output;      /* Output interface index */
    uint32_t dPkts;       /* Packets sent in Duration */
    uint32_t dOctets;     /* Octets sent in Duration. */
    uint32_t first;       /* SysUptime at start of flow */
    uint32_t last;        /* and of last packet of flow */
    uint16_t srcPort;     /* TCP/UDP source port number or equivalent */
    uint16_t dstPort;     /* TCP/UDP destination port number or equiv */
    uint8_t  pad;
    uint8_t  tcpFlags;    /* Cumulative OR of tcp flags */
    uint8_t  prot;        /* IP protocol, e.g., 6=TCP, 17=UDP, ... */
    uint8_t  tos;         /* IP Type-of-Service */
    uint16_t srcAs;       /* originating AS of source address */
    uint16_t dstAs;       /* originating AS of destination address */
    uint8_t  srcMask;     /* source address prefix mask bits */
    uint8_t  dstMask;     /* destination address prefix mask bits */
    uint16_t drops;
};

/**
 * Make pseudo-random NetFlow PDU
 *
 * Creates semi-random NetFlow v5 into \c buffer.
 * Some attributes are random and some are computed and
 * set so they make sense in the stream comming from this
 * exporter.
 *
 * Size of the resulting PDU can reach up to 1464 bytes
 * (24B for header + up to 30 records, each of 48B = 1464).
 * The buffer size must be greater or equal, otherwise
 * expect some segfaults.
 *
 * @param[out] buffer Buffer for NetFlow PDU
 * @param[in]  systemStartTime Start of NetFlow exporter (this program) \
 *                             This value is later used to determine flow durations.
 * @param[in]  numberOfFLows How many records should be generated into the PDU
 * @param[in]  totalFlowsSent Total number of flows sent from \c systemStartTime \
 *                            by this exporter including \c current numberOfFlows
 *
 * @return Final PDU size stored in \c buffer
 */
size_t makeRandomNetflowPacket(char *buffer, time_t systemStartTime, unsigned int numberOfFlows, unsigned int totalFlowsSent);


#endif
