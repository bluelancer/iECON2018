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
 */
#include "evalvid-client-server-helper.h"
#include "ns3/evalvid-client.h"
#include "ns3/evalvid-server.h"
//#include "ns3/evalvid-trace-client.h"
#include "ns3/uinteger.h"
#include "ns3/string.h"

namespace ns3 {

EvalvidServerHelper::EvalvidServerHelper ()
{}

EvalvidServerHelper::EvalvidServerHelper (uint16_t port)
{
  m_factory.SetTypeId (EvalvidServer::GetTypeId ());
  //SetAttribute ("RemoteAddress", Ipv4AddressValue (ip));
  SetAttribute ("Port", UintegerValue (port));
}

void
EvalvidServerHelper::SetAttribute (std::string name, const AttributeValue &value)
{
  m_factory.Set (name, value);
}

ApplicationContainer
EvalvidServerHelper::Install (NodeContainer c)
{
  ApplicationContainer apps;
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      Ptr<Node> node = *i;

      m_server = m_factory.Create<EvalvidServer> ();
      node->AddApplication (m_server);
      apps.Add (m_server);

    }
  return apps;
}

Ptr<EvalvidServer>
EvalvidServerHelper::GetServer (void)
{
  return m_server;
}

EvalvidClientHelper::EvalvidClientHelper ()
{}

EvalvidClientHelper::EvalvidClientHelper (Ipv4Address ip,uint16_t port)
{
  m_factory.SetTypeId (EvalvidClient::GetTypeId ());
  SetAttribute ("RemoteAddress", Ipv4AddressValue (ip));
  SetAttribute ("RemotePort", UintegerValue (port));
}

void
EvalvidClientHelper::SetAttribute (std::string name, const AttributeValue &value)
{
  m_factory.Set (name, value);
}

ApplicationContainer
EvalvidClientHelper::Install (NodeContainer c)
{
  ApplicationContainer apps;
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      Ptr<Node> node = *i;
      Ptr<EvalvidClient> client = m_factory.Create<EvalvidClient> ();
      node->AddApplication (client);
      apps.Add (client);
    }
  return apps;
}

/*
EvalvidTraceServerHelper::EvalvidTraceServerHelper ()
{}

EvalvidTraceServerHelper::EvalvidTraceServerHelper (Ipv4Address address, uint16_t port, std::string filename)
{
  m_factory.SetTypeId (EvalvidTraceClient::GetTypeId ());
  SetAttribute ("RemoteAddress", Ipv4AddressValue (address));
  SetAttribute ("RemotePort", UintegerValue (port));
  SetAttribute ("TraceFilename", StringValue (filename));
}

void
EvalvidTraceServerHelper::SetAttribute (std::string name, const AttributeValue &value)
{
  m_factory.Set (name, value);
}

ApplicationContainer
EvalvidTraceServerHelper::Install (NodeContainer c)
{
  ApplicationContainer apps;
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      Ptr<Node> node = *i;
      Ptr<EvalvidTraceClient> client = m_factory.Create<EvalvidTraceClient> ();
      node->AddApplication (client);
      apps.Add (client);
    }
  return apps;
}
*/
} // namespace ns3
