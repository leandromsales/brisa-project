package jbrisa.upnp.ssdp;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.MulticastSocket;
import java.net.UnknownHostException;
import java.util.Observable;

import jbrisa.utils.BrisaLogger;
import jbrisa.utils._Constants;


/**
 * SSDP Server
 * 
 * Call a start() to begin listening for MSearch requests from control points.
 * Whenever a new msearch request is parsed by the BrisaSSDPServer,
 * 
 * BrisaSSDPServer also implements SSDP notify messages. Call a doNotify() or a
 * doByeBye() when entering or leaving the multicast group.
 * 
 * @author Wendell Silva
 * 
 */
public class BrisaSSDPServer extends Observable {

	private boolean running;

	private Thread ssdpServerThread;
	private MulticastSocket ssdpListenerSocket;
	private DatagramSocket udpListener;

	private InetAddress SSDP_ADDR;

	/**
	 * Costructor
	 */
	public BrisaSSDPServer() {
		this.running = false;
	}

	/**
	 * Start the SSDP Server
	 */
	public void start() {

		ssdpServerThread = new Thread(new Runnable() {

			@Override
			public void run() {

				BrisaLogger.logger.info("Starting Brisa SSDP Server");

				try {
					ssdpListenerSocket = new MulticastSocket(
							_Constants.DEFAULT_MSEARCH_PORT);
					SSDP_ADDR = InetAddress
							.getByName(_Constants.DEFAULT_MSEARCH_IP);
					ssdpListenerSocket.joinGroup(SSDP_ADDR);

					BrisaLogger.logger.info("SSDP Server is running...");
					running = true;

					int datagramSize = _Constants.DEFAULT_DATAGRAM_SIZE;
					udpListener = new DatagramSocket();

					while (running) {

						DatagramPacket datagramReceived = new DatagramPacket(
								new byte[datagramSize], datagramSize);

						ssdpListenerSocket.receive(datagramReceived);

						setChanged();
						notifyObservers(datagramReceived);
					}
				} catch (UnknownHostException e) {
					e.printStackTrace();
				} catch (IOException e) {
					e.printStackTrace();
				} finally {
					running = false;
				}
			}
		});

		if (!isRunning()) {
			ssdpServerThread.start();
		} else {
			BrisaLogger.logger.warning("Brisa SSDP Client: Already running!");
		}
	}

	/**
	 * 
	 */
	public void stop() {
		if (isRunning()) {
			BrisaLogger.logger.info("Stopping Brisa SSDP Server");
			ssdpServerThread.interrupt();
			running = false;
		} else {
			BrisaLogger.logger.warning("Brisa SSDP Server: Already stopped!");
		}
	}

	public boolean isRunning() {
		return running;
	}

	/**
	 * Respond the M-SEARCH request
	 * 
	 * @param senderIp
	 * @param senderPort
	 * @param cacheControl
	 * @param date
	 * @param location
	 * @param server
	 * @param st
	 * @param usn
	 */
	public void respondMSearch(String senderIp, int senderPort,
			String cacheControl, String date, String location, String server,
			String st, String usn) {

		String message = _Constants.UPNP_MSEARCH_RESPONSE
				.replace("%1", cacheControl).replace("%2", date)
				.replace("%3", location).replace("%4", server)
				.replace("%5", st).replace("%6", usn);

		byte[] buffer = message.getBytes();

		try {

			DatagramPacket responseDatagram = new DatagramPacket(buffer,
					buffer.length, InetAddress.getByName(senderIp), senderPort);

			udpListener.send(responseDatagram);

			BrisaLogger.logger
					.info("BrisaSSDPServer writing msearch response for "
							+ senderIp + ":" + senderPort);
		} catch (IOException e) {
			e.printStackTrace();
		}

	}

	/**
	 * Sends notify message
	 * 
	 * @param usn
	 * @param location
	 * @param st
	 * @param server
	 * @param cacheControl
	 */
	public void doNotify(String usn, String location, String st, String server,
			String cacheControl) {

		String message = _Constants.UPNP_ALIVE_MESSAGE
				.replace("%1", cacheControl).replace("%2", location)
				.replace("%3", st).replace("%4", server).replace("%5", usn);

		byte[] buffer = message.getBytes();
		DatagramPacket aliveDatagram = new DatagramPacket(buffer,
				buffer.length, SSDP_ADDR, _Constants.DEFAULT_MSEARCH_PORT);

		// Send datagram;
		try {
			udpListener.send(aliveDatagram);
			BrisaLogger.logger.info("Alive message sent");
		} catch (IOException e) {
			e.printStackTrace();
		}

		BrisaLogger.logger.info("BrisaSSDPServer writing Notify alive for: "
				+ usn + " ");
	}

	/**
	 * Sends Bye-Bye Message
	 * 
	 * @param usn
	 * @param st
	 */
	public void doByeBye(String usn, String st) {

		String byebyeMessage = _Constants.UPNP_BYEBYE_MESSAGE.replace("%1", st)
				.replace("%2", usn);

		byte[] buffer = byebyeMessage.getBytes();
		DatagramPacket byebyeDatagram = new DatagramPacket(buffer,
				buffer.length, SSDP_ADDR, _Constants.DEFAULT_MSEARCH_PORT);

		// Send datagram;
		try {
			udpListener.send(byebyeDatagram);
			BrisaLogger.logger.info("Bye-Bye message sent");
		} catch (IOException e) {
			e.printStackTrace();
		}

		BrisaLogger.logger.info("BrisaSSDPServer writing Bye-Bye message for: "
				+ usn + " ");

		// final DatagramPacket answer = new DatagramPacket(
		// new byte[_Constants.DEFAULT_DATAGRAM_SIZE],
		// _Constants.DEFAULT_DATAGRAM_SIZE);

	}

}
