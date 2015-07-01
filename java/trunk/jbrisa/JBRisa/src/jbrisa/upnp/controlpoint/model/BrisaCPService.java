package jbrisa.upnp.controlpoint.model;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.net.InetAddress;
import java.net.Socket;
import java.util.ArrayList;
import java.util.Observable;

import jbrisa.utils.BrisaLogger;
import jbrisa.utils.XmlManager;

import org.jdom.Element;
import org.jdom.Namespace;


public class BrisaCPService extends Observable {
	
	public String serviceType;
	public String serviceId;
	public String SCPDURL;
	public String controlURL;
	public String eventSubURL;
	public String urlBase;
	
	public ArrayList<BrisaCPAction> actionList = new ArrayList<BrisaCPAction>();
	public ArrayList<BrisaCPStateVariable> serviceStateTable = new ArrayList<BrisaCPStateVariable>();


	public void call(String actionName)
	{
		call(actionName, new ArrayList<BrisaCPArgumentValue>());
	}
	
	public void call(String actionName, ArrayList<BrisaCPArgumentValue> arrayArguments)
	{
		 try {
			  String envelope = createEnvelope(actionName, serviceType, arrayArguments);
					
		      String hostname = urlBase.split(":")[1].replace("/", "");
		      int port = Integer.parseInt(urlBase.split(":")[2].replaceAll("/", ""));
		      String path = controlURL;
		      
		      //Create socket
		      InetAddress  addr = InetAddress.getByName(hostname);
		      Socket sock = new Socket(addr, port);
					
		      //Create header
		      BufferedWriter  wr = new BufferedWriter(new OutputStreamWriter(sock.getOutputStream(),"UTF-8"));
		      // You can use "UTF8" for compatibility with the Microsoft virtual machine.
		      wr.write("POST " + path + " HTTP/1.0\r\n");
		      wr.write("Host: "+hostname+"\r\n");
		      wr.write("Content-Length: " + envelope.length() + "\r\n");
		      wr.write("Content-Type: text/xml; charset=\"utf-8\"\r\n");
		      wr.write("SOAPACTION: \""+serviceType+"#"+actionName+"\"\r\n");
		      wr.write("\r\n");
					
		      //Send data
		      wr.write(envelope);
		      wr.flush();
					
		      // Response
		      BufferedReader rd = new BufferedReader(new InputStreamReader(sock.getInputStream()));
		      
		      String line = "";
		      String aux = "";
		      Boolean foundEnvelope = false;
		      
		      while((aux = rd.readLine()) != null){
		    	  if(aux.contains("Envelope"))
		    		  foundEnvelope = true;
		    	  
		    	  if(foundEnvelope)
		    		  line += aux;
		      }
		      
		      parseUpnpAnswer(line, actionName, serviceType);
		      
		    } catch (Exception e) {
		      e.printStackTrace();
		    }

	}
	
	private void parseUpnpAnswer(String line, String actionName, String serviceType) {
		
		try {
			BrisaLogger.logger.info(line);
			
			ArrayList<BrisaCPArgumentValue> array = new ArrayList<BrisaCPArgumentValue>();
			//String xml = line.substring(line.indexOf("<s:Envelope"), line.length());
			String xml = line;
			
			Element root = XmlManager.parseStringToXmlElement(xml);
			
			Element body = root.getChild("Body", root.getNamespace());
			Element fault = body.getChild("Fault", body.getNamespace());
			
			//IF THE ANSWER HAS THE FAULT NODE;
			if(fault != null)
			{
				BrisaCPActionFault faultAnswer = new BrisaCPActionFault();
				faultAnswer.faultCode = fault.getChild("faultcode").getValue();
				faultAnswer.faultString = fault.getChild("faultstring").getValue();
				
				Element detail = fault.getChild("detail");
				Element upnpError = detail.getChild("UPnPError", Namespace.getNamespace("urn:schemas-upnp-org:control-1-0"));
				faultAnswer.errorCode = upnpError.getChild("errorCode",upnpError.getNamespace()).getValue();
				faultAnswer.errorDescription = upnpError.getChild("errorDescription", upnpError.getNamespace()).getValue();
				
				//dispatchActionFail(faultAnswer);
				setChanged();
				notifyObservers(faultAnswer);
				
			}
			else{
				Element actionElement = body.getChild(actionName+"Response", Namespace.getNamespace(serviceType)); 
				
				for (Object e : actionElement.getChildren()) {
					
					BrisaCPArgumentValue arg = new BrisaCPArgumentValue();
					arg.setName(((Element) e).getName());
					arg.setValue(actionElement.getChild(((Element) e).getName()).getValue());
					array.add(arg);
				} 
				
				//dispatchActionResult(array);
				setChanged();
				notifyObservers(array);
			}

		} catch (Exception e) {
			// TODO: handle exception;
			e.printStackTrace();
		}
		
	}

	private String createEnvelope(String actionName, String serviceType,
			ArrayList<BrisaCPArgumentValue> arrayArguments) {
		
		String arguments = "";
		
		for (BrisaCPArgumentValue argument : arrayArguments) {
			arguments += "<"+argument.getName()+">"+argument.getValue()+"</"+argument.getName()+">"; 
		}
		
		String envelopeXml =  
				  "<?xml version=\"1.0\"?>" +
			      "<s:Envelope " +
			      "xmlns:s=\"http://schemas.xmlsoap.org/soap/envelope/\" " +
			      "s:encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\">" +
			      "<s:Body>" +
			      	"<u:"+actionName+" xmlns:u=\""+serviceType+"\">" +
			      		arguments+
			      	"</u:"+actionName+">" +
			      "</s:Body>" +
			      "</s:Envelope>";
		
		BrisaLogger.logger.info(envelopeXml);
		
		return envelopeXml;
	}

	public String getServiceType() {
		return serviceType;
	}

	public void setServiceType(String serviceType) {
		this.serviceType = serviceType;
	}

	public String getServiceId() {
		return serviceId;
	}

	public void setServiceId(String serviceId) {
		this.serviceId = serviceId;
	}

	public String getSCPDURL() {
		return SCPDURL;
	}

	public void setSCPDURL(String sCPDURL) {
		SCPDURL = sCPDURL;
	}

	public String getControlURL() {
		return controlURL;
	}

	public void setControlURL(String controlURL) {
		this.controlURL = controlURL;
	}

	public String getEventSubURL() {
		return eventSubURL;
	}

	public void setEventSubURL(String eventSubURL) {
		this.eventSubURL = eventSubURL;
	}

	public String getUrlBase() {
		return urlBase;
	}

	public void setUrlBase(String urlBase) {
		this.urlBase = urlBase;
	}

	public ArrayList<BrisaCPAction> getActionList() {
		return actionList;
	}

	public void setActionList(ArrayList<BrisaCPAction> actionList) {
		this.actionList = actionList;
	}

	public ArrayList<BrisaCPStateVariable> getServiceStateTable() {
		return serviceStateTable;
	}

	public void setServiceStateTable(
			ArrayList<BrisaCPStateVariable> serviceStateTable) {
		this.serviceStateTable = serviceStateTable;
	}
}
