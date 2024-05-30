/*
 * Copyright (c) 2015 Universita' degli Studi di Napoli "Federico II"
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
 * Author: Pasquale Imputato <p.imputato@gmail.com>
 * Author: Stefano Avallone <stefano.avallone@unina.it>
 */

#include "ns3/applications-module.h"
#include "ns3/core-module.h"
#include "ns3/flow-monitor-module.h"
#include "ns3/internet-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/traffic-control-module.h"

// This simple example shows how to use TrafficControlHelper to install a
// QueueDisc on a device.
//
// The default QueueDisc is a pfifo_fast with a capacity of 1000 packets (as in
// Linux). However, in this example, we install a RedQueueDisc with a capacity
// of 10000 packets.
//
// Network topology
//
//       10.1.1.0
// n0 -------------- n1
//    point-to-point
//
// The output will consist of all the traced changes in the length of the RED
// internal queue and in the length of the netdevice queue:
//
//    DevicePacketsInQueue 0 to 1
//    TcPacketsInQueue 7 to 8
//    TcPacketsInQueue 8 to 9
//    DevicePacketsInQueue 1 to 0
//    TcPacketsInQueue 9 to 8
//
// plus some statistics collected at the network layer (by the flow monitor)
// and the application layer. Finally, the number of packets dropped by the
// queuing discipline, the number of packets dropped by the netdevice and
// the number of packets requeued by the queuing discipline are reported.
//
// If the size of the DropTail queue of the netdevice were increased from 1
// to a large number (e.g. 1000), one would observe that the number of dropped
// packets goes to zero, but the latency grows in an uncontrolled manner. This
// is the so-called bufferbloat problem, and illustrates the importance of
// having a small device queue, so that the standing queues build in the traffic
// control layer where they can be managed by advanced queue discs rather than
// in the device layer.

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("TrafficControlExample");

/**
 * Number of packets in TX queue trace.
 *
 * \param oldValue Old velue.
 * \param newValue New value.
 */
void
TcPacketsInQueueTrace(uint32_t oldValue, uint32_t newValue)
{
    std::cout << "TcPacketsInQueue " << oldValue << " to " << newValue << std::endl;
}

/**
 * Packets in the device queue trace.
 *
 * \param oldValue Old velue.
 * \param newValue New value.
 */
void
DevicePacketsInQueueTrace(uint32_t oldValue, uint32_t newValue)
{
    std::cout << "DevicePacketsInQueue " << oldValue << " to " << newValue << std::endl;
}

/**
 * TC Soujoun time trace.
 *
 * \param sojournTime The soujourn time.
 */
void
SojournTimeTrace(Time sojournTime)
{
    std::cout << "Sojourn time " << sojournTime.ToDouble(Time::MS) << "ms" << std::endl;
}

int
main(int argc, char* argv[])
{
    double simulationTime = 10; // seconds
    std::string transportProt = "Udp";
    std::string socketType;


    if (transportProt == "Tcp")
    {
        socketType = "ns3::TcpSocketFactory";
    }
    else
    {
        socketType = "ns3::UdpSocketFactory";
    }

    NodeContainer nodes;
    nodes.Create(7);

    PointToPointHelper pointToPoint;
    pointToPoint.SetDeviceAttribute("DataRate", StringValue("10Mbps"));
    pointToPoint.SetChannelAttribute("Delay", StringValue("2ms"));
    pointToPoint.SetQueue("ns3::DropTailQueue", "MaxSize", StringValue("1p"));

    NetDeviceContainer devices02;
    devices02 = pointToPoint.Install(nodes.Get(0),nodes.Get(2));
    
    NetDeviceContainer devices21;
    devices21 = pointToPoint.Install(nodes.Get(2),nodes.Get(1));
    
    NetDeviceContainer devices23;
    devices23 = pointToPoint.Install(nodes.Get(2),nodes.Get(3));
    
    NetDeviceContainer devices15;
    devices15 = pointToPoint.Install(nodes.Get(1),nodes.Get(5));
    
    NetDeviceContainer devices34;
    devices34 = pointToPoint.Install(nodes.Get(3),nodes.Get(4));
    
     NetDeviceContainer devices56;
    devices56 = pointToPoint.Install(nodes.Get(5),nodes.Get(6));
    
     NetDeviceContainer devices46;
    devices46 = pointToPoint.Install(nodes.Get(4),nodes.Get(6));
    
    
    

    
    InternetStackHelper stack;
    stack.Install(nodes);
    
    Ipv4AddressHelper address;
    
    address.SetBase("10.1.1.0", "255.255.255.0");
    Ipv4InterfaceContainer interfaces02 = address.Assign(devices02);
    
    address.SetBase("10.1.2.0", "255.255.255.0");
    Ipv4InterfaceContainer interfaces21 = address.Assign(devices21);
     
    address.SetBase("10.1.3.0", "255.255.255.0"); 
    Ipv4InterfaceContainer interfaces23 = address.Assign(devices23);
    
    address.SetBase("10.1.4.0", "255.255.255.0");
    Ipv4InterfaceContainer interfaces15 = address.Assign(devices15);
     
    address.SetBase("10.1.5.0", "255.255.255.0"); 
    Ipv4InterfaceContainer interfaces34 = address.Assign(devices34);
    
    address.SetBase("10.1.6.0", "255.255.255.0"); 
    Ipv4InterfaceContainer interfaces56 = address.Assign(devices56);
    
    address.SetBase("10.1.7.0", "255.255.255.0"); 
    Ipv4InterfaceContainer interfaces46 = address.Assign(devices46);
     
    Ipv4GlobalRoutingHelper::PopulateRoutingTables();
    
    
    

    // UDP Flow
    uint16_t port = 7;
    Address localAddress(InetSocketAddress(Ipv4Address::GetAny(), port));
    PacketSinkHelper packetSinkHelper(socketType, localAddress);
    ApplicationContainer sinkApp = packetSinkHelper.Install(nodes.Get(0));

    sinkApp.Start(Seconds(0.0));
    sinkApp.Stop(Seconds(simulationTime + 0.1));

    uint32_t payloadSize = 1448;
    Config::SetDefault("ns3::TcpSocket::SegmentSize", UintegerValue(payloadSize));


    OnOffHelper onoff(socketType, Ipv4Address::GetAny());
    onoff.SetAttribute("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=1]"));
    onoff.SetAttribute("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));
    onoff.SetAttribute("PacketSize", UintegerValue(payloadSize));
    onoff.SetAttribute("DataRate", StringValue("50Mbps")); // bit/s
    ApplicationContainer apps;

    InetSocketAddress rmt(interfaces02.GetAddress(0), port);
    rmt.SetTos(0xb8);
    AddressValue remoteAddress(rmt);
    onoff.SetAttribute("Remote", remoteAddress);
    apps.Add(onoff.Install(nodes.Get(4)));
    apps.Start(Seconds(1.0));
    apps.Stop(Seconds(simulationTime + 0.1));

    // TCP Flow
    uint16_t porttcp = 9;
    socketType = "ns3::TcpSocketFactory";
    Address localAddresstcp(InetSocketAddress(Ipv4Address::GetAny(), porttcp ));
    PacketSinkHelper packetSinkHelpertcp (socketType, localAddresstcp);
    ApplicationContainer sinkApptcp = packetSinkHelpertcp.Install(nodes.Get(0));

    sinkApptcp.Start(Seconds(0.5));
    sinkApptcp.Stop(Seconds(simulationTime + 0.1));
    
    OnOffHelper onofftcp(socketType, Ipv4Address::GetAny());
    onofftcp.SetAttribute("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=1]"));
    onofftcp.SetAttribute("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));
    onofftcp.SetAttribute("PacketSize", UintegerValue(payloadSize));
    onofftcp.SetAttribute("DataRate", StringValue("50Mbps")); // bit/s
    ApplicationContainer appstcp;

    InetSocketAddress rmttcp(interfaces02.GetAddress(0), porttcp);
    rmttcp.SetTos(0xb8);
    AddressValue remoteAddresstcp(rmttcp);
    onofftcp.SetAttribute("Remote", remoteAddresstcp);
    appstcp.Add(onofftcp.Install(nodes.Get(5)));
    appstcp.Start(Seconds(1.5));
    appstcp.Stop(Seconds(simulationTime + 0.1));

    FlowMonitorHelper flowmon;
    Ptr<FlowMonitor> monitor = flowmon.InstallAll();
    Simulator::Stop (Seconds (simulationTime + 5));
    Simulator::Run ();

    Ptr<Ipv4FlowClassifier> classifier = DynamicCast<Ipv4FlowClassifier> (flowmon.GetClassifier ());
    std::map<FlowId, FlowMonitor::FlowStats> stats = monitor->GetFlowStats ();
    std::cout << std::endl << "*** Flow monitor statistics ***" << std::endl;
    std::cout << " Dropped Packets: " << stats[1].lostPackets << std::endl;
    for (std::map<FlowId, FlowMonitor::FlowStats>::const_iterator iter = stats.begin (); iter != stats.end
    (); ++iter)
    {
    Ipv4FlowClassifier::FiveTuple t = classifier->FindFlow (iter->first);
    NS_LOG_UNCOND("Flow ID: " << iter->first << " Src Addr " << t.sourceAddress << " Dst Addr " <<
    t.destinationAddress);
    NS_LOG_UNCOND("Tx Packets = " << iter->second.txPackets);
    std::cout << "Rx Packets   = " << iter->second.rxPackets<< std::endl; 
    std::cout << "Lost Packets = " << iter->second.lostPackets<< std::endl; 
    std::cout << "Throughput   = " << iter->second.rxBytes * 8.0 / (iter->second.timeLastRxPacket.GetSeconds()-iter->second.timeFirstTxPacket.GetSeconds()) / 1000000  << "           Kbps"<<  std::endl; 
   
    }

    Simulator::Destroy ();

   return 0;
  }
