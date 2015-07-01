package jbrisa.utils;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.Reader;
import java.io.StringReader;
import java.io.StringWriter;
import java.net.URL;
import java.util.ArrayList;
import java.util.List;

import jbrisa.upnp.controlpoint.model.BrisaCPDevice;
import jbrisa.upnp.controlpoint.model.BrisaCPIcon;
import jbrisa.upnp.controlpoint.model.BrisaCPService;

import org.jdom.Document;
import org.jdom.Element;
import org.jdom.Namespace;
import org.jdom.input.SAXBuilder;
import org.jdom.output.Format;
import org.jdom.output.XMLOutputter;


/**
 * 
 * @author Thiago Ribeiro
 * 
 */
public class XmlManager {

	// =========================================================================
	// CONSTRUCTORS ============================================================
	// =========================================================================

	// =========================================================================
	// PUBLIC METHODS===========================================================
	// =========================================================================

	/**
	 * Gererates XML file from XML Document
	 * @param doc
	 * @param fileAddress
	 */
	public static void generateXMLFile(Document doc, String fileAddress) {
		
		File file = new File(fileAddress);

		try {
			if (file.exists()) {
				file.delete();
			}
			if (file.createNewFile()) {
				XMLOutputter xmlout = new XMLOutputter(Format.getPrettyFormat());
				FileWriter f = new FileWriter(file);
				xmlout.output(doc, f);
				f.close();
			} else {
				System.err
				.println("Não foi possível criar o arquivo de location");
			}
		} catch (IOException e) {
			System.err
			.println("Não foi possível criar o arquivo de location");
			e.printStackTrace();
		}
	}
	
	/**
	 * Generate XML String from Document
	 * @param doc
	 * @return
	 */
	public static String generateXMLString(Document doc) {
		
		XMLOutputter xmlout = new XMLOutputter(Format.getPrettyFormat());
		StringWriter sw = new StringWriter();
		
		try {
			xmlout.output(doc, sw);
		} catch (IOException e) {
			e.printStackTrace();
		}
		return sw.toString();
	}
	
	
	public static Element parseStringToXmlElement(String xmlString) {
		try {

			SAXBuilder builder = new SAXBuilder();
			Reader in = new StringReader(xmlString);
			Document doc;
			doc = builder.build(in);
			Element root = doc.getRootElement();

			return root;

		} catch (Exception e) {
			return null;
		}
	}

	public static Element getDeviceXmlElement(String location) {

		SAXBuilder builder = new SAXBuilder();

		try {
			Document documento = builder.build(new URL(location));
			Element root = documento.getRootElement();
			Element deviceXml = root.getChild("device", root.getNamespace());

			BrisaLogger.logger
					.info("Xml from device location loaded sucessfully");

			return deviceXml;
		} catch (Exception e) {

			BrisaLogger.logger
					.warning("Xml from device location wasn`t loaded sucessfully. Device won`t be created. "
							+ e.getMessage());
			return null;
		}
	}

	public static Element getScdpXml(String location) {

		SAXBuilder builder = new SAXBuilder();

		try {

			Document documento = builder.build(new URL(location));
			Element root = documento.getRootElement();

			BrisaLogger.logger.info("Xml service from " + location
					+ " loaded sucessfully");

			return root;
		} catch (Exception e) {

			BrisaLogger.logger
					.warning("Xml from device location wasn`t loaded sucessfully. Service won`t be created. "
							+ e.getMessage());
		}

		return null;
	}

	public static String getXmlNodeValue(Element root, String node) {
		try {
			String value = root.getChild(node, root.getNamespace()).getValue();
			BrisaLogger.logger.info(node + " value set to: " + value);
			return value;

		} catch (Exception e) {
			BrisaLogger.logger.warning(node + " unnavaliable and set to empty. "
					+ e.getMessage());
			return "";
		}
	}

	public static ArrayList<BrisaCPService> getServiceList(Element root,
			String location) {

		try {
			ArrayList<BrisaCPService> arrayServices = new ArrayList<BrisaCPService>();

			List services = root.getChild("serviceList", root.getNamespace())
					.getChildren();
			BrisaLogger.logger.info("Found " + services.size()
					+ " services for this device");

			for (Object service : services) {

				BrisaCPService newService = new BrisaCPService();
				newService.controlURL = checkPathUrl(((Element) service)
						.getChild("controlURL", root.getNamespace()).getValue());
				newService.eventSubURL = checkPathUrl(((Element) service)
						.getChild("eventSubURL", root.getNamespace())
						.getValue());
				newService.SCPDURL = checkCompleteUrl(((Element) service)
						.getChild("SCPDURL", root.getNamespace()).getValue(),
						location);
				newService.serviceId = ((Element) service).getChild(
						"serviceId", root.getNamespace()).getValue();
				newService.serviceType = ((Element) service).getChild(
						"serviceType", root.getNamespace()).getValue();

				arrayServices.add(newService);

				BrisaLogger.logger.info("Service of id " + newService.serviceId
						+ " added sucessfully");
			}

			if (arrayServices.size() > 0) {
				return arrayServices;
			}

			// if no service were created, return null;
			else {
				BrisaLogger.logger
						.warning("No service were found for this device.");
				return null;
			}

		} catch (Exception e) {

			BrisaLogger.logger
					.warning("Problem while creating one of the services. "
							+ e.getMessage());
			return null;
		}
	}

	private static String checkPathUrl(String value) {

		String url = "";

		// If is a complete url, get just the path;
		if (value.contains("http://")) {
			try {
				url = "/" + value.split("/", 3)[0];

			} catch (Exception e) {
				return value;
			}
		}
		// If already is the path, check if it start's with '/';
		else {
			if (value.startsWith("/")) {
				url = value;
			} else {
				url = "/" + value;
			}
		}

		return url;
	}

	private static String checkCompleteUrl(String value, String location) {

		String url = "";

		if (value.contains("http://")) {
			url = value;
		} else {
			if (value.startsWith("/")) {
				url = XmlManager.getDeviceIP(location) + value;
			} else {
				url = XmlManager.getDeviceIP(location) + "/"+ value;
			}
		}

		return url;
	}

	public static ArrayList<BrisaCPIcon> getIconList(Element deviceXml) {

		try {
			ArrayList<BrisaCPIcon> arrayIcons = new ArrayList<BrisaCPIcon>();

			List iconXmlList = deviceXml.getChild("iconList",
					deviceXml.getNamespace()).getChildren();
			for (Object iconAux : iconXmlList) {

				BrisaCPIcon newIcon = new BrisaCPIcon();
				Element iconXmlItem = (Element) iconAux;

				newIcon.depth = getXmlNodeValue(iconXmlItem, "depth");
				newIcon.height = getXmlNodeValue(iconXmlItem, "height");
				newIcon.mimeType = getXmlNodeValue(iconXmlItem, "mimeType");
				newIcon.url = getXmlNodeValue(iconXmlItem, "url");
				newIcon.width = getXmlNodeValue(iconXmlItem, "width");

				arrayIcons.add(newIcon);
			}

			if (arrayIcons.size() > 0) {
				return arrayIcons;
			} else {
				BrisaLogger.logger.warning("No icon were found for this device.");
				return null;
			}

		} catch (Exception e) {
			BrisaLogger.logger
					.warning("Problem while creating one of the icons or there's no iconList for this device. "
							+ e.getMessage());
			return null;
		}
	}

	public static ArrayList<BrisaCPDevice> getDeviceList(Element deviceXml,
			String location) {

		try {
			ArrayList<BrisaCPDevice> arrayDevices = new ArrayList<BrisaCPDevice>();

			List devices = deviceXml.getChild("deviceList",
					deviceXml.getNamespace()).getChildren();
			BrisaLogger.logger.info("Found " + devices.size()
					+ " devices nested for this device");

			for (Object aux : devices) {

				BrisaCPDevice currentDevice = new BrisaCPDevice();
				Element deviceXmlItem = (Element) aux;

				currentDevice.deviceType = getXmlNodeValue(deviceXmlItem,
						"deviceType");

				currentDevice.friendlyName = getXmlNodeValue(deviceXmlItem,
						"friendlyName");

				currentDevice.manufacturer = getXmlNodeValue(deviceXmlItem,
						"manufacturer");

				currentDevice.manufacturerURL = getXmlNodeValue(deviceXmlItem,
						"manufacturerURL");

				currentDevice.modelDescription = getXmlNodeValue(deviceXmlItem,
						"modelDescription");

				currentDevice.modelName = getXmlNodeValue(deviceXmlItem,
						"modelName");

				currentDevice.modelNumber = getXmlNodeValue(deviceXmlItem,
						"modelNumber");

				currentDevice.modelURL = getXmlNodeValue(deviceXmlItem,
						"modelURL");

				currentDevice.serialNumber = getXmlNodeValue(deviceXmlItem,
						"serialNumber");

				currentDevice.UDN = getXmlNodeValue(deviceXmlItem, "UDN");

				currentDevice.UPC = getXmlNodeValue(deviceXmlItem, "UPC");

				currentDevice.presentationURL = getXmlNodeValue(deviceXmlItem,
						"presentationURL");

				currentDevice.iconList = getIconList(deviceXmlItem);

				currentDevice.serviceList = getServiceList(deviceXmlItem,
						location);

				currentDevice.deviceList = getDeviceList(deviceXmlItem,
						location);

				arrayDevices.add(currentDevice);
			}

			if (arrayDevices.size() > 0) {
				return arrayDevices;
			} else {
				BrisaLogger.logger
						.warning("No nested device were found for this device.");
				return null;
			}

		} catch (Exception e) {

			BrisaLogger.logger
					.warning("Problem while creating one of the nested devices or there's no deviceList for this device. "
							+ e.getMessage());
			return null;
		}
	}

	public static String getUrlBaseFromLocation(String location) {

		String urlBase = "";
		try {
			if (location.contains("http://")) {
				String[] aux = location.split("/");
				urlBase = "http://" + aux[2];
			} else {
				urlBase = location.split("/")[0];
			}
		} catch (Exception e) {
			// return "";
		}
		return urlBase;
	}

	public static String getLocationFromSSDP(String ssdp) {

		BrisaLogger.logger.info("Device answer to be analysed: \n" + ssdp);

		String[] arrayAux = ssdp.split("\r\n");

		for (String string : arrayAux) {

			String atribute = string.split(":")[0];

			if (atribute.toLowerCase().equals("location")
					|| atribute.toLowerCase().equals("location ")) {

				return getLocation(string);
			}
		}

		return null;
	}

	public static Integer getMaxAgeFromSSDP(String ssdp) {
		String[] arrayAux = ssdp.split("\r\n");

		for (String string : arrayAux) {

			String atribute = string.split(":")[0];

			if (atribute.toLowerCase().equals("cache-control")
					|| atribute.toLowerCase().equals("cache-control ")) {

				return getMaxAge(string);
			}
		}

		return _Constants.DEFAULT_MAX_AGE;
	}

	public static String getUUIDFromSSDP(String ssdp) {
		String[] arrayAux = ssdp.split("\r\n");

		for (String string : arrayAux) {

			String atribute = string.split(":")[0];

			if (atribute.toLowerCase().equals("usn")
					|| atribute.toLowerCase().equals("usn ")) {

				return getUUID(string);
			}
		}

		return null;
	}

	public static String getDeviceIP(String location) {

		String deviceIp = "";

		for (String aux : location.split("/")) {

			if (aux.contains(".") && aux.contains(":")) {

				deviceIp = aux;
				return "http://" + deviceIp;
			}
		}
		return null;

	}

	// =========================================================================
	// PRIVATE METHODS==========================================================
	// =========================================================================

	private static String getLocation(String string) {

		try {

			String location = string.split(":", 2)[1].replace(" ", "");
			BrisaLogger.logger.info("Location found and set to " + location);
			return location;

		} catch (Exception e) {

			BrisaLogger.logger
					.severe("Location coundn`t be found and the device is unavaliable. "
							+ e.getMessage());
		}

		return null;
	}

	private static Integer getMaxAge(String string) {

		try {

			int maxAge = Integer.parseInt(string.split("=")[1].replaceAll(" ",
					""));
			BrisaLogger.logger.info("Max-age found and set to " + maxAge);

			return maxAge;

		} catch (Exception e) {

			BrisaLogger.logger
					.warning("Max-age couldn`t be found, default value was returned. "
							+ e.getMessage());
		}

		return _Constants.DEFAULT_MAX_AGE;
	}

	private static String getUUID(String string) {

		try {

			String location = "";
			String[] aux = string.split(":");
			Boolean found = false;

			for (String aux2 : aux) {

				if (aux2.length() == 36) {

					location = aux2;
					found = true;
					BrisaLogger.logger.info("UUID found and set to " + location);
					return location;
				}
			}

			if (!found) {
				BrisaLogger.logger.info("UUID not found");
				return null;
			}

		} catch (Exception e) {

			BrisaLogger.logger
					.severe("UUID coundn`t be found. " + e.getMessage());
		}

		return null;
	}

	public static Element getElementFromRoot(Element root, String string,
			Namespace namespace) {

		try {
			return root.getChild(string, root.getNamespace());

		} catch (Exception e) {
			return null;
		}
	}

}
