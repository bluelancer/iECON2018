// LTE core
#include <iostream>
#include <ns3/core-module.h>
#include <ns3/network-module.h>
#include <ns3/lte-module.h>
#include "ns3/lte-helper.h"
#include <ns3/config-store.h>
#include <ns3/internet-module.h>
#include <ns3/point-to-point-module.h>
#include <ns3/applications-module.h>
#include "ns3/epc-helper.h"
#include <fstream>
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/ipv4-flow-classifier.h"

// Animation Generator
#include "ns3/netanim-module.h"

// Mobility Model
#include "ns3/ns2-mobility-helper.h"
#include <ns3/mobility-module.h>
#include "ns3/flow-monitor-helper.h"
#include "ns3/gnuplot.h"
#include "string"
#include <iostream>

// Vedio Streaming simulator
#include "ns3/evalvid-client-server-helper.h"

// Building fading model
#include <ns3/mobility-building-info.h>
#include <ns3/buildings-propagation-loss-model.h>
#include <ns3/building.h>
#include "ns3/buildings-helper.h"
#include <ns3/hybrid-buildings-propagation-loss-model.h>
#include "ns3/oh-buildings-propagation-loss-model.h"




using namespace ns3;
using namespace std;

// Prints actual position and velocity when a course change event occurs

NS_LOG_COMPONENT_DEFINE ("EvalvidLTEExample");
static void
CourseChange (std::ostream *os, std::string foo, Ptr<const MobilityModel> mobility)
{
  Vector pos = mobility->GetPosition (); // Get position
  Vector vel = mobility->GetVelocity (); // Get velocity

  // Prints position and velocities
  *os << Simulator::Now () << " POS: x=" << pos.x << ", y=" << pos.y
      << ", z=" << pos.z << "; VEL:" << vel.x << ", y=" << vel.y
      << ", z=" << vel.z << std::endl;
}


// Example to use ns2 traces file in ns3

void
ThroughputMonitor (FlowMonitorHelper *flowmon_helper, Ptr<FlowMonitor> monitor, Gnuplot2dDataset dataset, double Flow_Number)
{
  double Throughput = 0.0;
  double JitterSum = 0.0;
  double LostRatio = 0.0;
  double LostPacket = 0.0;
  double Delay =0.0;

  std::map<FlowId, FlowMonitor::FlowStats> stats = monitor->GetFlowStats ();
  Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier>(flowmon_helper->GetClassifier ());
  for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator iter = stats.begin (); iter != stats.end (); ++iter)
      {
    	
	    Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow (iter->first);
	    NS_LOG_UNCOND("Flow ID: " << iter->first << " Src Addr " << t.sourceAddress << " Dst Addr " << t.destinationAddress);
	    NS_LOG_UNCOND("Tx Packets = " << iter->second.txPackets);
	    NS_LOG_UNCOND("Rx Packets = " << iter->second.rxPackets);
	    Delay = iter->second.lastDelay.GetSeconds();
      NS_LOG_UNCOND("Delay =: "<< Delay<<" s");
	    Throughput = iter->second.rxBytes * 8.0 / (iter->second.timeLastRxPacket.GetSeconds()-iter->second.timeFirstTxPacket.GetSeconds()) / 1024;
	    JitterSum = (double) iter->second.jitterSum.GetSeconds();
      uint32_t Tx_Packets = iter->second.txPackets;
      uint32_t Rx_Packets = iter->second.rxPackets;
      LostPacket = Tx_Packets - Rx_Packets;
	    LostRatio = LostPacket / Tx_Packets;
	    NS_LOG_UNCOND("Throughput = "<< Throughput<<" Kbps");
	    NS_LOG_UNCOND("Jitter Sum = "<< JitterSum);
      NS_LOG_UNCOND("Total Lost Ratio = "<< LostRatio<<" Percent");
	    if ( iter->first == Flow_Number )
			dataset.Add((double) iter->second.timeLastRxPacket.GetSeconds(),(double) LostRatio);
     	
      }
      Simulator::Schedule(Seconds(1),&ThroughputMonitor, flowmon_helper, monitor, dataset, Flow_Number);
      
}


int main (int argc, char *argv[])
{
  std::string traceFile;
  std::string logFile;

  // LogComponentEnable ("EvalvidClient", LOG_LEVEL_INFO);
  // LogComponentEnable ("EvalvidServer", LOG_LEVEL_INFO);

  int    nodeNum;
  double duration;

  // Enable logging from the ns2 helpern
  // LogComponentEnable ("Ns2MobilityHelper",LOG_LEVEL_DEBUG);

  // Parse command line attribute
  CommandLine cmd;
  cmd.AddValue ("traceFile", "Ns2 movement trace file", traceFile);
  cmd.AddValue ("nodeNum", "Number of nodes", nodeNum);
  cmd.AddValue ("duration", "Duration of Simulation", duration);
  cmd.AddValue ("logFile", "Log file", logFile);
  cmd.Parse (argc,argv);

  // Check command line arguments
  if (traceFile.empty () || nodeNum <= 0 || duration <= 0 || logFile.empty ())
    {
      std::cout << "Usage of " << argv[0] << " :\n\n"
      "./waf --run \"ns2-mobility-trace"
      " --traceFile=src/mobility/examples/default.ns_movements"
      " --nodeNum=2 --duration=100.0 --logFile=ns2-mob.log\" \n\n"
      "NOTE: ns2-traces-file could be an absolute or relative path. You could use the file default.ns_movements\n"
      "      included in the same directory of this example file.\n\n"
      "NOTE 2: Number of nodes present in the trace file must match with the command line argument and must\n"
      "        be a positive number. Note that you must know it before to be able to load it.\n\n"
      "NOTE 3: Duration must be a positive number. Note that you must know it before to be able to load it.\n\n";

      return 0;
    }

  // Create Ns2MobilityHelper with the specified trace log file as parameter
  Ns2MobilityHelper ns2 = Ns2MobilityHelper (traceFile);

  // open log file for output
  std::ofstream os;
  os.open (logFile.c_str ());

  // LogComponentEnable("BuildingsHelper", LOG_LEVEL_ALL);
  // LogComponentEnable("CqaFfMacScheduler", LOG_LEVEL_ALL);
  // LogComponentEnable("FdTbfqFfMacScheduler",LOG_LEVEL_ALL);

  // double x_min = -200.0;
  // double x_max = 0.0;
  // double y_min = -200.0;
  // double y_max = 0.0;

  double z_min = 0.0;
  double z_max = 10.0;

  double Max_X_Array[200]= {1707.87,2385.25,1414.13,591.75,588.3,664.5,661.12,634.73,997.19,1421.16,724.04,1467.75,497.35,436.03,529.28,510.98,531.37,452.54,451.99,574.39,463.01,426.78,452.61,555.32,640.57,642.78,677.19,723.29,777.96,836.73,741.5,827.17,800.44,686.79,455.27,442.39,433.98,782.7,463.08,421.31,389.08,547.79,535.21,435.98,804.13,645.12,1016.21,768.19,1107.79,1211.95,1232.31,1288.04,1329.62,1353.08,1583.22,1517.95,1473.65,1476.99,1549.81,1577.72,1679.74,1615.73,1678.92,1617.36,1733.91,1779.4,1802.99,1835.74,1748.19,1665.97,1724.03,1587.42,1648.4,1472.32,1305.5,1503.25,1380.45,1268.39,1131.03,1326.9,1418.04,1484.04,1605.16,1506.04,1157.75,519.22,1876.72,1844.6,1770.44,1825.49,1798.85,1886.94,1762.37,1868.85,1894.19,1882.6,1919.04,1939.06,1962.35,1988.93,2011.59,2007.34,1920.22,1964.16,1976.76,1947.47,2007.93,1114.32,1079.81,677.66,675.57,689.2,676.38,741.47,660.16,886.03,596.83,1539.18,828.97,762.52,763.37,837.54,723.65,709.37,714.93,966.88,593.71,578.35,1038.8,782.23,943.33,943.33,478.33,618.96,1085.01,1200.26,1244.82,1147.44,1053.23,960.98,957.43,1176.02,1227.84,966.4,894.23,892.62,1523.58,1490.04,853.86,921.85,951.78,1070.87,1108.19,479.55,626.78,579.56,642.13,687.28,622.54,934.7,807.94,853.87,892.74,897.48,927.95,777,773.7,806.25,889.73,841.6,1039.16,1109.99,1136.87,957.7,1045.85,1008.9,552.71,513.8,518.82,663.87,574.22,1414.13,1393.47,1426.82,545.59,1042.62,1418.04,421,1838.53,1118.42,414,470.46,440.3,996.09,920.18,1315.15,1226.44,938.06,986.26,1372.47};
  double Min_X_Array[200]= {1679.16,1683.92,1375.57,529.08,566.12,580.61,582.62,558.3,877.12,1349.48,715.37,1293.56,435.94,391.97,486.38,467.93,457.42,409.47,384.32,512.05,394.63,339.51,427.19,487.11,584.51,582.01,633.39,662.49,728.71,772.02,695.21,763.51,740.31,672.92,430.46,429.2,410.27,660.11,438.11,352.48,301.62,536.04,516.8,213.69,687.91,616.12,938.67,637.94,1006.12,1061.95,1145.43,1184.1,1251.61,1268.25,1479.05,1461.27,1428.09,1360.74,1455.95,1503.73,1572.66,1540.69,1619.6,1564.28,1661.2,1721.69,1757.8,1770.76,1665.8,1608.42,1628.75,1457.75,1593.62,1400.76,1103.71,1457.38,1282.76,1200.44,1004.42,1261.57,1326.65,1425.25,1493.89,1252.88,939.96,415.91,1870.5,1831.08,1758.81,1807.76,1785.37,1875.56,1749.38,1854.24,1883.88,1869.75,1904.77,1926.2,1950.34,1972.52,1996.66,1998.38,1909.49,1948.87,1960.98,1936.2,1993.93,1035.47,1001.75,656.06,653.98,668.17,655.25,736.25,623.8,821.73,504.21,1388.37,714.18,660.81,578.17,728.31,712.36,698.09,706.81,819.32,487.49,443.34,1004.44,744.15,843.85,843.85,456.42,496.43,1079.28,1136.77,1155.46,1038.27,961.04,888.03,943.29,1097.37,1149.15,794.28,814.6,805.24,1516.62,1479.38,811.74,879.57,913.15,969.70,1034.99,414.07,577.97,543.05,615.09,626.68,554.49,903.11,759.78,812.36,852.15,874.48,896.72,735.75,727.59,760.86,797.81,790.2,963.34,1027.18,1083.63,909.57,983.35,933.46,504.13,496.16,478.96,596.8,550.65,1396.01,1375.57,1360.74,512.78,857.99,1326.65,351.35,1815.06,1039.73,369.92,460.5,396.35,951.4,900.99,1293.76,1168.68,931.2,934.14,1148.4};
  double Max_Y_Array[200]= {3776.13,4959.23,4592.37,3613.41,4185.32,3925.77,4208.69,4242.09,3616.29,3760.66,4191.62,4553.03,3872.59,3852.62,3860.08,3906.97,3777.49,3804.76,3733.59,3928.09,3701.75,4267.38,4175.37,3567.77,3569.73,3682.55,3656.98,3727.96,3748.39,3690.64,3582.39,3614.36,3663.77,3585.77,3568.21,3578.52,3597.55,4081.83,4619.12,4700.09,4638.82,4604.18,4615.2,4781.04,4615.74,4517.2,4873.58,4870.86,4799.31,4760.11,4688.71,4668.76,4621.04,4599.39,4479.85,4387.96,4435.34,4392.86,4272.47,4318.05,4349.2,4164.57,4179.65,4239.99,4218.66,4158.65,4071.48,4031.3,4009.93,4087.58,4080.33,3879.77,3954.82,3971.16,4449.78,4003.46,4169.51,4233.99,4605.71,3771.36,3934.95,3830.47,4051.24,4262.62,3901.93,4020.75,4733.02,4729.15,4764.46,4747.91,4751.2,4770.17,4793.98,4723.23,4729.84,4805.04,4712.1,4709.31,4707.79,4706.91,4700.06,4684.08,4768.68,4760.25,4789.23,4802.62,4747.73,4174.79,4221.23,4738.46,4752.68,4692,4743.99,4755.39,4663.98,4729.57,4171.08,4852.63,4541.52,4303.13,4441.21,4365.41,4753.93,4752.13,4752.83,4196.13,4563.42,4524.74,4115.9,4218.82,4175,4175,3983.25,4177.35,4309.48,4527.24,4492.8,4418.74,4532.89,4653.17,4673.05,4130.7,4166.21,4484.88,4403.76,4563.99,4768.33,4766.18,3975.16,4014.34,3948.29,3949.64,3977.74,4382.26,3735.37,3686.87,3845.76,3813.19,3818.62,4331.01,3969.86,3919.49,3906.21,3851.87,3843.68,4194.07,4183.02,4171.1,4264.96,4260.28,4274.96,4287.2,4250.79,4051.08,4036,4096.59,4336.6,4319.08,4310.03,4542.72,3927.33,4598.24,4564.14,4588.35,4300.68,4389.68,3934.95,4437.65,4796.68,4088.63,4183.6,4182.39,4145.8,3987.94,4137.26,4112.33,4288.39,4621.17,4618.39,4368.56};
  double Min_Y_Array[200]= {3743.93,4612.55,4560.51,3554.06,4158.05,3866.76,4143.58,4181.22,3447.88,3708.42,4182.04,4400.55,3791.82,3809.58,3810.89,3864.03,3718.27,3754.9,3678.3,3846.78,3645.2,4175.96,4147.85,3494.99,3505.36,3627.96,3615.61,3674.26,3690.27,3619.07,3539.08,3556.27,3576.71,3568.66,3541.28,3564.86,3570.64,3973.97,4601.96,4643.24,4514.58,4593.5,4598.15,4512.41,4500.94,4487.16,4747.48,4721.03,4670.67,4614.44,4581.36,4556.09,4527.57,4494.34,4355.01,4325.75,4393.5,4271.83,4161.02,4253,4217.31,4064.43,4115.22,4195.38,4149.94,4093.5,4032.24,3960.07,3902.36,4030.26,4001.57,3779,3886.14,3891.1,1202.18,3953.59,4077.1,4154.83,4497.59,3718.84,3843.7,3746.18,3945.11,4003.56,3778.72,3914.46,4724.66,4718.77,4753.25,4725.42,4737.57,4761.49,4780.3,4714.26,4719.43,4792.52,4696.3,4698.2,4695.74,4690.57,4687.71,4678.63,4759.02,4746.89,4772.43,4791.56,4732.32,4100.72,4147.8,4726.81,4741.02,4644.36,4735.49,4746.15,4651.33,4651.5,4054.42,4730.17,4402.47,4201.7,4279.67,4259.93,4732.49,4730.7,4731.79,4052.07,4473.89,4400.33,4077.03,4185.8,4080.07,4080.07,3957.22,4053.94,4303.77,4479.54,4404.57,4346.28,4416.23,4529.26,4650.72,4048.91,4084.37,4369.15,4328.4,4511.17,4761.45,4755.95,3927.49,3951.17,3903.83,3843.53,3897.06,4317.7,3685.73,3653.25,3820.87,3727.31,3763.19,4300.73,3927.12,3872.65,3859.08,3812.18,3804.26,4148.15,4126.3,4131.33,4150.44,4220.83,4211.42,4176.24,4190.06,3992.78,3972.12,4030.93,4294.68,4300.67,4263.88,4462.46,3903.3,4588.35,4554.49,4530.38,4271.27,4235.05,3843.7,4364.25,4773.26,4006.79,4118.08,4173.28,4089.52,3936.31,4119.92,4089.3,4225.36,4615.47,4552.19,4217.69};

for (int i = 0; i< 200; i++)
{
  Ptr<Building> b = CreateObject <Building> ();
  b->SetBoundaries (Box (Min_X_Array[i], Max_X_Array[i], Min_Y_Array[i], Max_Y_Array[i], z_min, z_max));
  b->SetBuildingType (Building::Residential);
  b->SetExtWallsType (Building::ConcreteWithWindows);
}
  // Create all nodes.
  NodeContainer ueNodes;
  ueNodes.Create (nodeNum);
  NodeContainer enbNodes;

  // Using Isotropic Antenna Model
  enbNodes.Create (1);

  // Using Cosine Antenna Model
  // enbNodes.Create (3);

  
  ns2.Install (ueNodes.Begin(),ueNodes.End()); 
  
  
  // Configure callback for logging
  Config::Connect ("/NodeList/*/$ns3::MobilityModel/CourseChange",
                   MakeBoundCallback (&CourseChange, &os));
  MobilityHelper mobility;
  Ptr<ListPositionAllocator> positionAlloc = CreateObject<ListPositionAllocator>();
  positionAlloc->Add (Vector (880.0, 4100.0, 10.0));
  mobility.SetPositionAllocator (positionAlloc);
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install(enbNodes);

  BuildingsHelper::Install (ueNodes);
  BuildingsHelper::Install (enbNodes);
  BuildingsHelper::MakeMobilityModelConsistent ();
  
  Ptr<LteHelper> lteHelper = CreateObject<LteHelper> ();
  lteHelper->EnableMacTraces ();
  Ptr<PointToPointEpcHelper> epcHelper = CreateObject<PointToPointEpcHelper> ();

  epcHelper->Initialize ();

  Config::SetDefault ("ns3::LteEnbPhy::TxPower", DoubleValue (100));
  Config::SetDefault ("ns3::LteEnbPhy::NoiseFigure", DoubleValue (5));
  Config::SetDefault ("ns3::LteUePhy::TxPower", DoubleValue (100));
  Config::SetDefault ("ns3::LteUePhy::NoiseFigure", DoubleValue (5));

  lteHelper->SetEnbDeviceAttribute ("DlEarfcn", UintegerValue (2750));
  lteHelper->SetEnbDeviceAttribute ("UlEarfcn", UintegerValue (20750));
  lteHelper->SetAttribute ("PathlossModel", StringValue ("ns3::FriisPropagationLossModel"));
  // lteHelper->SetAttribute ("PathlossModel", StringValue ("ns3::HybridBuildingsPropagationLossModel"));
  lteHelper->SetPathlossModelAttribute ("Frequency", DoubleValue (2600));
  lteHelper->SetSpectrumChannelType ("ns3::MultiModelSpectrumChannel");  



  // Tell LTE EPC will be used
  lteHelper->SetEpcHelper (epcHelper);

  // Install EPC in PGW node
  Ptr<Node> pgw = epcHelper->GetPgwNode ();

  // Here you find the Scheduler you want to have.

  // CQA Scheduler
  lteHelper->SetSchedulerType ("ns3::CqaFfMacScheduler"); 
  
  // Token Bank Scheduler
  // lteHelper->SetSchedulerType ("ns3::FdTbfqFfMacScheduler");
  // Config::SetDefault ("ns3::FdTbfqFfMacScheduler::HarqEnabled", BooleanValue (false));
  // lteHelper->SetSchedulerAttribute("DebtLimit", IntegerValue(-625000)); // default value -625000 bytes (-5Mb)
  // lteHelper->SetSchedulerAttribute("CreditLimit", UintegerValue(625000)); // default value 625000 bytes (5Mb)
  // lteHelper->SetSchedulerAttribute("TokenPoolSize", UintegerValue(100000)); // default value 1 byte
  // lteHelper->SetSchedulerAttribute("CreditableThreshold", UintegerValue(10000)); // default value 0

  // PSS Scheduler
  // lteHelper->SetSchedulerType ("ns3::PssFfMacScheduler"); 
  // lteHelper->SetSchedulerAttribute("nMux", UintegerValue(10)); 
  // lteHelper->SetSchedulerAttribute("PssFdSchedulerType", StringValue("CoItA"));

  NodeContainer remoteHostContainer;
  remoteHostContainer.Create (1);
  Ptr<Node> remoteHost = remoteHostContainer.Get (0);
  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (remoteHost);
  mobility.Install (pgw);
  InternetStackHelper internet;
  internet.Install (remoteHostContainer);
  
  PointToPointHelper p2ph;
  p2ph.SetDeviceAttribute ("DataRate", DataRateValue (DataRate ("100Gb/s")));
  p2ph.SetDeviceAttribute ("Mtu", UintegerValue (1500));
  p2ph.SetChannelAttribute ("Delay", TimeValue (Seconds (0.010)));
  

  NetDeviceContainer internetDevices = p2ph.Install (pgw, remoteHost);
  Ipv4AddressHelper ipv4h;
  ipv4h.SetBase ("1.0.0.0", "255.0.0.0");
  Ipv4InterfaceContainer internetIpIfaces = ipv4h.Assign (internetDevices);
  Ipv4Address remoteHostAddr = internetIpIfaces.GetAddress (1);


  NetDeviceContainer enbDevs; 
  NetDeviceContainer ueDevs;

  // Using Cosine Antenna Model 
  // lteHelper->SetEnbAntennaModelType ("ns3::CosineAntennaModel");
  // lteHelper->SetEnbAntennaModelAttribute ("Orientation", DoubleValue (360/3));
  // lteHelper->SetEnbAntennaModelAttribute ("Beamwidth",   DoubleValue (60));
  // lteHelper->SetEnbAntennaModelAttribute ("MaxGain",     DoubleValue (0.0)); 
  // enbDevs.Add (lteHelper->InstallEnbDevice (enbNodes.Get (0)));


  // lteHelper->SetEnbAntennaModelType ("ns3::CosineAntennaModel");
  // lteHelper->SetEnbAntennaModelAttribute ("Orientation", DoubleValue (0));
  // lteHelper->SetEnbAntennaModelAttribute ("Beamwidth",   DoubleValue (60));
  // lteHelper->SetEnbAntennaModelAttribute ("MaxGain",     DoubleValue (0.0)); 
  // enbDevs.Add (lteHelper->InstallEnbDevice (enbNodes.Get (1)));

  // lteHelper->SetEnbAntennaModelType ("ns3::CosineAntennaModel");
  // lteHelper->SetEnbAntennaModelAttribute ("Orientation", DoubleValue (2*360/3));
  // lteHelper->SetEnbAntennaModelAttribute ("Beamwidth",   DoubleValue (60));
  // lteHelper->SetEnbAntennaModelAttribute ("MaxGain",     DoubleValue (0.0)); 
  // enbDevs.Add (lteHelper->InstallEnbDevice (enbNodes.Get (2)));



  // Using Isotropic Antenna Model
  // lteHelper->SetEnbAntennaModelType ("ns3::IsotropicAntennaModel");

  enbDevs.Add (lteHelper->InstallEnbDevice (enbNodes.Get (0)));


  // lteHelper->SetUeAntennaModelType ("ns3::IsotropicAntennaModel");
  // ueDevs.Add (lteHelper->InstallUeDevice (ueNodes.Get (0)));


  Ipv4StaticRoutingHelper ipv4RoutingHelper;
  Ptr<Ipv4StaticRouting> remoteHostStaticRouting = ipv4RoutingHelper.GetStaticRouting (remoteHost->GetObject<Ipv4> ());
  remoteHostStaticRouting->AddNetworkRouteTo (Ipv4Address ("7.0.0.0"), Ipv4Mask ("255.0.0.0"), 1);
  internet.Install (ueNodes);


  Ptr<Node> ue;
  Ptr<NetDevice> ueLteDevice;
  Ipv4InterfaceContainer ueIpIface;

  for (uint32_t u = 0; u < ueNodes.GetN(); ++u)
    {
      ue = ueNodes.Get (u);
      ueDevs.Add(lteHelper->InstallUeDevice (ueNodes.Get (u)));
      ueLteDevice = ueDevs.Get (u);
      ueIpIface = epcHelper->AssignUeIpv4Address (NetDeviceContainer (ueLteDevice));

      // set the default gateway for the UE
      Ptr<Ipv4StaticRouting> ueStaticRouting = ipv4RoutingHelper.GetStaticRouting (ue->GetObject<Ipv4> ());
      ueStaticRouting->SetDefaultRoute (epcHelper->GetUeDefaultGatewayAddress (), 1);
      
      // why to define the device first and attach to node instead of install to nodes
    }

  lteHelper->Attach (ueDevs, enbDevs.Get (0));

  uint16_t dlPort = 1234;
  uint16_t ulPort = 2000;
  uint16_t VideoPort = 8000;
  uint16_t DummyPort = 9000;


	// Setting up QCI
  for (uint32_t u = 0; u < ueNodes.GetN (); ++u)
    {
      Ptr<NetDevice> ueDevice = ueDevs.Get (u);
      GbrQosInformation qosCommand;
      qosCommand.gbrDl = 300000;  // bit/s, considering IP, UDP, RLC, PDCP header size
      qosCommand.gbrUl = 300000;
      qosCommand.mbrDl = qosCommand.gbrDl;
      qosCommand.mbrUl = qosCommand.gbrUl;
      
      enum EpsBearer::Qci qCommand;
      qCommand = EpsBearer::GBR_CONV_VOICE; //bearerCommand.arp.priorityLevel = 1;
      EpsBearer bearerCommand (qCommand, qosCommand); 
      bearerCommand.arp.preemptionCapability = true;
      bearerCommand.arp.preemptionVulnerability = true;

      Ptr<EpcTft> tftCommand = Create<EpcTft> ();
      EpcTft::PacketFilter pfCommand;
	    pfCommand.localPortStart = dlPort;
	    pfCommand.localPortEnd = dlPort;
	    tftCommand->Add (pfCommand);

	    lteHelper->ActivateDedicatedEpsBearer (ueDevice, bearerCommand, tftCommand);

      GbrQosInformation qosVideo;
      qosVideo.gbrDl = 10000000;  // bit/s, considering IP, UDP, RLC, PDCP header size
      qosVideo.gbrUl = 10000000;
      // qosVideo.mbrDl = qosVideo.gbrDl;
      // qosVideo.mbrUl = qosVideo.gbrUl;

      enum EpsBearer::Qci qVideo;
      qVideo = EpsBearer::GBR_CONV_VOICE; //bearerVedio.arp.priorityLevel = 8;
      EpsBearer bearerVedio (qVideo, qosVideo);
      // bearerVedio.arp.preemptionCapability = true;
      // bearerVedio.arp.preemptionVulnerability = true;

      Ptr<EpcTft> tftVedio = Create<EpcTft> ();
      EpcTft::PacketFilter pfVideo;
	    pfVideo.localPortStart = VideoPort;
	    pfVideo.localPortEnd = VideoPort;
	    tftVedio->Add (pfVideo);

	    lteHelper->ActivateDedicatedEpsBearer (ueDevice, bearerVedio, tftVedio);

      GbrQosInformation qosDummy;
      qosDummy.gbrDl = 10000000;  // bit/s, considering IP, UDP, RLC, PDCP header size
      qosDummy.gbrUl = 10000000;
      qosDummy.mbrDl = qosDummy.gbrDl;
      qosDummy.mbrUl = qosDummy.gbrUl;

      enum EpsBearer::Qci q;
      q = EpsBearer::NGBR_VIDEO_TCP_DEFAULT; // bearerDummy.arp.priorityLevel = 9;
      EpsBearer bearerDummy (q, qosDummy);
      // bearerDummy.arp.preemptionCapability = true;
      // bearerDummy.arp.preemptionVulnerability = true;

      Ptr<EpcTft> tftDummy = Create<EpcTft> ();
      EpcTft::PacketFilter pfDummy;
	    pfDummy.localPortStart = DummyPort;
  	  pfDummy.localPortEnd = DummyPort;
  	  tftDummy->Add (pfDummy);

	  lteHelper->ActivateDedicatedEpsBearer (ueDevice, bearerDummy, tftDummy);	  
      
    }


  for (uint32_t u = 0; u < ueNodes.GetN (); ++u)
  {
    // ++ulPort;
    OnOffHelper dlCommandServer ("ns3::UdpSocketFactory",
                                       InetSocketAddress (ueIpIface.GetAddress (u), dlPort));
    OnOffHelper ulCommandServer ("ns3::UdpSocketFactory",
                                       InetSocketAddress (remoteHostAddr, ulPort));
    dlCommandServer.SetAttribute ("DataRate",DataRateValue (1000));
    ulCommandServer.SetAttribute ("DataRate",DataRateValue (1000));

    ApplicationContainer serverApps; 
    ApplicationContainer clientApps;

    serverApps.Add (dlCommandServer.Install (remoteHost));
    serverApps.Add (ulCommandServer.Install (ueNodes.Get(u)));

    UdpClientHelper dlCommandClient (ueIpIface.GetAddress (u), dlPort);
    UdpClientHelper ulCommandClient (remoteHostAddr, ulPort);

    clientApps.Add (dlCommandClient.Install (ueNodes.Get(u)));
    clientApps.Add (ulCommandClient.Install (remoteHost));
    
    // Server Application Start 
    serverApps.Start (Seconds (0.01));
    // Client Application Start
    clientApps.Start (Seconds (0.01));

    EvalvidServerHelper Videoserver(VideoPort);
    Videoserver.SetAttribute ("SenderTraceFilename", StringValue("st_highway_cif.st"));
    Videoserver.SetAttribute ("SenderDumpFilename", StringValue("sd_a01_lte"));
    ApplicationContainer VideoServerapps = Videoserver.Install(ueNodes.Get(u));  

  	EvalvidClientHelper Videoclient (ueIpIface.GetAddress(u),VideoPort);
  	Videoclient.SetAttribute ("ReceiverDumpFilename", StringValue("rd_a01_lte"));
  	ApplicationContainer VideoClientapps = Videoclient.Install (remoteHost);
  	
    VideoServerapps.Start (Seconds (0.01));
  	VideoServerapps.Stop (Seconds (100.00));
  	VideoClientapps.Start (Seconds (1.01));    
  	VideoClientapps.Stop (Seconds (140.00)); 

    UdpEchoServerHelper DummyServer (DummyPort);     
    ApplicationContainer DummyserverApps = DummyServer.Install (remoteHost);

    UdpEchoClientHelper DummyClient (remoteHostAddr, DummyPort);
    DummyClient.SetAttribute ("MaxPackets", UintegerValue (1000000));
    DummyClient.SetAttribute ("Interval", TimeValue (Seconds (0.0001)));
    DummyClient.SetAttribute ("PacketSize", UintegerValue (1024));
    ApplicationContainer DummyclientApps = DummyClient.Install (ueNodes.Get (u));

    DummyserverApps.Start (Seconds (0.01));
    DummyserverApps.Stop (Seconds (9.00));
    DummyclientApps.Start (Seconds (0.01));
    DummyserverApps.Stop (Seconds (10.00));
  }


  // gnuplot
  string fileNameWithNoExtension = "TimeVSPacket lost ratio";
  string graphicsFileName        = "well.png";
  string plotFileName            = "123.plt";
  string plotTitle               = "Time vs Packet lost ratio";
  string dataTitle               = "Packet lost ratio";

  // Monitoring Flow Number
  double Flow_Number = 6;

  Gnuplot gnuplot (graphicsFileName);
  gnuplot.SetTitle (plotTitle);
  gnuplot.SetTerminal ("png");
  gnuplot.SetLegend ("Time", "Per Packet delay");

  static Gnuplot2dDataset dataset;
  dataset.SetTitle (dataTitle);
  dataset.SetStyle (Gnuplot2dDataset::LINES_POINTS);
  

  // Continous Print out portal
  Ptr<FlowMonitor> monitor;
  FlowMonitorHelper flowmon_helper;
  monitor = flowmon_helper.InstallAll();
  Simulator::Schedule(Seconds(3),&ThroughputMonitor, &flowmon_helper, monitor, dataset, Flow_Number);

  // // XML output  
  // Ptr<FlowMonitor> flowmon;
  // FlowMonitorHelper flowmonHelper;
  // flowmon = flowmonHelper.Install (ueNodes);
  // flowmon = flowmonHelper.Install (remoteHostContainer);


  // AnimationInterface anim ("animation.xml");
  // anim.SetMaxPktsPerTraceFile (1000000);
  // anim.SetConstantPosition (remoteHost, 0.0, 0.0);
  // anim.SetConstantPosition (pgw, 0.0, 1000.0);  
  
  Simulator::Stop (Seconds (duration));

  // // Radio environment map Helper
  // Ptr<RadioEnvironmentMapHelper> remHelper = CreateObject<RadioEnvironmentMapHelper> ();
  // remHelper->SetAttribute ("ChannelPath", StringValue ("/ChannelList/1"));
  // remHelper->SetAttribute ("OutputFile", StringValue ("rem.out"));
  // remHelper->SetAttribute ("XMin", DoubleValue (0.0));
  // remHelper->SetAttribute ("XMax", DoubleValue (2000.0));
  // remHelper->SetAttribute ("XRes", UintegerValue (100));
  // remHelper->SetAttribute ("YMin", DoubleValue (3000.0));
  // remHelper->SetAttribute ("YMax", DoubleValue (5000.0));
  // remHelper->SetAttribute ("YRes", UintegerValue (75));
  // remHelper->SetAttribute ("Z", DoubleValue (1.0));
  // remHelper->SetAttribute ("RbId", IntegerValue (1));
  // remHelper->SetAttribute ("MaxPointsPerIteration", UintegerValue (100000000));
  // remHelper->Install ();


  Simulator::Run ();
  
  // monitor->CheckForLostPackets ();

  // Screen Print out
  ThroughputMonitor (&flowmon_helper, monitor, dataset, Flow_Number);

  // XML Output
  monitor->SerializeToXmlFile ("LTE", false, false);

  gnuplot.AddDataset (dataset);
  ofstream plotFile (plotFileName.c_str());
  gnuplot.GenerateOutput (plotFile);
  plotFile.close ();  	


  Simulator::Destroy ();
  os.close (); // close log file
  return 0;
}
