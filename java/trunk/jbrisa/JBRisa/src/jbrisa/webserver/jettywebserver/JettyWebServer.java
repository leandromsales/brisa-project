package jbrisa.webserver.jettywebserver;

import java.io.BufferedReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.Observable;
import java.util.Observer;

import javax.servlet.ServletException;
import javax.servlet.http.HttpServlet;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import jbrisa.upnp.device.core.BrisaDevice;
import jbrisa.upnp.device.core.BrisaService;
import jbrisa.upnp.device.event.BrisaEventSubscription;
import jbrisa.upnp.device.model.BrisaAction;
import jbrisa.upnp.device.model.BrisaArgument;
import jbrisa.utils.BrisaLogger;
import jbrisa.utils.XmlManager;
import jbrisa.webserver.BrisaWebServer;

import org.eclipse.jetty.server.Handler;
import org.eclipse.jetty.server.Server;
import org.eclipse.jetty.server.handler.HandlerList;
import org.eclipse.jetty.server.handler.ResourceHandler;
import org.eclipse.jetty.server.nio.SelectChannelConnector;
import org.eclipse.jetty.servlet.ServletContextHandler;
import org.eclipse.jetty.servlet.ServletHolder;
import org.jdom.Element;
import org.jdom.output.Format;
import org.jdom.output.XMLOutputter;


public class JettyWebServer extends Observable implements BrisaWebServer {

	private int port; 
	private BrisaDevice brisaDevice;
	private Server server;

	
	public JettyWebServer() {
		this.server = new Server();
	}

	@Override
	public void addNewObserver(Observer o) {
		addObserver(o);
	}
	
	@Override
	public void setPort(int port) {
		this.port = port;
	}

	@Override
	public int getPort() {
		return this.port;
	}
	
	@Override
	public void setBrisaDevice(BrisaDevice brisaDevice) {
		this.brisaDevice = brisaDevice;
	}

	/**
	 * Start JBRisa Web Server
	 */
	@Override
	public void start() {

		if (!isRunning()) {
			
			// Server server = new Server(); No construtor.
			SelectChannelConnector connector = new SelectChannelConnector();
			connector.setPort(port);
			server.addConnector(connector);

			// SERVLET PARA RECEBER GET - POST - SUBSCRIBE;
			ServletContextHandler context = new ServletContextHandler(
					ServletContextHandler.SESSIONS);
			context.setContextPath("/");
			context.addServlet(new ServletHolder(new HelloServlet()), "/*");

			// HANDLER DE ARQUIVOS - LOCATION E SCDP XML'S;
			
			ResourceHandler resource_handler = new ResourceHandler();
			resource_handler.setDirectoriesListed(true);
			resource_handler.setWelcomeFiles(new String[] { "index.html" });
			resource_handler.setResourceBase("./");
			
			// ADICIONANDO OS HANDLES DE FILE E DE POST;
			HandlerList handlers = new HandlerList();
			handlers.setHandlers(new Handler[] { 
					resource_handler ,context
			});
			server.setHandler(handlers);

			try {
				server.start();
				// server.join(); Join junta as threads.
			} catch (InterruptedException e) {
				e.printStackTrace();
			} catch (Exception e) {
				e.printStackTrace();
			}

		} else {

			BrisaLogger.logger.warning("JBrisa Web Server: Already running!");
		}
	}

	/**
	 * Stop JBrisa Web Server
	 */
	@Override
	public void stop() {
		if (isRunning()) {
			try {
				server.join();
				server.stop();
			} catch (InterruptedException e) {
				e.printStackTrace();
			} catch (Exception e) {
				e.printStackTrace();
			}
		} else {
			BrisaLogger.logger.warning("JBrisa Web Server: Already stopped!");
		}
	}

	/**
	 * 
	 * @return is Running
	 */
	@Override
	public boolean isRunning() {
		return server.isRunning();
	}
	
	/**
	 * @author thiago
	 * 
	 *         Servlet used to parse the HTTP messages received by the device server;
	 * 
	 */
	@SuppressWarnings("serial")
	class HelloServlet extends HttpServlet {

		public HelloServlet() {
		}
		
		@Override
		protected void service(HttpServletRequest request, HttpServletResponse response)
				throws ServletException, IOException {
			
			if(request.getMethod().equals("POST"))
			{
				doPost(request, response);
			}
			else if(request.getMethod().equals("GET"))
			{
				doGet(request, response);
			}
			else if(request.getMethod().equals("SUBSCRIBE")){
				
				doSubscribe(request, response);
			}
		}
		
		protected void doSubscribe(HttpServletRequest request,
				HttpServletResponse response) throws ServletException,
				IOException {

			String path = request.getRequestURL().toString();

			// find the service for the subscription;
			BrisaService subscriptionService = brisaDevice
					.getServiceByEventSubURL(path);

			if (subscriptionService != null) {

				// if nt: upnp:event is a valid subscription;
				String nt = "";
				Enumeration nts = request.getHeaders("NT");
				if (nts != null && nts.hasMoreElements()) {
					nt = (String) nts.nextElement();
				}

				if (nt.equals("upnp:event")) {

					String userAgent = "";
					Enumeration values = request.getHeaders("USER-AGENT");
					if (values != null && values.hasMoreElements()) {
						userAgent = (String) values.nextElement();
					}

					ArrayList<String> callback = new ArrayList<String>();
					Enumeration callbacks = request.getHeaders("CALLBACK");
					if (callbacks != null && callbacks.hasMoreElements()) {
						
						String ipCallback = ((String)callbacks.nextElement()).replace("<", "").replace(">", "");
						callback.add(ipCallback);
					}

					int realTimeout = 0;
					Enumeration timeouts = request.getHeaders("TIMEOUT");
					if (timeouts != null && timeouts.hasMoreElements()) {
						String timeout = (String) timeouts.nextElement();
						timeout = timeout.replace("Second-", "");
						
						if(!timeout.equals("infinite"))
							realTimeout = Integer.parseInt(timeout);
					}

					BrisaEventSubscription brisaSub = new BrisaEventSubscription("", callback, realTimeout);
					subscriptionService.addEventSubscription(brisaSub);

					//retornar mensagem para control point;
					/*
					response.setStatus(HttpServletResponse.SC_OK);
					
			        response.setDateHeader("DATE", System.currentTimeMillis());  
			        response.setHeader("SERVER", "Mac OsX UPnP/1.1 JBRisa/1.0");
			        response.setHeader("SID", UUIDFactory.getCompleteUiid());
			        response.setIntHeader("CONTENT-LENGHT", 0);
			        response.setHeader("TIMEOUT", "Second-"+realTimeout);
			        */
				}
			}

		}
		
		protected void doGet(HttpServletRequest request,
				HttpServletResponse response) throws ServletException, IOException {
			
			//Return location xml;
			if(request.getPathInfo().equals("/"+brisaDevice.getFileAddress()))
			{
				response.setContentType("text/xml");
				response.setStatus(HttpServletResponse.SC_OK);
				
				response.getWriter().println(brisaDevice.getLocationXML());
			}
			//TODO if(request.getPathInfo().equals("/SwitchPower-scpd.xml"))
			else 
			{
				BrisaService service = brisaDevice.getServiceByScdp(request.getPathInfo());
				
				if(service != null)
				{
					response.setContentType("text/xml");
					response.setStatus(HttpServletResponse.SC_OK);
					
					XMLOutputter out = new XMLOutputter(Format.getPrettyFormat());
					String aux = out.outputString(service.getScdpDocument());
					
					response.getWriter().println(aux);
				}
				else{
					super.doGet(request, response);
				}
			}
		}

		/**
		 * Metodo utilizado para receber as mensagens de POST;
		 */
		@Override
		protected void doPost(HttpServletRequest req, HttpServletResponse resp)
				throws ServletException, IOException {

			BufferedReader reader = req.getReader();
			StringBuilder sb = new StringBuilder();
			String line = reader.readLine();
			while (line != null) {
				sb.append(line + "\n");
				line = reader.readLine();
			}
			reader.close();
			String data = sb.toString();

			String envelopeToSend = parsePostData(data);
			
			resp.setContentType("text/html");
	        resp.setStatus(HttpServletResponse.SC_OK);
	        resp.getWriter().println(envelopeToSend);
		}

		private String parsePostData(String data) {
			try {

				Element root = XmlManager.parseStringToXmlElement(data);
				Element body = XmlManager.getElementFromRoot(root, "Body",
						root.getNamespace());
				
				//When a action is invoked, we build a new BrisaAction object;
				BrisaAction brisaAction = new BrisaAction();

				for (Object action : body.getChildren()) {

					brisaAction.setName(((Element) action).getName());
					
					BrisaService service = new BrisaService(((Element) action).getNamespace().getURI());
					brisaAction.setService(service);
					
					ArrayList<BrisaArgument> arrayArg= new ArrayList<BrisaArgument>();

					for (Object argument : ((Element) action).getChildren()) {

						BrisaArgument argumentAdd = new BrisaArgument();
						argumentAdd.setName(((Element) argument).getName());
						argumentAdd.setValue(((Element) argument).getValue());
						
						arrayArg.add(argumentAdd);
					}
					
					brisaAction.setArgumentList(arrayArg);
				}

				//Ap�s criar o objeto brisaDevice, eu pego o BRisaService equivalente ao serviceType dele;
				BrisaService service = brisaDevice.getServiceByType(brisaAction.getService().getServiceType());
				
				if(service != null)
				{
						String name = brisaAction.getName();
						ArrayList<BrisaArgument> brin = new ArrayList<BrisaArgument>(); 

						if(brisaAction.getArgumentList().size() > 0)
						{
							brin = brisaAction.getArgumentList();
						}
						
						@SuppressWarnings("unchecked")
						ArrayList<BrisaArgument> brarg = (ArrayList<BrisaArgument>) service.getClass().getMethod(name.substring(0,1).toLowerCase()+name.substring(1, name.length()), ArrayList.class).
								invoke(service, brin);
						
						if(brarg == null)
							brarg = new ArrayList<BrisaArgument>();
						
						return returnSucessEnvelope(brisaAction.getName(), brisaAction.getService().getServiceType(), brarg);
				}
				else{
					return returnErrorEnvelope(501, "Falha no JBrisa");
				}

			} catch (Exception e) {

				BrisaLogger.logger.info("Problem while parsing incoming envelope");
				return returnErrorEnvelope(501, "Falha no JBrisa");
			}
		}
		
		private String returnErrorEnvelope(Integer errorCode, String errorDescription)
		{
			return "<s:Envelope xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" " +
	        		"s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">" +
	        		"<s:Body> " +
	        		"<s:Fault>" +
	        		"<faultcode>s:Client</faultcode> " +
	        		"<faultstring>UPnPError</faultstring> " +
	        		"<detail>" +
	        		"<UPnPError xmlns=\"urn:schemas-upnp-org:control-1-0\"> " +
	        		"<errorCode>"+errorCode+"</errorCode> " +
	        		"<errorDescription>"+errorDescription+"</errorDescription>" +
	        		"</UPnPError> " +
	        		"</detail>" +
	        		"</s:Fault> " +
	        		"</s:Body>" +
	        		"</s:Envelope>";
		}
		
		private String returnSucessEnvelope(String actionName, String serviceType, ArrayList<BrisaArgument> arrayArguments)
		{
			String answer = "<s:Envelope " +
					"xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" " +
					"s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">" +
					"<s:Body>" +
					"<u:"+actionName+"Response xmlns:u=\""+serviceType+"\">";
			
			for (BrisaArgument brisaArgument : arrayArguments) {
				answer += "<"+brisaArgument.getName()+">"+brisaArgument.getValue()+"</"+brisaArgument.getName()+">";
			}
					
			answer += "</u:"+actionName+"Response>" +
					"</s:Body>" +
					"</s:Envelope>";
			
			return answer;
			
		}
	}
	/*
	public class HelloWorld extends AbstractHandler
	{
	    public void handle(String target,
	                       Request baseRequest,
	                       HttpServletRequest request,
	                       HttpServletResponse response) 
	        throws IOException, ServletException
	    {
	        response.setContentType("text/html;charset=utf-8");
	        response.setStatus(HttpServletResponse.SC_OK);
	        baseRequest.setHandled(true);
	        response.getWriter().println("<h1>Hello World</h1>");
	    }
	}
	*/
}




