/** Copyright (C) 2006, Ian Paul Larsen.
 **
 **  This program is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  This program is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 **/

// Network opcodes.
//
// Interpreter::execByteCode() in Interpreter.cpp decodes each opcode and
// hands these ones to execNetworkOp().  The cases are exactly as they were
// there: a break still ends the opcode, and the checks execByteCode()
// makes after every opcode still run when this returns.

#include "InterpreterPrivate.h"

void Interpreter::execNetworkOp(int opcode) {
	switch(opcode) {

#ifdef BASIC256_ENABLE_TCP
		case OP_NETLISTEN: {
		    int port = stack->popInt();
    				int fn   = stack->popInt();

    				if (fn < 0 || fn >= NUMSOCKETS) {
        				error->q(ERROR_NETSOCKNUMBER);
    				} else {
        				// Close any existing socket on this slot
        				if (sockets[fn]) {
            				sockets[fn]->disconnectFromHost();
            				sockets[fn]->close();
            				sockets[fn]->deleteLater();
            				sockets[fn] = nullptr;
        				}

        				QTcpServer server;
        				// Loopback by default: an inbound port open on every interface is a
        				// backdoor on this machine, and classroom use only needs this host.
        				QHostAddress bindto = settingsNetListenAny ? QHostAddress::Any : QHostAddress::LocalHost;
        				if (!server.listen(bindto, port)) {
            				error->q(ERROR_NETBIND, server.errorString());
        				} else {
            				// Block here waiting for one incoming connection
            				if (!server.waitForNewConnection(30000)) {  // 30 second timeout
                				error->q(ERROR_NETACCEPT, server.errorString());
            				} else {
                				sockets[fn] = server.nextPendingConnection();
                				// Detach socket from server so it survives server going out of scope
                				sockets[fn]->setParent(nullptr);
            				}
        				}
        				server.close();
    				}
		}
		break;

		case OP_NETCONNECT: {
    				int port       = stack->popInt();
    				QString address = stack->popQString();
    				int fn         = stack->popInt();

    				if (fn < 0 || fn >= NUMSOCKETS) {
        				error->q(ERROR_NETSOCKNUMBER);
    				} else {
        				// Close any existing connection on this slot
        				if (sockets[fn]) {
            				sockets[fn]->disconnectFromHost();
            				sockets[fn]->close();
            				sockets[fn]->deleteLater();
            				sockets[fn] = nullptr;
        				}

        				// Create a new socket and attempt connection
        				QTcpSocket *sock = new QTcpSocket();
        				sock->connectToHost(address, port);

        				if (!sock->waitForConnected(30000)) {  // 30 second timeout
            				// Host lookup failed or connection refused
            				if (sock->error() == QAbstractSocket::HostNotFoundError) {
               				 error->q(ERROR_NETHOST, sock->errorString());
            				} else {
                				error->q(ERROR_NETCONN, sock->errorString());
            				}
            				delete sock;
        				} else {
            				sockets[fn] = sock;
        				}
    				}
		}
		break;

		case OP_NETREAD: {
    				int fn = stack->popInt();
    				if (fn < 0 || fn >= NUMSOCKETS) {
       					error->q(ERROR_NETSOCKNUMBER);
        				stack->pushQString("");
    				} else {
        				if (!sockets[fn]) {
            				error->q(ERROR_NETNONE);
            				stack->pushQString("");
        				} else {
            				// Wait up to 1 second for data to arrive if none buffered yet
            				if (sockets[fn]->bytesAvailable() == 0) {
                				sockets[fn]->waitForReadyRead(1000);
            				}
            				if (sockets[fn]->bytesAvailable() == 0) {
                				error->q(ERROR_NETREAD, sockets[fn]->errorString());
                				stack->pushQString("");
            				} else {
                				QByteArray data = sockets[fn]->read(2048);
                				stack->pushQString(QString::fromUtf8(data));
            				}
        				}
    				}
		}
		break;

		case OP_NETWRITE: {
    				QString data = stack->popQString();
    				int fn = stack->popInt();
    				if (fn < 0 || fn >= NUMSOCKETS) {
        				error->q(ERROR_NETSOCKNUMBER);
    				} else {
        				if (!sockets[fn]) {
            				error->q(ERROR_NETNONE);
        				} else {
            				QByteArray bytes = data.toUtf8();
            				qint64 n = sockets[fn]->write(bytes);
            				if (n < 0) {
                				error->q(ERROR_NETWRITE, sockets[fn]->errorString());
            				} else {
                				// Flush to ensure data is actually sent before continuing
                				sockets[fn]->flush();
            				}
        				}
    				}
		}
		break;

		case OP_NETCLOSE: {
    				int fn = stack->popInt();
    				if (fn < 0 || fn >= NUMSOCKETS) {
        				error->q(ERROR_NETSOCKNUMBER);
    				} else {
        				if (!sockets[fn]) {
            				error->q(ERROR_NETNONE);
        				} else {
            				netSockClose(fn);   // pass the slot index, not a file descriptor
        				}
    				}
		}
		break;

		case OP_NETDATA: {
    				// Push 1 if there is data available to read, 0 if not
    				int fn = stack->popInt();
    				if (fn < 0 || fn >= NUMSOCKETS) {
        				stack->pushInt(0);
        				error->q(ERROR_NETSOCKNUMBER);
    				} else {
        				if (!sockets[fn]) {
            				stack->pushInt(0);
            				error->q(ERROR_NETNONE);
        				} else {
            				// Mirror the original: wait up to 1ms before deciding no data
            				if (sockets[fn]->bytesAvailable() == 0) {
                				sockets[fn]->waitForReadyRead(1);
            				}
            				stack->pushInt(sockets[fn]->bytesAvailable() > 0 ? 1 : 0);
       					}
    				}
		}
		break;

		case OP_NETADDRESS: {
    				// Return the first non-loopback IPv4 address of this machine.
    				// QNetworkInterface works identically on Windows, Linux, macOS, and Android —
    				// the entire #ifdef WIN32 / #ifdef ANDROID / #else split is gone.
    				QString found;
    				const QList<QNetworkInterface> ifaces = QNetworkInterface::allInterfaces();
    				for (const QNetworkInterface &iface : ifaces) {
        				// Skip loopback and interfaces that are not up
        				if (iface.flags().testFlag(QNetworkInterface::IsLoopBack)) continue;
        				if (!iface.flags().testFlag(QNetworkInterface::IsUp)) continue;
        				for (const QNetworkAddressEntry &entry : iface.addressEntries()) {
            				if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol) {
                				found = entry.ip().toString();
                				break;
            				}
        				}
        				if (!found.isEmpty()) break;
    				}
    				if (found.isEmpty()) {
        				// Fall back to loopback, matching original behaviour on failure
        				found = QStringLiteral("127.0.0.1");
    				}
    				stack->pushQString(found);
		}
		break;
#else
		case OP_NETLISTEN: {
			stack->popInt();		// port
			stack->popInt();		// fn
			error->q(ERROR_NOTAVAILABLE);
		}
		break;

		case OP_NETCONNECT: {
			stack->popInt();		// port
			stack->popQString();	// address
			stack->popInt();		// fn
			error->q(ERROR_NOTAVAILABLE);
		}
		break;

		case OP_NETREAD: {
			stack->popInt();		// fn
			error->q(ERROR_NOTAVAILABLE);
			stack->pushQString("");
		}
		break;

		case OP_NETWRITE: {
			stack->popQString();	// data
			stack->popInt();		// fn
			error->q(ERROR_NOTAVAILABLE);
		}
		break;

		case OP_NETCLOSE: {
			stack->popInt();		// fn
			error->q(ERROR_NOTAVAILABLE);
		}
		break;

		case OP_NETDATA: {
			stack->popInt();		// fn
			error->q(ERROR_NOTAVAILABLE);
			stack->pushInt(0);
		}
		break;

		case OP_NETADDRESS: {
			error->q(ERROR_NOTAVAILABLE);
			stack->pushQString("");
		}
		break;
#endif

		case OP_FREENET: {
			// return the next free network socket number - throw error if not free sockets
			int f=-1;
			for (int t=0; (t<NUMSOCKETS)&&(f==-1); t++) {
				if (sockets[t] == nullptr) f = t;
			}
			if (f==-1) {
				error->q(ERROR_FREENET);
				stack->pushInt(0);
			} else {
				stack->pushInt(f);
			}
		}
		break;

	}
}
