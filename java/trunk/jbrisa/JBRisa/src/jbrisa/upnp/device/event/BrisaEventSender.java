package jbrisa.upnp.device.event;

import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;

import jbrisa.utils.BrisaLogger;
import jbrisa.utils._Constants;


public class BrisaEventSender {

	public static void sendMulticastMessage(String m) {

		try {
			byte[] buffer = m.getBytes();
			DatagramPacket aliveDatagram = new DatagramPacket(buffer,
					buffer.length, InetAddress
					.getByName(_Constants.DEFAULT_MULTICAST_EVENT_IP), _Constants.DEFAULT_MULTICAST_EVENT_PORT);

			// Send datagram;
			DatagramSocket udpListener = new DatagramSocket();
			udpListener.send(aliveDatagram);
			
			BrisaLogger.logger.info("Multicast event message sent");
			
		} catch (Exception e) {
			
			BrisaLogger.logger.warning("Problem while sending the multicast event message. "+e.getMessage());
			
			e.printStackTrace();
		}
	}
	
	public static void sendUnicastMessage(String m, String ip, int port) {

		try {
			byte[] buffer = m.getBytes();
			
			DatagramPacket aliveDatagram = new DatagramPacket(buffer,
					buffer.length, InetAddress
					.getByName(ip), port);

			// Send datagram;
			DatagramSocket udpListener = new DatagramSocket();
			udpListener.send(aliveDatagram);
			
			BrisaLogger.logger.info("Unicast event message sent");
			
		} catch (Exception e) {
			
			BrisaLogger.logger.warning("Problem while sending the multicast event message. "+e.getMessage());
			
			e.printStackTrace();
		}
	}
}
