package jbrisa.webserver;

import jbrisa.webserver.jettywebserver.JettyWebServer;

public class BrisaWebServerFactory {
	
	public static BrisaWebServer getBrisaWebServer() {
		
		BrisaWebServer webserver;
		
		webserver = new JettyWebServer();
		
		return webserver;
		
	}
	
	
}
