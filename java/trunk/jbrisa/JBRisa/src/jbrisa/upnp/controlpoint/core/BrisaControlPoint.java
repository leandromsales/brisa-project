package jbrisa.upnp.controlpoint.core;

import java.net.SocketException;
import java.util.ArrayList;
import java.util.Observable;
import java.util.Observer;

import jbrisa.upnp.controlpoint.event.BrisaEventProxy;
import jbrisa.upnp.controlpoint.event.BrisaMulticastEventReceiver;
import jbrisa.upnp.controlpoint.event.BrisaUnicastEventReceiver;
import jbrisa.upnp.controlpoint.model.BrisaCPActionFault;
import jbrisa.upnp.controlpoint.model.BrisaCPArgumentValue;
import jbrisa.upnp.controlpoint.model.BrisaCPDevice;
import jbrisa.upnp.controlpoint.model.BrisaCPMulticastMessage;
import jbrisa.upnp.controlpoint.model.BrisaCPService;
import jbrisa.upnp.controlpoint.model.BrisaCPSubscriptionAnswer;
import jbrisa.upnp.controlpoint.model.BrisaCPUnicastMessage;
import jbrisa.upnp.event.BrisaControlPointEventManager;
import jbrisa.upnp.ssdp.BrisaSSDPClient;
import jbrisa.utils.BrisaLogger;
import jbrisa.utils.NetworkManager;
import jbrisa.utils.XmlManager;
import jbrisa.utils._Constants;


/**
 * Class that implements the BRisa Control Point.
 * 
 * @author Thiago Ribeiro
 * @author Wendell Silva
 * 
 */
public class BrisaControlPoint extends BrisaControlPointEventManager implements
		Observer {

	private Boolean running;

	private BrisaSSDPClient ssdpClient;

	private BrisaMSearchClient msearchClient;

	private BrisaMulticastEventReceiver multicastReceiver;

	private ArrayList<BrisaCPDevice> arrayDevices;

	private String urlBase;

	private int port;

	private String ipAddress;

	private BrisaUnicastEventReceiver unicastReceiver;
	
	//private int deliveryPath;

	/**
	 * 
	 */
	public BrisaControlPoint() {

		running = false;

		msearchClient = new BrisaMSearchClient();
		msearchClient.addObserver(this);

		ssdpClient = new BrisaSSDPClient(this);
		ssdpClient.addObserver(this);

		multicastReceiver = new BrisaMulticastEventReceiver();
		subscribeMulticastEvents();
		
		arrayDevices = new ArrayList<BrisaCPDevice>();
	}
	
	/**
	 * @return
	 */
	public int getPort() {
		return port;
	}

	/**
	 * @return
	 */
	public String getIpAddress() {
		return ipAddress;
	}

	/**
	 * @return
	 */
	public String getUrlBase() {
		return urlBase;
	}

	/**
	 * 
	 */
	public void start() {

		if (isRunning()) {
			BrisaLogger.logger.warning("BRisa Control Point already Running!");
			return;
		} 
		else 
		{
			try {
				this.ipAddress = NetworkManager.getIPAddress();
				this.port = NetworkManager.getPort();
				this.urlBase = "http://" + this.ipAddress + ":" + this.port;

				// Only have new instance after define ip and port;
				unicastReceiver = new BrisaUnicastEventReceiver(getIpAddress(),getPort());
				unicastReceiver.start();
				unicastReceiver.addObserver(this);	
				
				msearchClient.start();
				ssdpClient.start();
				multicastReceiver.start();

				running = true;
				BrisaLogger.logger.info("Starting BRisa Control Point");

			} catch (SocketException e) {
				BrisaLogger.logger.severe("It wasn't possible obtain an ip adress. "+ e.getMessage());
				e.printStackTrace();
				this.stop();
			}
		}
	}

	/**
	 * 
	 */
	public void discover() {
		if (isRunning()) {
			
			arrayDevices = new ArrayList<BrisaCPDevice>();
			msearchClient.setSearchType(BrisaMSearchClient.DEFAULT_SEARCH_TYPE);
			msearchClient.discover();
		}
	}
	
	/**
	 * @param searchType
	 */
	public void discover(String searchType) {
		if (isRunning()) {
			
			arrayDevices = new ArrayList<BrisaCPDevice>();
			msearchClient.setSearchType(searchType);
			msearchClient.discover();
		}
	}

	/**
	 * 
	 */
	public void stop() {
		if (isRunning()) {
			BrisaLogger.logger.info("Stopping Brisa Control Point");
			msearchClient.stop();
			ssdpClient.stop();
			multicastReceiver.stop();
			this.running = false;
		} else {
			BrisaLogger.logger.warning("Brisa Control Point: Already stopped!");
		}
	}

	/**
	 * 
	 * @return
	 */
	public Boolean isRunning() {
		return running;
	}

	/**
	 * @return
	 */
	public ArrayList<BrisaCPDevice> getArrayDevices() {
		return arrayDevices;
	}

	/**
	 * @return
	 */
	public BrisaMulticastEventReceiver getMulticastReceiver() {
		return multicastReceiver;
	}

	/**
	 * 
	 * @param o
	 */
	/*
	public void subscribeMulticastEvents(Observer o) {
		multicastReceiver.addObserver(o);
	}
	*/

	/**
	 * 
	 */
	public void subscribeMulticastEvents() {
		multicastReceiver.addObserver(this);
	}

	public void unsubscribeMulticastEvents() {
		multicastReceiver.deleteObserver(this);
	}
	
	/**
	 * Method used to print in console all the current devices in the BRisa
	 * devices's list;
	 */
	public void printDevices() {

		BrisaLogger.logger.info("Printing all devices.");
		BrisaLogger.logger.info("There are " + arrayDevices.size());

		for (BrisaCPDevice device : arrayDevices) {

			String print = "\n devicetype: " + device.deviceType
					+ "\n friendlyname: " + device.friendlyName
					+ "\n location: " + device.location + "\n manufacturer: "
					+ device.manufacturer + "\n manufacturerurl: "
					+ device.manufacturerURL + "\n modeldescription: "
					+ device.modelDescription + "\n modelName: "
					+ device.modelName + "\n modelnumber: "
					+ device.modelNumber + "\n modelurl: " + device.modelURL
					+ "\n serialnumber: " + device.serialNumber + "\n udn: "
					+ device.UDN + "\n upc: " + device.UPC
					+ "\n presentationUrl: " + device.presentationURL
					+ "\n urlbase: " + device.urlBase + "\n";

			for (BrisaCPService service : device.serviceList) {
				print += " -- servicetype: " + service.serviceType + "\n"
						+ " -- serviceid: " + service.serviceId + "\n"
						+ " -- scdpurl: " + service.SCPDURL + "\n"
						+ " -- controlurl: " + service.controlURL + "\n"
						+ " -- eventsuburl: " + service.eventSubURL + "\n\n";
			}

			System.out.println(print);
		}
	}

	/**
	 * Gets an event proxy to subscribe, usubscribe or renew the events from a
	 * service.
	 * 
	 * @return
	 */
	public BrisaEventProxy getSubscriptionProxy(BrisaCPService service) {

		//deliveryPath++;

		ArrayList<String> callback = new ArrayList<String>();
		callback.add(this.urlBase);

		String ip = service.urlBase.replaceAll("http://", "").split(":")[0];
		int port = Integer.parseInt(service.urlBase.replaceAll("http://", "")
				.split(":")[1]);
		
		BrisaEventProxy subscription = new BrisaEventProxy(callback,
				_Constants.JBRISA_CP_INFO, ip, port, service.eventSubURL);

		subscription.addObserver(this);
		
		return subscription;

	}

	// =========================================================================
	// PRIVATE METHODS==========================================================
	// =========================================================================

	@SuppressWarnings("unchecked")
	@Override
	public void update(Observable o, Object arg) {

		//IF UPDATE CAME FROM EVENTPROXY, IS A SUBSCRIPTION ANSWER;
		//dispatchSubscriptionAnswerRecieved
		if (o instanceof BrisaEventProxy) {
		
			BrisaCPSubscriptionAnswer b = (BrisaCPSubscriptionAnswer) arg;
			dispatchSubscriptionAnswerRecieved(b);
			
		}
		
		//IF UPDATE CAME FROM A DEVICE, IS A TIMEOUT UPDATE;
		//dispatchEventDeviceRemoved
		else if (o instanceof BrisaCPDevice) {

			BrisaCPDevice d = (BrisaCPDevice) o;
			BrisaLogger.logger.warning("Device " + d.friendlyName+"has timed out.");
			Boolean removed = removeDevice(d);
			
			if(removed)
				dispatchEventDeviceRemoved(d);
		}

		//IF CAME FROM A SERVICE IS AN ACTION WAS INVOKED;
		//dispatchActionResult || dispatchActionFail
		else if (o instanceof BrisaCPService) {

			if (arg instanceof ArrayList<?>) {
				dispatchActionResult((ArrayList<BrisaCPArgumentValue>) arg);
			} else if (arg instanceof BrisaCPActionFault) {
				dispatchActionFail((BrisaCPActionFault) arg);
			}

		}

		// IF UPDATE CAME FROM SSDPCLIENT, IT'S A NEW DEVICE FROM ALIVE;
		//
		else if (o instanceof BrisaSSDPClient) {

			if (arg instanceof BrisaCPDevice) {
				BrisaCPDevice b = (BrisaCPDevice) arg;	
				Boolean added = addDevice(b);
				
				if(added)
					dispatchDeviceAdded(b);
			}
		}

		// IF UPDATE CAME FROM MSEARCHCLIENT, A NEW DEVICE WAS CREATED;
		//dispatchDeviceAdded
		else if (o instanceof BrisaMSearchClient) {

			if (arg instanceof String) {

				BrisaDeviceCreator deviceCreator = new BrisaDeviceCreator(this);
				BrisaCPDevice d = deviceCreator.createDevice((String) arg);

				if (d != null) {
					
					Boolean added = addDevice(d);
					if(added)
						dispatchDeviceAdded(d);
				}
			}
		}
		// IF UPDATE CAME FROM MULTICASTEVENT IS A MULTICAST EVENT
		//dispatchMulticastEventRecieved
		else if (o instanceof BrisaMulticastEventReceiver) {

			dispatchMulticastEventRecieved((BrisaCPMulticastMessage) arg);
		}
		
		if(o instanceof BrisaUnicastEventReceiver)
		{
			dispatchUnicastEventRecieved((BrisaCPUnicastMessage) arg);
		}
		
	}

	private synchronized Boolean addDevice(BrisaCPDevice d) {
		Boolean found = false;

		for (BrisaCPDevice existentDevice : arrayDevices) {
			if (existentDevice.location.equals(d.location)
					&& existentDevice.friendlyName.equals((d.friendlyName))) {
				found = true;
				break;
			}
		}

		if (!found) {
			arrayDevices.add(d);
			d.addObserver(this);

			for (BrisaCPService service : d.serviceList) {
				service.addObserver(this);
			}
			
			return true;
		}
		
		return false;
	}

	private Boolean removeDevice(BrisaCPDevice deviceRemove) {

		Boolean removed = false;
		
		for (BrisaCPDevice device : arrayDevices) {

			if (device.UDN.equals(deviceRemove.UDN)) {

				arrayDevices.remove(device);
				BrisaLogger.logger.warning("Device of id " + deviceRemove.UDN
						+ " found and removed.");
				removed = true;
				break;
			}
		}
		
		return removed;
	}
	
	//remover esse metodo, gambiarra!!!!!
	public void removeDeviceFromByeInSSDPClient(String byebyeMessage)
	{
		String uuidFromSSDP = XmlManager.getUUIDFromSSDP(byebyeMessage);
		
		for (BrisaCPDevice device : getArrayDevices()) {
			
			if(device.UDN.contains(uuidFromSSDP)){
				
				device.stopTimer();
				getArrayDevices().remove(device);
				BrisaLogger.logger.info("Device removed. "+device.friendlyName);
				
				dispatchEventDeviceRemoved(device);
				break;
			}
		}
	}
}
