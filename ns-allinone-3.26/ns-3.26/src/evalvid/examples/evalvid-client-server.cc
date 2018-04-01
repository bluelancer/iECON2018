/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
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

 Author: Billy Pinheiro <haquiticos@gmail.com>

 */



// Network topology
//
//       n0    n1
//       |     |
//       =======
//         LAN
//
// - UDP flows from n0 to n1

#include <fstream>
#include <string.h>

#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/csma-helper.h"
#include "ns3/evalvid-client-server-helper.h"

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("EvalvidClientServerExample");

int
main (int argc, char *argv[])
{
//
// Enable logging for EvalvidClient and
//
  LogComponentEnable ("EvalvidClient", LOG_LEVEL_INFO);
  LogComponentEnable ("EvalvidServer", LOG_LEVEL_INFO);

  //
  // But since this is a realtime script, don't allow the user to mess with
  // that.
  //
  //GlobalValue::Bind ("SimulatorImplementationType", StringValue ("ns3::RealtimeSimulatorImpl"));

//
// Explicitly create the nodes required by the topology (shown above).
//
  NS_LOG_INFO ("Create nodes.");
  NodeContainer n;
  n.Create (2);

  InternetStackHelper internet;
  internet.Install (n);

  NS_LOG_INFO ("Create channels.");
//
// Explicitly create the channels required by the topology (shown above).
//
  CsmaHelper csma;
  csma.SetChannelAttribute ("DataRate", DataRateValue (DataRate(5000000)));
  csma.SetChannelAttribute ("Delay", TimeValue (MilliSeconds (2)));
  csma.SetDeviceAttribute ("Mtu", UintegerValue (1400));
  NetDeviceContainer d = csma.Install (n);

  Ipv4AddressHelper ipv4;
//
// We've got the "hardware" in place.  Now we need to add IP addresses.
//
  NS_LOG_INFO ("Assign IP Addresses.");
  ipv4.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer i = ipv4.Assign (d);

  NS_LOG_INFO ("Create Applications.");



  //
  // Create one udpServer applications on node one.
  //
  uint16_t port = 4000;
  EvalvidServerHelper server (port);
  server.SetAttribute ("SenderTraceFilename", StringValue("st_highway_cif.st"));
  server.SetAttribute ("SenderDumpFilename", StringValue("sd_a01"));
  server.SetAttribute ("PacketPayload",UintegerValue(1014));
  ApplicationContainer apps = server.Install (n.Get(1));
  apps.Start (Seconds (9.0));
  apps.Stop (Seconds (101.0));


  //
  // Create one EvalvidClient application
  //
  EvalvidClientHelper client (i.GetAddress (1),port);
  client.SetAttribute ("ReceiverDumpFilename", StringValue("rd_a01"));
  apps = client.Install (n.Get (0));
  apps.Start (Seconds (10.0));
  apps.Stop (Seconds (100.0));


  //
// Now, do the actual simulation.
//
  NS_LOG_INFO ("Run Simulation.");
  Simulator::Run ();
  Simulator::Destroy ();
  NS_LOG_INFO ("Done.");

}
