/*
	Skyscraper 2.1 - Chat Client
	Copyright (C)2025 Ryan Thoryk
	https://www.skyscrapersim.net
	https://sourceforge.net/projects/skyscraper/
	Contact - ryan@skyscrapersim.net

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#include <string>
#include "slikenet/peerinterface.h"
#include "slikenet/statistics.h"
#include "chatclient.h"

#if LIBCAT_SECURITY==1
#include "slikenet/SecureHandshake.h"
#endif

namespace Skyscraper::Network
{
    ChatClient::ChatClient()
    {
		client = SLNet::RakPeerInterface::GetInstance();
		stats = 0;
		ClientPort = 5121;
		ServerPort = 5120;

		SLNet::SystemAddress ClientAddress = SLNet::UNASSIGNED_SYSTEM_ADDRESS;

		SLNet::SocketDescriptor desc(static_cast<unsigned short>(ClientPort), 0);
		desc.socketFamily = AF_INET;
		Startup(&desc);
	}

    ChatClient::~ChatClient()
    {

    }

	void ChatClient::Connect()
	{
		std::string ip_address = "127.0.0.1";
		SLNet::ConnectionAttemptResult ConnectionResult = client->Connect(ip_address.c_str(), static_cast<unsigned short>(ServerPort), "skyscraper", (int) strlen("skyscraper"));
		RakAssert(ConnectionResult == SLNet::CONNECTION_ATTEMPT_STARTED);
    }

	void ChatClient::Disconnect()
	{
		client->CloseConnection(client->GetSystemAddressFromIndex(0), false);
	}

	void ChatClient::Shutdown()
	{
		client->Shutdown(100);
	}

	void ChatClient::Startup(SLNet::SocketDescriptor *desc)
	{
		bool result = client->Startup(8, desc, 1) == SLNet::RAKNET_STARTED;
		client->SetOccasionalPing(true);
	}

	void ChatClient::GetStats()
	{
		stats = client->GetStatistics(client->GetSystemAddressFromIndex(0));
		char message [2048];
		StatisticsToString(stats, message, 2048, 2);
	}

	void ChatClient::Ping()
	{
		if(client->GetSystemAddressFromIndex(0) != SLNet::UNASSIGNED_SYSTEM_ADDRESS)
			client->Ping(client->GetSystemAddressFromIndex(0));
	}

	void ChatClient::Send(const std::string &message)
	{
		client->Send(message.c_str(), message.size() + 1, HIGH_PRIORITY, RELIABLE_ORDERED, 0, SLNet::UNASSIGNED_SYSTEM_ADDRESS, true);
	}

	void ChatClient::Receive()
	{
		SLNet::Packet* packets;
		for (packets = client->Receive(); packets; client->DeallocatePacket(packets), packets = client->Receive())
		{
			printf("%s\n", packets->data);
		}
	}

}
