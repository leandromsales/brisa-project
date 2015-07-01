package jbrisa.utils;

public class _Constants {
	
	public static final String JBRISA_CP_INFO = "JBrisa Control Point/1.0";
	
	public static final String SSDP_MSEARCH = "M-SEARCH";
	
	public static final int DEFAULT_MSEARCH_PORT = 1900;
	
	public static final String DEFAULT_MSEARCH_IP = "239.255.255.250";

	public static final int DEFAULT_DATAGRAM_SIZE = 2048;
	
	public static final int DEFAULT_MAX_AGE = 1800;
	
	public static final String SSDP_NTS_ALIVE = "ssdp:alive";
	
	public static final String SSDP_NTS_BYE = "ssdp:byebye";
	
	public static final String SSDP_NTS_UPDATE = "ssdp:update";
	
	public static final String UPNP_MSEARCH_DISCOVER_BASE = 
		  "M-SEARCH * HTTP/1.1\r\n"
		+ "HOST: 239.255.255.250:1900\r\n"
		+ "MAN: \"ssdp:discover\"\r\n";
	
	
	// TODO: Add this three fields commented below in the ALIVE_MESSAGE 
	// as per upnp spec 1.1, section 1.2.2.
	//  - BOOTID.UPNP.ORG
	//  - CONFIGID.UPNP.ORG
	//  - SEARCHPORT.UPNP.ORG (optional)
	public static final String UPNP_ALIVE_MESSAGE = "NOTIFY * HTTP/1.1\r\n"
	                                          + "HOST: 239.255.255.250:1900\r\n"
	                                          + "CACHE-CONTROL: max-age=%1\r\n"
	                                          + "LOCATION: %2\r\n"
	                                          + "NT: %3\r\n"
	                                          + "NTS: ssdp:alive\r\n"
	                                          + "SERVER: %4\r\n"
	                                          + "USN: %5\r\n"
	                                          + "\r\n";

	// TODO: Implement ssdp:update as per spec 1.1, section 1.2.4
	// and use the below define to build the message, where
	// SEARCHPORT.UPNP.ORG are optional.
	public static final String UPNP_UPDATE_MESSAGE = "NOTIFY * HTTP/1.1\r\n"
	                                           + "HOST: 239.255.255.250:1900\r\n"
	                                           + "LOCATION: %1\r\n"
	                                           + "NT: %2\r\n"
	                                           + "NTS: ssdp:update\r\n"
	                                           + "USN: %3\r\n"
	                                           + "CONFIGID.UPNP.ORG: %4\r\n"
	                                           + "NEXTBOOTID.UPNP.ORG: %5\r\n"
	                                           + "SEARCHPORT.UPNP.ORG: %6\r\n"
	                                           + "\r\n";

	// TODO: Add this two fields commented below in the BYEBYE MESSAGE
	// as per upnp spec 1.1, section 1.2.2 and 1.2.3.
	//  - BOOTID.UPNP.ORG
	//  - CONFIGID.UPNP.ORG
	public static final String UPNP_BYEBYE_MESSAGE = "NOTIFY * HTTP/1.1\r\n"
	                                           + "HOST: 239.255.255.250:1900\r\n"
	                                           + "NT: %1\r\n"
	                                           + "NTS: ssdp:byebye\r\n"
	                                           + "USN: %2\r\n"
	                                           + "\r\n";

	// TODO: Add this three fields commented below in the MSEARCH_RESPONSE
	// as per upnp spec 1.1, section 1.3.3.
	//  - BOOTID.UPNP.ORG
	//  - CONFIGID.UPNP.ORG
	//  - SEARCHPORT.UPNP.ORG (optional)
	public static final String UPNP_MSEARCH_RESPONSE = "HTTP/1.1 200 OK\r\n"
	                                             + "CACHE-CONTROL: max-age = %1\r\n"
	                                             + "DATE: %2\r\n"
	                                             + "EXT: \r\n"
	                                             + "LOCATION: %3\r\n"
	                                             + "SERVER: %4\r\n"
	                                             + "ST: %5\r\n"
	                                             + "USN: %6\r\n"
	                                             + "\r\n";
	
	public static final String UPNP_MULTICAST_EVENT_RESPONSE = 
			"NOTIFY * HTTP/1.0\r\n"
            + "HOST: 239.255.255.246:7900\r\n"
            + "CONTENT-TYPE: text/xml; charset=\"utf-8\"\r\n"
            + "USN: %1\r\n"
            + "SVCID: %2\r\n"
            + "NT: upnp:event\r\n"
            + "NTS: upnp:propchange\r\n"
            + "SEQ: %3\r\n"
            + "LVL: %4\r\n"
            + "BOOTID.UPNP.ORG: %5\r\n"
            + "CONTENT-LENGHT: %6\r\n"
            + "\r\n";
	
	@Deprecated
	public static final String LOCATION_BEGIN_STRING = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n"
			+ "<root xmlns=\"urn:schemas-upnp-org:device-1-0\">\r\n"
			+ "  <specVersion>\r\n"
			+ "    <major>1</major>\r\n"
			+ "    <minor>0</minor>\r\n"
			+ "  </specVersion>\r\n"
			+ "  <device>\r\n"
			+ "    <deviceType></deviceType>\r\n"
			+ "    <friendlyName></friendlyName>\r\n"
			+ "    <manufacturer></manufacturer>\r\n"
			+ "    <manufacturerURL></manufacturerURL>\r\n"
			+ "    <modelDescription></modelDescription>\r\n"
			+ "    <modelName></modelName>\r\n"
			+ "    <modelNumber></modelNumber>\r\n"
			+ "    <modelURL></modelURL>\r\n"
			+ "    <serialNumber></serialNumber>\r\n"
			+ "    <UDN></UDN>\r\n"
			+ "    <serviceList>\r\n";
	
	public static final String LOCATION_END_STRING = "    </serviceList>\r\n"
			+ "  </device>\r\n" + "</root>\r\n";
	
	public static final String LOCATION_SERVICE_STRING = "      <service>\r\n"
		+ "        <serviceType></serviceType>\r\n"
		+ "        <serviceId></serviceId>\r\n"
		+ "        <controlURL></controlURL>\r\n"
		+ "        <eventSubURL></eventSubURL>\r\n"
		+ "        <SCPDURL></SCPDURL>\r\n"
		+ "      </service>\r\n";

	public static final String DEFAULT_MULTICAST_EVENT_IP = "239.255.255.246";
	
	public static final Integer DEFAULT_MULTICAST_EVENT_PORT = 7900;
	
	public static final String UPNP_SUBSCRIPTION_MESSAGE = "SUBSCRIBE %1 HTTP/1.1\r\n"
			+ "HOST: %2:%3\r\n"
			+ "USER-AGENT: %4\r\n"
			+ "CALLBACK: %5\r\n"
			+ "NT: upnp:event\r\n"
			+ "TIMEOUT: %6\r\n" + "\r\n";
	
	public static final String UPNP_RENEW_SUBSCRIPTION_MESSAGE = "SUBSCRIBE %1 HTTP/1.1\r\n"
		+ "HOST: %2:%3\r\n"
		+ "SID: %4\r\n"
		+ "TIMEOUT: %5\r\n" + "\r\n";
	
	public static final String UPNP_UNSUBSCRIPTION_MESSAGE = "UNSUBSCRIBE %1 HTTP/1.1\r\n"
		+ "HOST: %2:%3\r\n"
		+ "SID: %4\r\n"
		+ "\r\n";

}
