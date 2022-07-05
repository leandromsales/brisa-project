package jbrisa.upnp.controlpoint.event;

import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.util.Observable;

import jbrisa.upnp.controlpoint.model.BrisaCPUnicastMessage;
import jbrisa.utils.BrisaLogger;
import jbrisa.utils.XmlManager;

import org.jdom.Element;


public class BrisaUnicastEventReceiver extends Observable{

	private Integer port;
	private String ipAdress;

	public BrisaUnicastEventReceiver(String ip, Integer port) {
		this.ipAdress = ip;
		this.port = port;
	}

	public void start() {

		new Thread(new Runnable() {

			@Override
			public void run() {

				// String eventMessage = "";
				// ServerSocket serverSocket;

				try {

					DatagramSocket serverSocket1 = new DatagramSocket(port);
					byte[] receiveData = new byte[1024];
					while (true) {

						DatagramPacket receivePacket = new DatagramPacket(
								receiveData, receiveData.length);
						serverSocket1.receive(receivePacket);
						String sentence = new String(receivePacket.getData());

						parse(sentence);
					}

				} catch (Exception e) {
					// TODO: handle exception
				}

				/*
				 * try { serverSocket = new ServerSocket(port);
				 * BrisaLogger.logger.info("Brisa Event Receiver started.");
				 * BrisaLogger.logger.info("Address: " + ipAdress + ":" + port);
				 * 
				 * while (true) { Socket client = serverSocket.accept();
				 * BufferedReader in = new BufferedReader( new
				 * InputStreamReader(client.getInputStream()));
				 * 
				 * String line = in.readLine();
				 * 
				 * while(line != null && !line.equalsIgnoreCase("\r\n")) {
				 * eventMessage += line + "\r\n"; line = in.readLine(); }
				 * 
				 * System.out.println("Mensagem unicast recebida: " +
				 * eventMessage);
				 * 
				 * } } catch (IOException e) { e.printStackTrace(); }
				 */
			}

		}).start();
	}

	public void parse(String message) {

		String[] auxLines = (message).split("\r\n\r\n");
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

		if (root != null) {

			BrisaCPUnicastMessage unicastMessage = new BrisaCPUnicastMessage();

			for (Object property : root.getChildren()) {

				for (Object argument : ((Element) property).getChildren()) {

					unicastMessage.setName(((Element) argument).getName());
					unicastMessage.setValue(((Element) argument).getValue());
					break;
				}
			}

			// parse the httpheader;
			/*
			 * for (String string : httpHeader.split("\r\n")) {
			 * 
			 * if(string.startsWith("USN:")) {
			 * multicastMessage.setUsn(string.replace("USN:", "").trim()); }
			 * else if(string.startsWith("SVCID:")) {
			 * multicastMessage.setSvcid(string.replace("SVCID:", "").trim()); }
			 * else if(string.startsWith("SEQ:")) {
			 * multicastMessage.setSeq(string.replace("SEQ:", "").trim()); }
			 * else if(string.startsWith("LVL:")) {
			 * multicastMessage.setSvcid(string.replace("LVL:", "").trim()); } }
			 */

			multicastReceived(unicastMessage);
		}
	}

	/**
	 * 
	 * @param multicastMessage
	 */
	private void multicastReceived(BrisaCPUnicastMessage unicast) {
		setChanged();
		notifyObservers(unicast);
	}

}
