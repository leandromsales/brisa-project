package jbrisa.upnp.ssdp;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.InetAddress;
import java.net.MulticastSocket;
import java.net.UnknownHostException;
import java.util.Observable;

import jbrisa.upnp.controlpoint.core.BrisaControlPoint;
import jbrisa.upnp.controlpoint.core.BrisaDeviceCreator;
import jbrisa.upnp.controlpoint.model.BrisaCPDevice;
import jbrisa.utils.BrisaLogger;
import jbrisa.utils.XmlManager;
import jbrisa.utils._Constants;


/**
 * SSDP Client
 * 
 * SSDP stack implementantion for UPnP control points.
 * 
 * Create a new BrisaSSCPClient and call "start()" to connect to the multicast
 * group and start listening to ssdp notification messages.
 * 
 * When BrisaSSDPClient receives a notification message it emits
 * "newDeviceEvent()" in case of "ssdp:alive" and "removedDeviceEvent" in case
 * of "ssdp:byebye". Other ssdp messages will be ignored.
 * 
 * @author Wendell Silva
 * 
 */
public class BrisaSSDPClient extends Observable{

	private boolean running;

	private MulticastSocket ssdpListenerSocket;

	private InetAddress SSDP_ADDR;
	
	private Thread ssdpClientThread;

	//TODO remover brisa control point daqui, criando um listener para essa classe;
	private BrisaControlPoint brisaControlPoint;

	/**
	 * Constructor
	 */
	public BrisaSSDPClient(BrisaControlPoint bcp) {
		running = false;
		this.brisaControlPoint = bcp;
	}

	/**
	 * Start BRisa SSDP Client
	 */
	public void start() {
		
		ssdpClientThread = new Thread(new Runnable() {
			
			@Override
			public void run() {
				
				BrisaLogger.logger.info("Starting Brisa SSDP Client");
				try {
					ssdpListenerSocket = new MulticastSocket(
							_Constants.DEFAULT_MSEARCH_PORT);
					SSDP_ADDR = InetAddress
							.getByName(_Constants.DEFAULT_MSEARCH_IP);
					ssdpListenerSocket.joinGroup(SSDP_ADDR);

					BrisaLogger.logger.info("SSDP Client is running...");
					running = true;

					int datagramSize = _Constants.DEFAULT_DATAGRAM_SIZE;

					while (running) {
						DatagramPacket datagramReceived = new DatagramPacket(
								new byte[datagramSize], datagramSize);

						ssdpListenerSocket.receive(datagramReceived);
						parseMessage(datagramReceived);
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
		
		if (!isRunning()) {
			ssdpClientThread.start();
		} else {
			BrisaLogger.logger.warning("Brisa SSDP Client: Already running!");
		}
	}

	/**
	 * Stop BRisa SSDP Client
	 */
	public void stop() {
		
		if(isRunning()) {
			ssdpClientThread.interrupt();
			running = false;
		} else {
			BrisaLogger.logger.warning("Brisa SSDP Client: Already stopped!");
		}
	}

	/**
	 * 
	 * @return
	 */
	public boolean isRunning() {
		return running;
	}

	// =========================================================================
	// PRIVATE METHODS==========================================================
	// =========================================================================

	/**
	 * Parse the SSDP arrived message;
	 */
	private void parseMessage(DatagramPacket msg) {

		String messsageSSDPRecieved = new String(msg.getData());

		//IF AN ALIVE MESSAGE RECIEVED;
		if (messsageSSDPRecieved.contains(_Constants.SSDP_NTS_ALIVE)) {

			BrisaLogger.logger.info("SSDP alive message recieved.");
			parseAliveMessage(messsageSSDPRecieved);
			
		} 
		//IF AN BYE BYE MESSAGE RECIEVED;
		else if (messsageSSDPRecieved.contains(_Constants.SSDP_NTS_BYE)) {

			BrisaLogger.logger.info("SSDP byebye message recieved.");
			parseByeByeMessage(messsageSSDPRecieved);
			
		} 
		//IF AN UPDATE MESSAGE RECIEVED;
		else if (messsageSSDPRecieved.contains(_Constants.SSDP_NTS_UPDATE)) {

			BrisaLogger.logger.info("SSDP update message recieved.");
			parseUpdateMessage(messsageSSDPRecieved);
		}
	}

	/**
	 * Parse update Message
	 * 
	 * @deprecated
	 * @param messageSSDPReceived
	 */
	private void parseUpdateMessage(String messageSSDPReceived) {
		//CHECK IF DEVICE EXISTS BEFORE CREATE A NEW REPLACE DEVICE;
		/*
		//Create the new device from update message
		BrisaDeviceCreator deviceCreator = new BrisaDeviceCreator();
		Device d = deviceCreator.createDevice(messageSSDPReceived);
		
		System.out.println("MENSAGEM SSDP: \n" + messageSSDPReceived);
		
		if(d != null)
		{
			//Find the correspondent device in the device list and replace it or add it;
			Boolean found = false;
			
			for (Device currentDevice : Singleton.getInstance().arrayDevices) {
				if(currentDevice.UDN.equals(d.UDN)){
					
					currentDevice = d;
					found = true;
					notifyAllObservers();
				}
			}
			
			if(!found){
				
				Singleton.getInstance().arrayDevices.add(d);
				Singleton.logger.info("New device added via update message.");
			}
		}
		*/
	}

	/**
	 * Parse alive message
	 * 
	 * @param aliveMessage
	 */
	private void parseAliveMessage(String aliveMessage){
		
		Boolean found = false;
		
		String locationFromSSDP = XmlManager.getLocationFromSSDP(aliveMessage);
		Integer maxAgeFromSSDP = XmlManager.getMaxAgeFromSSDP(aliveMessage);
		
		for (BrisaCPDevice deviceUpdate : brisaControlPoint.getArrayDevices()) {
			
			if(deviceUpdate.location.contains(locationFromSSDP))
			{
				deviceUpdate.maxAge = maxAgeFromSSDP;
				deviceUpdate.startTimer();
				
				BrisaLogger.logger.info("Max age updated to "+maxAgeFromSSDP+ " in device "+deviceUpdate.friendlyName);
				
				found = true;
				break;
			}
		}
		
		if(!found){
			
			BrisaLogger.logger.info("No device found for the location "+locationFromSSDP);
			BrisaLogger.logger.info("Adding device to the device's list "+locationFromSSDP);
			
			BrisaDeviceCreator deviceCreator = new BrisaDeviceCreator(brisaControlPoint);
			BrisaCPDevice d = deviceCreator.createDevice(aliveMessage);
			
			if(d != null)
			{
				notifyAllObservers(d);
			}
		}
	}

	/**
	 * Parse Bye-Bye message
	 * 
	 * @param byebyeMessage
	 */
	private void parseByeByeMessage(String byebyeMessage) {
		
		brisaControlPoint.removeDeviceFromByeInSSDPClient(byebyeMessage);
		/*
		Boolean found = false;
		String uuidFromSSDP = XmlManager.getUUIDFromSSDP(byebyeMessage);
		
		for (BrisaCPDevice device : brisaControlPoint.getArrayDevices()) {
			
			if(device.UDN.contains(uuidFromSSDP)){
				
				device.stopTimer();
				
				brisaControlPoint.getArrayDevices().remove(device);
				
				BrisaLogger.logger.info("Device removed. "+device.friendlyName);
				
				found = true;
				
				break;
			}
		}
		
		if(found){
			
			this.setChanged();
			this.notifyObservers();
		}
		else{
			
			BrisaLogger.logger.info("Coudn't remove the device 'cause the device wasn't found for this uuid :"+uuidFromSSDP);
		}
		*/
	}
	
	/**
	 * Notify all Observers
	 */
	private void notifyAllObservers(BrisaCPDevice d){
		
		this.setChanged();
		this.notifyObservers(d);
	}
	
	
}
