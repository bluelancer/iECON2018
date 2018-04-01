#include <iostream>
#include <ns3/core-module.h>
#include <ns3/network-module.h>
#include <ns3/mobility-module.h>
#include <ns3/lte-module.h>
#include "ns3/lte-helper.h"
#include <ns3/config-store.h>
#include <ns3/internet-module.h>
#include <ns3/point-to-point-module.h>
#include <ns3/applications-module.h>
//#define private public
#include "ns3/epc-helper.h"
//#undef private
#include "ns3/netanim-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include <ns3/config-store-module.h>

// Overall Diagram
//
// IP.addr =7.0.0.0   |   IP.addr =1.0.0.0
// Ue1				  |
//	port 1234         |
//		\			  | 
//   	port 2000     |
//			--<-eNb---|--PGW---<---Emulation Channel---RemoteHost
//		port 2001	  |		itf1
//		/			  |
//	 port 1234        |
// Ue2				  |
// yifei.jin@ericsson.com





using namespace ns3;

int main (int argc, char *argv[])
{
	CommandLine cmd;
	// Config::SetDefault ("ns3::EmuEpcHelper::sgwDeviceName", "veth0");
    // Config::SetDefault ("ns3::EmuEpcHelper::enbDeviceName", "veth1"); 
    // ConfigStore inputConfig;
    // inputConfig.ConfigureDefaults();
    // parse again so you can override default values from the command line
    cmd.Parse(argc, argv);
	Config::SetDefault ("ns3::LteSpectrumPhy::CtrlErrorModelEnabled", BooleanValue (false));
	Config::SetDefault ("ns3::LteSpectrumPhy::DataErrorModelEnabled", BooleanValue (false));  

	GlobalValue::Bind ("ChecksumEnabled", BooleanValue (true));

	ConfigStore inputConfig;
	inputConfig.ConfigureDefaults();	

	Ptr<LteHelper> lteHelper = CreateObject<LteHelper> (); //Creat an helper object
    cmd.Parse(argc, argv);


	uint32_t ueNodesNumber = 2;
	//Create Node objects for the eNB(s) and the UEs:
	NodeContainer enbNodes; // I am ENb
	enbNodes.Create (1);
	NodeContainer ueNodes; //I am Ues
	ueNodes.Create (ueNodesNumber);
	// can we have dynamic number of ues in our simulation area,  suppose we have a car want to being connected
	// Dynamic attachment of the cars (set buget, future work)

	// Configure the Mobility model for all the nodes:
	MobilityHelper mobility;

	
	mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
	mobility.Install (enbNodes);

	mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
									"MinX", DoubleValue (0.0),
									"MinY", DoubleValue (0.0),
									"DeltaX", DoubleValue (5.0),
									"DeltaY", DoubleValue (10.0),
									"GridWidth", UintegerValue (3),
									"LayoutType", StringValue ("RowFirst"));
	mobility.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
                             "Bounds", RectangleValue (Rectangle (-50, 50, -50, 50)));
	mobility.Install (ueNodes);



	// PHY LAYER
	// AIMING: Moblity, LTE protocol stack, Attach eNodeB
	// to protencial Ue respectively.

	// Install an LTE protocol stack on the eNB(s) and Ues:
	NetDeviceContainer enbDevs;
	enbDevs = lteHelper->InstallEnbDevice (enbNodes);
	NetDeviceContainer ueDevs;
	ueDevs = lteHelper->InstallUeDevice (ueNodes);

	// Attach all Ues to the only EnodeB
//	lteHelper->Attach (ueDevs, enbDevs.Get (0));


	// MAC LAYER
	// AIMING: 
	//    1, Set EPC Bearer according to the QoS/QCI
	//    2, Set SchedulerType according to the QoS of the 
	//       Bearers
	//    3, Create PGW as Core network


	// Create EPC helper, 
	// Also this will automatically create PGW node
	// Create a emulation channel to internet
	Ptr<EmuEpcHelper> epcHelper = CreateObject<EmuEpcHelper> ();

	epcHelper->Initialize ();

	// Tell LTE EPC will be used
	lteHelper->SetEpcHelper (epcHelper);

	// Install EPC in PGW node
	Ptr<Node> pgw = epcHelper->GetPgwNode ();

	// Here you find the Scheduler you want to have.
	lteHelper->SetSchedulerType ("ns3::FdMtFfMacScheduler"); 

	// Create a single RemoteHost
	NodeContainer remoteHostContainer;
	remoteHostContainer.Create (1);
	Ptr<Node> remoteHost = remoteHostContainer.Get (0);
	mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
	mobility.Install (remoteHost);
	InternetStackHelper internet;
	internet.Install (remoteHostContainer);

	// Create the internet
	PointToPointHelper p2ph;
	p2ph.SetDeviceAttribute ("DataRate", DataRateValue (DataRate ("100Gb/s")));
	p2ph.SetDeviceAttribute ("Mtu", UintegerValue (1500));
	p2ph.SetChannelAttribute ("Delay", TimeValue (Seconds (0.010)));

	// install internet on PGW node and Remote host
	NetDeviceContainer internetDevices = p2ph.Install (pgw, remoteHost);
	Ipv4AddressHelper ipv4h;
	
	// Assign starts from 1.0.0.0 , Mask is 255.0.0.0
	ipv4h.SetBase ("1.0.0.0", "255.0.0.0");
	Ipv4InterfaceContainer internetIpIfaces = ipv4h.Assign (internetDevices);
	
	// interface 0 is localhost, 1 is the p2p device
	Ipv4Address remoteHostAddr = internetIpIfaces.GetAddress (1);
	

	// Define the Internet first, and then attach Enb to Ue

	NetDeviceContainer ueLteDevs = lteHelper->InstallUeDevice (ueNodes);
	NetDeviceContainer enbLteDevs = lteHelper->InstallEnbDevice (enbNodes);

	Ipv4StaticRoutingHelper ipv4RoutingHelper;
	Ptr<Ipv4StaticRouting> remoteHostStaticRouting = ipv4RoutingHelper.GetStaticRouting (remoteHost->GetObject<Ipv4> ());
	remoteHostStaticRouting->AddNetworkRouteTo (Ipv4Address ("7.0.0.0"), Ipv4Mask ("255.0.0.0"), 1);

	// we install the IP stack on the UEs
	internet.Install (ueNodes);

	Ptr<Node> ue;
	Ptr<NetDevice> ueLteDevice;
	Ipv4InterfaceContainer ueIpIface;
	// assign IP address to UEs
	for (uint32_t u = 0; u < ueNodes.GetN(); ++u)
	  {
	    ue = ueNodes.Get (u);
	    ueLteDevice = ueLteDevs.Get (u);
	    ueIpIface = epcHelper->AssignUeIpv4Address (NetDeviceContainer (ueLteDevice));
	    // set the default gateway for the UE
	    Ptr<Ipv4StaticRouting> ueStaticRouting = ipv4RoutingHelper.GetStaticRouting (ue->GetObject<Ipv4> ());
	    ueStaticRouting->SetDefaultRoute (epcHelper->GetUeDefaultGatewayAddress (), 1);
	    // why to define the device first and attach to node instead of install to nodes
	  }


	// attach Enb to Ue
	lteHelper->Attach (ueLteDevs, enbLteDevs.Get (0));



	// If you want to ue dedicated bearer: comment out the rest:
/*
	Ptr<EpcTft> tft = Create<EpcTft> ();
	EpcTft::PacketFilter pf;
	pf.localPortStart = 1234;
	pf.localPortEnd = 1234;
	tft->Add (pf);
	lteHelper->ActivateDedicatedEpsBearer (ueLteDevs, EpsBearer (EpsBearer::NGBR_VIDEO_TCP_DEFAULT), tft);
*/


	// RLC LAYER
	// AIMING: UDP socket
	uint16_t dlPort = 1234;
	uint16_t ulPort = 2000;
	for (uint32_t u = 0; u < ueNodes.GetN (); ++u)
	{
		++ulPort;
		PacketSinkHelper dlPacketSinkHelper ("ns3::UdpSocketFactory",
		                                   InetSocketAddress (Ipv4Address::GetAny (), dlPort));
		PacketSinkHelper ulPacketSinkHelper ("ns3::UdpSocketFactory",
		                                   InetSocketAddress (Ipv4Address::GetAny (), ulPort));
		ApplicationContainer serverApps; 
		ApplicationContainer clientApps;

		serverApps.Add (dlPacketSinkHelper.Install (ueNodes.Get(u)));
      	serverApps.Add (ulPacketSinkHelper.Install (remoteHost));
		
		UdpClientHelper dlClient (ueIpIface.GetAddress (0), dlPort);
		UdpClientHelper ulClient (remoteHostAddr, ulPort);

		clientApps.Add (dlClient.Install (remoteHost));
        clientApps.Add (ulClient.Install (ueNodes.Get(u)));
		
		// Server Application Start 
		serverApps.Start (Seconds (0.01));
		// Client Application Start
		clientApps.Start (Seconds (0.01));
	}
	// Set up Netanim

	//AnimationInterface anim ("lteemuanim.xml");

	//anim.SetMobilityPollInterval (Seconds (1));
	//anim.SetConstantPosition (enbNodes.Get(0), 2.0, 2.0);
	//anim.EnablePacketMetadata (true);
	//anim.SetConstantPosition (remoteHost, 10.0, 10.0);
	//anim.SetConstantPosition (pgw, 2.0, 10.0);
	Simulator::Stop (Seconds (10));

	Simulator::Run ();
	Simulator::Destroy ();
	return 0;
}