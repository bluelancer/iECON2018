/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Billy Pinheiro <haquiticos@gmail.com>
 *         Saulo da Mata <damata.saulo@gmail.com>
 *
 */

#ifndef __EVALVID_SERVER_H__
#define __EVALVID_SERVER_H__

#include "ns3/application.h"
#include "ns3/event-id.h"
#include "ns3/ptr.h"
#include "ns3/address.h"
#include "ns3/ipv4-address.h"
#include "ns3/seq-ts-header.h"
#include "ns3/socket.h"


#include <stdio.h>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <map>

using std::ifstream;
using std::ofstream;
using std::ostream;

using std::ios;
using std::endl;


using namespace std;

namespace ns3 {

/**
 * \ingroup applications
 * \defgroup Evalvid Evalvid
 */

/**
 * \ingroup Evalvid
 * \class EvalvidClient
 * \brief A Udp server. Receives UDP packets from a remote host. UDP packets
 * carry a 32bits sequence number followed by a 64bits time stamp in their
 * payloads. The application uses, the sequence number to determine if a packet
 * is lost, and the time stamp to compute the delay
 */
class EvalvidServer : public Application
{
public:
  static TypeId GetTypeId (void);
  EvalvidServer ();
  virtual ~EvalvidServer ();

protected:
  virtual void DoDispose (void);

private:

  virtual void StartApplication (void);
  virtual void StopApplication (void);

  void Setup (void);
  void HandleRead (Ptr<Socket> socket);
  void Send();


  string      m_videoTraceFileName;	        //File from mp4trace tool of Evalvid.
  string      m_senderTraceFileName;		//File with information of packets transmitted by EvalvidServer.
  fstream     m_videoTraceFile;
  uint32_t    m_numOfFrames;
  uint16_t    m_packetPayload;
  ofstream    m_senderTraceFile;
  uint32_t    m_packetId;
  uint16_t    m_port;
  Ptr<Socket> m_socket;
  Address     m_peerAddress;
  EventId     m_sendEvent;

  struct m_videoInfoStruct_t
  {
    string   frameType;
    uint32_t frameSize;
    uint16_t numOfUdpPackets;
    Time     packetInterval;
  };

  map<uint32_t, m_videoInfoStruct_t*> m_videoInfoMap;
  map<uint32_t, m_videoInfoStruct_t*>::iterator m_videoInfoMapIt;
};

} // namespace ns3

#endif // __EVALVID_SERVER_H__
