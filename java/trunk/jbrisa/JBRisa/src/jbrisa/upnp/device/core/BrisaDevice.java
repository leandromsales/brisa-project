package jbrisa.upnp.device.core;

import java.net.DatagramPacket;

import java.net.SocketException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;

import java.util.Observable;
import java.util.Observer;
import java.util.StringTokenizer;

import jbrisa.upnp.ssdp.BrisaSSDPServer;
import jbrisa.utils.BrisaLogger;
import jbrisa.utils.NetworkManager;
import jbrisa.utils.XmlManager;
import jbrisa.utils._Constants;
import jbrisa.webserver.BrisaWebServer;
import jbrisa.webserver.BrisaWebServerFactory;

import org.jdom.Document;
import org.jdom.Element;
import org.jdom.Namespace;


//@SuppressWarnings("unused")
public class BrisaDevice implements Observer {

	private String major;
	private String minor;
	private String urlBase;
	private String deviceType;
	private String friendlyName;
	private String manufacturer;
	private String manufacturerUrl;
	private String modelDescription;
	private String modelName;
	private String modelNumber;
	private String modelUrl;
	private String serialNumber;
	private String udn;
	private String upc;
	private String presentationUrl;

	private String ipAddress;
	private Integer port;

	private String fileAddress;

	// QTemporaryFile descriptionFile;

	public String getFileAddress() {
		return fileAddress;
	}

	public void setFileAddress(String fileAddress) {
		this.fileAddress = fileAddress;
	}

	private BrisaSSDPServer brisaSSDPServer;
	private BrisaWebServer webserver;

	// O XML
	private String location;
	private String serverName;
	private boolean running;

	// private ArrayList<BrisaIcon> iconList;
	private ArrayList<BrisaService> serviceList;
	private ArrayList<BrisaDevice> embeddedDeviceList;

	/**
	 * Construtor
	 * 
	 * @param deviceType
	 * @param friendlyName
	 * @param manufacturer
	 * @param manufacturerURL
	 * @param modelDescription
	 * @param modelName
	 * @param modelNumber
	 * @param modelURL
	 * @param serialNumber
	 * @param udn
	 * @param upc
	 * @param presentationURL
	 * @param defaultPort
	 */
	public BrisaDevice(String deviceType, String friendlyName,
			String manufacturer, String manufacturerURL,
			String modelDescription, String modelName, String modelNumber,
			String modelURL, String serialNumber, String udn, String upc,
			String presentationURL, Integer defaultPort) /* throws JDOMException */{

		// START ALL THE VARIABLES;
		this.running = false;
		this.serverName = "JBrisa Webserver UPnP/1.0 " + modelName + " "
				+ modelNumber;

		this.brisaSSDPServer = new BrisaSSDPServer();
		this.brisaSSDPServer.addObserver(this);

		this.deviceType = deviceType;

		this.friendlyName = friendlyName;
		this.fileAddress = this.friendlyName + ".xml";
		this.fileAddress = this.fileAddress.replaceAll(" ", "");

		this.manufacturer = manufacturer;
		this.manufacturerUrl = manufacturerURL;
		this.modelDescription = modelDescription;
		this.modelName = modelName;
		this.modelNumber = modelNumber;
		this.modelUrl = modelURL;
		this.serialNumber = serialNumber;
		this.udn = udn;
		this.upc = upc;
		this.presentationUrl = presentationURL;

		this.major = "1";
		this.minor = "0";

		this.port = defaultPort;

		serviceList = new ArrayList<BrisaService>();
		embeddedDeviceList = new ArrayList<BrisaDevice>();

		try {

			this.ipAddress = NetworkManager.getIPAddress();
			
			this.location = "http://" + this.ipAddress + ":" + this.port + "/"
					+ this.fileAddress;
			
			this.buildUrlBase();

		} catch (SocketException e) {
			BrisaLogger.logger.severe("Nao foi possivel obter um endereco IP");
			e.printStackTrace();
			System.exit(1);
		}
		
	}

	// Se o cara nao quiser dar a porta
	public BrisaDevice(String deviceType, String friendlyName,
			String manufacturer, String manufacturerURL,
			String modelDescription, String modelName, String modelNumber,
			String modelURL, String serialNumber, String udn, String upc,
			String presentationURL) {

		this(deviceType, friendlyName, manufacturer, manufacturerURL,
				modelDescription, modelName, modelNumber, modelURL,
				serialNumber, udn, upc, presentationURL, jbrisa.utils.NetworkManager
						.getPort());
	}

	/**
	 * Contruir URL
	 */
	private void buildUrlBase() {
		this.urlBase = "http://" + this.ipAddress + ":" + this.port;
	}

	/**
	 * Call this method to join the network and start the device.
	 */
	public void start() {

		if (isRunning()) {
			BrisaLogger.logger.info("BrisaDevice already running!");
			return;
		}

		BrisaLogger.logger.info("Starting Brisa Device");
		
		brisaSSDPServer.start();
		startWebServer();
		
		getLocationXML();
		
		doNotify();
		running = true;

		// ////////////////////////////////////////////////////////////////////
	}

	/**
	 * Stops the device and leaves the network.
	 */
	public void stop() {

		if (!isRunning()) {
			BrisaLogger.logger.info("BrisaDevice already stopped!");
			return;
		}

		BrisaLogger.logger.info("Stopping Brisa Device");
		this.doByeBye();
		brisaSSDPServer.stop();
		webserver.stop();
		running = false;
	}

	/**
	 * @return running
	 */
	public boolean isRunning() {
		return running;
	}

	/**
	 * Start Web Server
	 */
	private void startWebServer() {

		webserver = BrisaWebServerFactory.getBrisaWebServer();
		webserver.addNewObserver(this);
		webserver.setPort(this.port);
		webserver.setBrisaDevice(this);
		webserver.start();

		System.out.println("Brisa Device: Acquired Address " + this.ipAddress
				+ ":" + this.port);

	}

	/**
	 * Generates location's XML document
	 * @return 
	 */
	private Document generateXMLLocationDocument() {

		Document locationDocument = new Document();

		Namespace nms = Namespace
				.getNamespace("urn:schemas-upnp-org:device-1-0");
		
		Element root = new Element("root", nms);
		locationDocument.setRootElement(root);

		Element specVersionElement = new Element("specVersion", nms);
		Element majorElement = new Element("major", nms);
		majorElement.setText(this.major);
		Element minorElement = new Element("minor", nms);
		minorElement.setText(this.minor);
		specVersionElement.addContent(majorElement);
		specVersionElement.addContent(minorElement);

		Element urlBaseElement = new Element("URLBase", nms);
		urlBaseElement.setText(this.urlBase);

		Element deviceElement = new Element("device", nms);

		Element friendlyNameElement = new Element("friendlyName", nms);
		friendlyNameElement.setText(this.friendlyName);

		Element deviceTypeElement = new Element("deviceType", nms);
		deviceTypeElement.setText(this.deviceType);

		Element manufacturerElement = new Element("manufacturer", nms);
		manufacturerElement.setText(this.manufacturer);

		Element manufacturerURLElement = new Element("manufacturerURL", nms);
		manufacturerURLElement.setText(this.manufacturerUrl);

		Element modelDescriptionElement = new Element("modelDescription", nms);
		modelDescriptionElement.setText(this.modelDescription);

		Element modelNameElement = new Element("modelName", nms);
		modelNameElement.setText(this.modelName);

		Element modelNumberElement = new Element("modelNumber", nms);
		modelNumberElement.setText(this.modelNumber);

		Element serialNumberElement = new Element("serialNumber", nms);
		serialNumberElement.setText(this.serialNumber);

		Element udnElement = new Element("UDN", nms);
		udnElement.setText(this.udn);

		Element upcElement = new Element("UPC", nms);
		upcElement.setText(this.upc);

		// TODO IconList
		Element iconListElement = new Element("iconList", nms);

		Element serviceListElement = new Element("serviceList", nms);

		for (BrisaService service : this.serviceList) {

			Element serviceElement = new Element("service", nms);

			Element serviceTypeElement = new Element("serviceType", nms);
			serviceTypeElement.setText(service.getServiceType());

			Element serviceIdElement = new Element("serviceId", nms);
			serviceIdElement.setText(service.getServiceId());

			Element SCPDURLElement = new Element("SCPDURL", nms);
			SCPDURLElement.setText(service.getScpdURL());

			Element controlURLElement = new Element("controlURL", nms);
			controlURLElement.setText(service.getControlURL());

			Element eventSubURLElement = new Element("eventSubURL", nms);
			eventSubURLElement.setText(service.getEventSubURL());

			serviceElement.addContent(serviceTypeElement);
			serviceElement.addContent(serviceIdElement);
			serviceElement.addContent(SCPDURLElement);
			serviceElement.addContent(controlURLElement);
			serviceElement.addContent(eventSubURLElement);

			serviceListElement.addContent(serviceElement);
		}

		// TODO DeviceList
		Element deviceListElement = new Element("deviceList", nms);

		Element presentationURLElement = new Element("presentationURL", nms);
		presentationURLElement.setText(this.presentationUrl);

		root.addContent(specVersionElement);
		root.addContent(urlBaseElement);
		root.addContent(deviceElement);

		deviceElement.addContent(friendlyNameElement);
		deviceElement.addContent(deviceTypeElement);
		deviceElement.addContent(manufacturerElement);
		deviceElement.addContent(manufacturerURLElement);
		deviceElement.addContent(modelDescriptionElement);
		deviceElement.addContent(modelNameElement);
		deviceElement.addContent(modelNumberElement);
		deviceElement.addContent(serialNumberElement);
		deviceElement.addContent(udnElement);
		deviceElement.addContent(upcElement);

		deviceElement.addContent(iconListElement);
		deviceElement.addContent(serviceListElement);
		deviceElement.addContent(deviceListElement);
		deviceElement.addContent(presentationURLElement);

		return locationDocument;

	}
	
	/**
	 * Retorna o conteúdo do XML do location
	 * @return
	 */
	public String getLocationXML() {
		return XmlManager.generateXMLString(generateXMLLocationDocument());
	}

	/**
	 * Sends the ssdp:alive messages for root device, embedded devices and
	 * services according to the UPnP 1.0 especification.
	 */
	public void doNotify() {

		String serviceType;
		String serviceUdn;
		String embeddedUdn;
		String embeddedType;

		// Notify for root device
		brisaSSDPServer.doNotify(udn + "::upnp:rootdevice", location, "upnp:rootdevice",
				serverName, "1800");
		brisaSSDPServer.doNotify(udn, location, udn, serverName, "1800");
		brisaSSDPServer.doNotify(udn + "::" + deviceType, location, deviceType,
				serverName, "1800");

		// Notify for embedded devices
		for (BrisaDevice embeddedDevice : embeddedDeviceList) {
			embeddedUdn = embeddedDevice.getUdn();
			embeddedType = embeddedDevice.getDeviceType();
			brisaSSDPServer.doNotify(embeddedUdn, location, embeddedUdn, serverName,
					"1800");
			brisaSSDPServer.doNotify(embeddedUdn + "::" + embeddedType, location,
					embeddedType, serverName, "1800");
		}

		// Notify for services
		for (BrisaService service : serviceList) {
			serviceType = service.getServiceType();
			serviceUdn = udn + "::" + serviceType;
			brisaSSDPServer.doNotify(serviceUdn, location, serviceType, serverName, "1800");
		}
	}

	/**
	 * Sends the ssdp:byebye messages for root device, embedded devices and
	 * services according to the UPnP 1.0 especification.
	 */
	public void doByeBye() {

		String serviceType;
		String serviceUdn;
		String embeddedUdn;
		String embeddedType;

		// Notify for root device
		brisaSSDPServer.doByeBye(udn + "::upnp:rootdevice", "upnp:rootdevice");
		brisaSSDPServer.doByeBye(udn, udn);
		brisaSSDPServer.doByeBye(udn + "::" + deviceType, deviceType);

		// Bye Bye for embedded devices
		for (BrisaDevice embeddedDevice : embeddedDeviceList) {
			embeddedUdn = embeddedDevice.getUdn();
			embeddedType = embeddedDevice.getDeviceType();
			brisaSSDPServer.doByeBye(embeddedUdn, embeddedUdn);
			brisaSSDPServer.doByeBye(embeddedUdn + "::" + embeddedType, embeddedType);
		}

		// ByeBye for services
		for (BrisaService service : serviceList) {
			serviceType = service.getServiceType();
			serviceUdn = udn + "::" + serviceType;
			brisaSSDPServer.doByeBye(serviceUdn, serviceType);
		}
	}

	@Override
	public void update(Observable o, Object arg) {

		//if BrisaDevice recieved a msearch message, respond msearch;
		if (o instanceof BrisaSSDPServer && arg instanceof DatagramPacket) {

			DatagramPacket msg = (DatagramPacket) arg;
			String messsageSSDPReceived = new String(msg.getData());

			if (messsageSSDPReceived.contains(_Constants.SSDP_MSEARCH)) {

				BrisaLogger.logger.info("BrisaSSDPServer Received msearch from "
						+ msg.getAddress().getHostAddress());

				String st = getST(messsageSSDPReceived);

				respondMSearch(st, msg.getAddress().getHostAddress(),msg.getPort());
			}
		}
	}

	/**
	 * 
	 * @param msg
	 */
	private String getST(String msg) {

		String st = new String();

		StringTokenizer tokenizer = new StringTokenizer(msg, "\n\r");

		while (tokenizer.hasMoreTokens()) {
			String token = tokenizer.nextToken();
			if (token.toLowerCase().startsWith("st")) {
				st = token.substring(3).replaceAll(" ", "");
			}
		}

		return st;

	}

	/**
	 * 
	 * @param st
	 * @param senderIp
	 * @param senderPort
	 */
	private void respondMSearch(String st, String senderIp, int senderPort) {

		String formato = "dd/MM/yyyy";
		SimpleDateFormat format = new SimpleDateFormat(formato);
		String sDate = format.format(new Date());

		// TODO Debugar!
		if (st.equals("ssdp:all")) {
			respondMSearchAll(senderIp, senderPort, sDate);
		} else if (st.equals("upnp:rootdevice")) {

			brisaSSDPServer.respondMSearch(senderIp, senderPort, "1800", sDate, location,
					serverName, "upnp:rootdevice", this.udn
							+ "::upnp:rootdevice");

		} else if (st.contains("uuid:")) {

			String embeddedUdn;
			@SuppressWarnings("unused")
			String embeddedType;

			if (st.equals(udn)) {

				brisaSSDPServer.respondMSearch(senderIp, senderPort, "1800", sDate,
						location, serverName, "upnp:rootdevice", udn
								+ "::upnp:rootdevice");
			}

			// Embbebed Devices respondMSearch
			for (BrisaDevice embeddedDevice : embeddedDeviceList) {
				embeddedUdn = embeddedDevice.getUdn();
				embeddedType = embeddedDevice.getDeviceType();
				if (st.equals(embeddedUdn))
					brisaSSDPServer.respondMSearch(senderIp, senderPort, "1800", sDate,
							location, serverName, embeddedUdn, embeddedUdn);
			}
		} else if (st.contains(":device:")) {

			String embeddedUdn;
			String embeddedType;

			if (st.equals(deviceType)) {

				brisaSSDPServer.respondMSearch(senderIp, senderPort, "1800", sDate,
						location, serverName, "upnp:rootdevice", udn
								+ "::upnp:rootdevice");
			}

			for (BrisaDevice embeddedDevice : embeddedDeviceList) {
				embeddedUdn = embeddedDevice.getUdn();
				embeddedType = embeddedDevice.getDeviceType();
				if (st.equals(embeddedType))
					brisaSSDPServer.respondMSearch(senderIp, senderPort, "1800", sDate,
							location, serverName, embeddedUdn, embeddedUdn);
			}
		} else if (st.contains(":service:")) {

			String serviceType;
			String serviceUdn;

			for (BrisaService service : serviceList) {
				serviceType = service.getServiceType();
				serviceUdn = udn + "::" + serviceType;

				if (st.equals(serviceType))
					brisaSSDPServer.respondMSearch(senderIp, senderPort, "1800", sDate,
							location, serverName, serviceUdn, serviceType);
			}
		}

	}

	private void respondMSearchAll(String senderIp, int senderPort, String sDate) {
		String serviceType;
		String serviceUdn;
		String embeddedUdn;
		String embeddedType;

		// MSearch response for root device
		brisaSSDPServer.respondMSearch(senderIp, senderPort, "1800", sDate, location,
				serverName, "upnp:rootdevice", udn + "::upnp:rootdevice");

		brisaSSDPServer.respondMSearch(senderIp, senderPort, "1800", sDate, location,
				serverName, udn, udn);

		brisaSSDPServer.respondMSearch(senderIp, senderPort, "1800", sDate, location,
				serverName, udn + "::" + deviceType, deviceType);

		// MSearch response for embedded devices
		for (BrisaDevice embeddedDevice : embeddedDeviceList) {
			embeddedUdn = embeddedDevice.getUdn();
			embeddedType = embeddedDevice.getDeviceType();
			brisaSSDPServer.respondMSearch(senderIp, senderPort, "1800", sDate, location,
					serverName, embeddedUdn, embeddedUdn);
			brisaSSDPServer.respondMSearch(senderIp, senderPort, "1800", sDate, location,
					serverName, embeddedUdn + "::" + embeddedType, embeddedType);
		}

		// MSearch response for services
		for (BrisaService service : serviceList) {
			serviceType = service.getServiceType();
			serviceUdn = udn + "::" + serviceType;
			brisaSSDPServer.respondMSearch(senderIp, senderPort, "1800", sDate, location,
					serverName, serviceUdn, serviceType);
		}
	}

	/**
	 * 
	 * @param serviceType
	 * @param serviceId
	 * @param SCPDURL
	 * @param controlURL
	 * @param eventSubURL
	 */
	public void addService(String serviceType, String serviceId,
			String SCPDURL, String controlURL, String eventSubURL) {

		BrisaService serviceSwap = new BrisaService(serviceId, serviceType);
		this.addService(serviceSwap);
	}

	public void addService(BrisaService servdev) {
		serviceList.add(servdev);
		servdev.setUdn(this.udn);
	}

	public void addEmbeddedDevice(String deviceType, String friendlyName,
			String manufacturer, String manufacturerURL,
			String modelDescription, String modelName, String modelNumber,
			String modelURL, String serialNumber, String UDN, String UPC,
			String presentationURL) {

		BrisaDevice deviceSwap = new BrisaDevice(deviceType, friendlyName,
				manufacturer, manufacturerURL, modelDescription, modelName,
				modelNumber, modelURL, serialNumber, UDN, UPC, presentationURL,
				0);
		embeddedDeviceList.add(deviceSwap);
	}

	public void addEmbeddedDevice(BrisaDevice newEmbeddedDevice) {
		embeddedDeviceList.add(newEmbeddedDevice);
	}

	public BrisaService getServiceById(String serviceId) {
		for (BrisaService s : this.serviceList) {
			if (s.getServiceId().equals(serviceId))
				return s;
		}
		return null;
	}
	
	public BrisaService getServiceByEventSubURL(String eventSubURL) {
		for (BrisaService s : this.serviceList) {
			if (s.getEventSubURL().contains(eventSubURL) || eventSubURL.contains(s.getEventSubURL()))
				return s;
		}
		return null;
	}

	public BrisaService getServiceByType(String serviceType) {
		for (BrisaService s : this.serviceList) {
			if (s.getServiceType().equals(serviceType)) {
				return s;
			}
		}
		return null;
	}
	
	public BrisaService getServiceByScdp(String serviceScdp) {
		for (BrisaService s : this.serviceList) {
			
			String scdp = s.getScpdURL();
			
			if (serviceScdp.equals(scdp)) {
				return s;
			}
		}
		return null;
	}

	// ============== GETTERS AND SETTERS =============

	public String getMajor() {
		return major;
	}

	public void setMajor(String major) {
		this.major = major;
	}

	public String getMinor() {
		return minor;
	}

	public void setMinor(String minor) {
		this.minor = minor;
	}

	public String getUrlBase() {
		return urlBase;
	}

	public void setUrlBase(String urlBase) {
		this.urlBase = urlBase;
	}

	public String getDeviceType() {
		return deviceType;
	}

	public void setDeviceType(String deviceType) {
		this.deviceType = deviceType;
	}

	public String getFriendlyName() {
		return friendlyName;
	}

	public void setFriendlyName(String friendlyName) {
		this.friendlyName = friendlyName;
	}

	public String getManufacturer() {
		return manufacturer;
	}

	public void setManufacturer(String manufacturer) {
		this.manufacturer = manufacturer;
	}

	public String getManufacturerUrl() {
		return manufacturerUrl;
	}

	public void setManufacturerUrl(String manufacturerUrl) {
		this.manufacturerUrl = manufacturerUrl;
	}

	public String getModelDescription() {
		return modelDescription;
	}

	public void setModelDescription(String modelDescription) {
		this.modelDescription = modelDescription;
	}

	public String getModelName() {
		return modelName;
	}

	public void setModelName(String modelName) {
		this.modelName = modelName;
	}

	public String getModelNumber() {
		return modelNumber;
	}

	public void setModelNumber(String modelNumber) {
		this.modelNumber = modelNumber;
	}

	public String getModelUrl() {
		return modelUrl;
	}

	public void setModelUrl(String modelUrl) {
		this.modelUrl = modelUrl;
	}

	public String getSerialNumber() {
		return serialNumber;
	}

	public void setSerialNumber(String serialNumber) {
		this.serialNumber = serialNumber;
	}

	public String getUdn() {
		return udn;
	}

	public void setUdn(String udn) {
		this.udn = udn;
	}

	public String getUpc() {
		return upc;
	}

	public void setUpc(String upc) {
		this.upc = upc;
	}

	public String getPresentationUrl() {
		return presentationUrl;
	}

	public void setPresentationUrl(String presentationUrl) {
		this.presentationUrl = presentationUrl;
	}

	public String getServerName() {
		return serverName;
	}

	public void setServerName(String serverName) {
		this.serverName = serverName;
	}

	public String getIpAddress() {
		return ipAddress;
	}

	public Integer getPort() {
		return port;
	}

}
