package jbrisa.upnp.controlpoint.event;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.InetAddress;
import java.net.MulticastSocket;
import java.net.UnknownHostException;
import java.util.Observable;

import jbrisa.upnp.controlpoint.model.BrisaCPMulticastMessage;
import jbrisa.utils.BrisaLogger;
import jbrisa.utils.XmlManager;

import org.jdom.Element;


public class BrisaMulticastEventReceiver extends Observable {

	/*
	 * ! Socket listening to 239.255.255.246:7900.
	 */
	private InetAddress group;
	private MulticastSocket listener;

	private boolean running;
	private Thread multicastEventReceiverThread;

	public BrisaMulticastEventReceiver() {
		this.running = false;
	}

	/**
	 * Start Multicast Receiver
	 */
	public void start() {

		if (isRunning()) {
			BrisaLogger.logger
					.warning("BRisa Multicast Event Receiver already Running!");
			return;
		}

		multicastEventReceiverThread = new Thread(new Runnable() {

			@Override
			public void run() {

				try {
					BrisaLogger.logger
							.info("Starting BRisa Multicast Event Receiver");
					group = InetAddress.getByName("239.255.255.246");
					listener = new MulticastSocket(7900);
					listener.joinGroup(group);
					running = true;
					while (running) {
						DatagramPacket datagramReceived = new DatagramPacket(
								new byte[2048], 2048);
						listener.receive(datagramReceived);
						parse(datagramReceived);
					}
				} catch (UnknownHostException e) {
					running = false;
					e.printStackTrace();
				} catch (IOException e) {
					running = false;
					e.printStackTrace();
				}

			}
		});

		multicastEventReceiverThread.start();

	}

	/**
	 * Stop Multicast Receiver
	 */
	public void stop() {
		if (!isRunning()) {
			BrisaLogger.logger
					.warning("BRisa Multicast Event Receiver already Running!");
			return;
		}
		BrisaLogger.logger.info("Stopping BRisa Multicast Event Receiver");
		multicastEventReceiverThread.interrupt();
		running = false;

	}

	/**
	 * 
	 * @return
	 */
	public boolean isRunning() {
		return running;
	}

	/**
	 * 
	 * @param message
	 */
	public void parse(DatagramPacket message) {

		String[] auxLines = (new String(message.getData())).split("\r\n\r\n");
		String multicastXml = "";
		String httpHeader = "";

		for (String string : auxLines) {
			if (string.startsWith("<")) {
				multicastXml = string.replaceAll("\r\n", "");
				break;
			} else if (string.startsWith("NOTIFY")) {
				httpHeader = string;
			}
		}

		BrisaLogger.logger.info("Multicast event recieved: " + multicastXml);

		Element root = XmlManager.parseStringToXmlElement(multicastXml.trim());

		//multicastXml = multicastXml.trim();

		// SAXBuilder builder = new SAXBuilder();
		// builder.setIgnoringElementContentWhitespace(true);
		//
		// Reader in = new StringReader(multicastXml);
		// Document doc = builder.build(in);
		// root = doc.getRootElement();

		if (root != null) {

			BrisaCPMulticastMessage multicastMessage = new BrisaCPMulticastMessage();
			
			// fill the name and value;
			
			for (Object property : root.getChildren()) {

				for (Object argument : ((Element) property).getChildren()) {

					multicastMessage.setName(((Element) argument).getName());
					multicastMessage.setValue(((Element) argument).getValue());
					break;
				}
			}
			
			// parse the httpheader;
			for (String string : httpHeader.split("\r\n")) {
				
				if(string.startsWith("USN:"))
				{
					multicastMessage.setUsn(string.replace("USN:", "").trim());
				}
				else if(string.startsWith("SVCID:"))
				{
					multicastMessage.setSvcid(string.replace("SVCID:", "").trim());
				}
				else if(string.startsWith("SEQ:"))
				{
					multicastMessage.setSeq(string.replace("SEQ:", "").trim());
				} 
				else if(string.startsWith("LVL:"))
				{
					multicastMessage.setSvcid(string.replace("LVL:", "").trim());
				} 
				
			}
			
			multicastReceived(multicastMessage);
		}
	}

	/**
	 * 
	 * @param multicastMessage
	 */
	private void multicastReceived(BrisaCPMulticastMessage multicastMessage) {
		setChanged();
		notifyObservers(multicastMessage);
	}

}
