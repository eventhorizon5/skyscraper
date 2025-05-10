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
		packets = 0;

		unsigned char packetIdentifier;
		SLNet::SystemAddress clientID = SLNet::UNASSIGNED_SYSTEM_ADDRESS;

		int ClientPort = 5121;
		int ServerPort = 5120;
		std::string ipaddress = "127.0.0.1";

		SLNet::SocketDescriptor socketDescriptor(static_cast<unsigned short>(ClientPort),0);
		socketDescriptor.socketFamily=AF_INET;
		client->Startup(8, &socketDescriptor, 1);
		client->SetOccasionalPing(true);

		SLNet::ConnectionAttemptResult ConnectionResult = client->Connect(ipaddress.c_str(), static_cast<unsigned short>(ServerPort), "skyscraper", (int) strlen("skyscraper"));
		RakAssert(ConnectionResult == SLNet::CONNECTION_ATTEMPT_STARTED);

		std::string message;

    }

    ChatClient::~ChatClient()
    {

    }
}
